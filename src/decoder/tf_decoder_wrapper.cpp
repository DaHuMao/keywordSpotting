// Copyright horizon 2019
#include "tf_decoder_wrapper.h"
#include <fstream>
//std::ofstream f1("./log1.txt");
//std::ofstream f2("./log2.txt");
//FILE* fout1=fopen("./out1.pcm","wb+");
//FILE* fout2=fopen("./out2.pcm","wb+");
#define DECODERTAG "TF_DECODER"
namespace kws_ztx {
int64_t time_ave=0;
customDataBase *tf_decoder_wrapper::_custom_data = nullptr;
enginCallbackFunc tf_decoder_wrapper::speechCallback = nullptr;
tf_decoder_wrapper::tf_decoder_wrapper(customDataBase *p_custom, int32_t decoder_num)
    : _decoder_num(decoder_num),
     clip_duration_ms(1000),
     clip_stride_ms(30),
     average_window_ms(500),
     time_tolerance_ms(750),
     suppression_ms(1500),
     detection_threshold(0.55f),
     sample_rate(16000),
     pbuf(nullptr),
     decoder_data(nullptr),
     frame_num(0),
     input_data_name("decoded_sample_data:0"),
     input_rate_name("decoded_sample_data:1"),
     output_name("labels_softmax"),
     audio_data_tensor(nullptr),
     sample_rate_tensor(nullptr),
     recognize_commands(nullptr),
     fft_resampler(nullptr),
     res_cache_in(nullptr),
     res_cache_out(nullptr),
     cir_cache_buf(nullptr)
    {
      _custom_data = p_custom;
}
tf_decoder_wrapper::~tf_decoder_wrapper() {
}
CODE_STATUS tf_decoder_wrapper::init_decoder(
    enginCallbackFunc enginCallback_wakeup, void *param) {
  LOGI_T(DECODERTAG) << "start init_decoder";
  speechCallback = enginCallback_wakeup;
  std::string cfg_path =_cfg->config_file_path;
  strHandle::opre_config opc(cfg_path + "/decoder_config.ini");
  LOGI_T(DECODERTAG)<< "start read " << cfg_path << "/decoder_config.ini";
  if(strHandle::strtoi(opc.getvalue("clip_stride_ms"),&clip_stride_ms) == 0){
    LOGI_T(DECODERTAG)<<"set clip_stride_ms: " << clip_stride_ms;
  }
  if(strHandle::strtoi(opc.getvalue("clip_duration_ms"),&clip_duration_ms) == 0){
    LOGI_T(DECODERTAG)<<"set clip_duration_ms: "<< clip_duration_ms;
  }
  if(strHandle::strtoi(opc.getvalue("average_window_ms"),&average_window_ms) == 0){
    LOGI_T(DECODERTAG)<<"set average_window_ms: "<< average_window_ms;
  }
  if(strHandle::strtoi(opc.getvalue("suppression_ms"),&suppression_ms) == 0){
    LOGI_T(DECODERTAG)<<"set suppression_ms: "<<suppression_ms;
  }
  if(strHandle::strtoi(opc.getvalue("time_tolerance_ms"),&time_tolerance_ms) == 0){
    LOGI_T(DECODERTAG)<<"set time_tolerance_ms: "<<time_tolerance_ms;
  }
  if(strHandle::strtof(opc.getvalue("detection_threshold"),&detection_threshold) == 0){
    LOGI_T(DECODERTAG)<<"set detection_threshold: "<<detection_threshold;
  }
  if(strHandle::strtou(opc.getvalue("sample_rate"),&sample_rate) == 0){
    LOGI_T(DECODERTAG)<<"set sample_rate: "<<sample_rate;
  }

  pbuf=new kws_cir_buf<float>((clip_duration_ms + clip_stride_ms * 30) * sample_rate / 1000);
  decoder_data = new float[clip_duration_ms * sample_rate / 1000];
  cache_buf = new float[clip_duration_ms * sample_rate / 1000];
  if(_cfg->sample_rate != sample_rate){
    fft_resampler = new fft_resample(_cfg->sample_rate,sample_rate);
    if(fft_resampler->init() != 0){
      LOGE_T(DECODERTAG)<<" failed init fft_resample ";
      return CODE_STATUS::kCODE_ERROR;
    }
    res_cache_in_size = fft_resampler->get_one_frame_size();
    res_cache_out_size = res_cache_in_size * sample_rate /_cfg->sample_rate;
    res_cache_in = new float[res_cache_in_size];
    res_cache_out = new float[res_cache_out_size];
    cir_cache_buf = CreateCircularBuffer(clip_stride_ms * sample_rate * 40 / 1000);    
  }
  return CODE_STATUS::kCODE_OK;
}

CODE_STATUS tf_decoder_wrapper::start_decoder() {
  LOGI_T(DECODERTAG) << "start decoder , pthis addr: "<< this;
  std::string cfg_path =_cfg->config_file_path;
  strHandle::opre_config opc(cfg_path + "/decoder_config.ini");
  LOGI_T(DECODERTAG)<< "start read " << cfg_path << "/decoder_config.ini";
   
  std::string model_file_name = opc.getvalue("model_file_name");
  std::string model_file=cfg_path + "/" + model_file_name;
  if(model_file_name.empty()){
    LOGE_T(DECODERTAG) << "you should set mode_file_name in "<< cfg_path \
                       << "/decoder_config.ini";
    return CODE_STATUS::kCODE_ERROR;
  }
  LOGI_T(DECODERTAG) << "loading model from "<< model_file;
  Status load_graph_status = load_graph(model_file, &session);
  if (!load_graph_status.ok()) {
    LOGE_T(DECODERTAG) << load_graph_status;
    return CODE_STATUS::kCODE_ERROR;
  }
  LOGI_T(DECODERTAG) << "========successed loading model============";
  std::string labels = cfg_path + "/" + opc.getvalue("label_txt_name");
  LOGI_T(DECODERTAG) << "start read labels from "<< labels;
  Status read_labels_status = read_labels_file(labels, &labels_list);
  if (!read_labels_status.ok()) {
    LOGE_T(DECODERTAG) << read_labels_status;
    return CODE_STATUS::kCODE_ERROR;
  }
  audio_data_tensor = new Tensor (tensorflow::DT_FLOAT,
                           tensorflow::TensorShape({(clip_duration_ms*sample_rate)/1000, 1}));

  sample_rate_tensor = new Tensor(tensorflow::DT_INT32, tensorflow::TensorShape({}));
  sample_rate_tensor->scalar<int32>()() = sample_rate;

  recognize_commands = new tensorflow::RecognizeCommands(
      labels_list, average_window_ms, detection_threshold, suppression_ms);
  return CODE_STATUS::kCODE_OK;
}
void tf_decoder_wrapper::write_buf(float* write_ptr,uint64_t write_size){
  uint64_t size = pbuf->write_buf(write_ptr,write_size);
  if(size != write_size){
      LOGW_T(DECODERTAG) << "Write failed and the buffer may have run out of space ,30 frame data will be discarded"\
        <<size<<" "<<write_size<<" "<<pbuf->size();
      frame_num = frame_num +  30;
      pbuf->pop_buf(30 * clip_stride_ms * sample_rate / 1000);
  }
}
CODE_STATUS tf_decoder_wrapper::process_decoder(void *data, size_t dataLen) {
  kws_data_flow* data_flow = reinterpret_cast<kws_data_flow*>(data);
  LOGD_T(DECODERTAG) << " process_decoder data_size: "<<data_flow->size;
  int16_t_to_float(data_flow->ptr,cache_buf,data_flow->size);
  if(_cfg->sample_rate != sample_rate){
    FillBuffer(cir_cache_buf,reinterpret_cast<char*>(cache_buf),data_flow->size * sizeof(float));
    int read_size=0;
    while((read_size=ReadBuffer(cir_cache_buf,reinterpret_cast<char*>(res_cache_in),res_cache_in_size * sizeof(float)) \
        == (res_cache_in_size * sizeof(float)))){
     // fwrite(res_cache_in,sizeof(float),res_cache_in_size,fout1); 
      if(0 != fft_resampler->process(res_cache_in,res_cache_out,res_cache_in_size)){
        LOGE_T(DECODERTAG)<<"fft_resample process error";
        return CODE_STATUS::kCODE_ERROR;
      }
     // LOGI_T(DECODERTAG)<<"----"<< read_size<<" "<<res_cache_in_size;
     // fwrite(res_cache_out,sizeof(float),res_cache_out_size,fout2); 
      write_buf(res_cache_out, res_cache_out_size);
    }
  }else{
    write_buf(cache_buf, data_flow->size);
  }
  return CODE_STATUS::kCODE_OK;
}

CODE_STATUS tf_decoder_wrapper::stop_decoder() {
  CODE_STATUS ans = CODE_STATUS::kCODE_OK;
  return ans;
}

CODE_STATUS tf_decoder_wrapper::destroy_decoder() {
  CODE_STATUS ret = CODE_STATUS::kCODE_OK;
  save_free(&pbuf);
  save_free(&fft_resampler);
  save_free(&audio_data_tensor);
  save_free(&sample_rate_tensor);
  save_free(&recognize_commands);
  save_free_arr(&decoder_data);
  save_free_arr(&cache_buf);
  save_free_arr(&res_cache_in);
  save_free_arr(&res_cache_out);
  ret = stop_decoder();
  if (ret == CODE_STATUS::kCODE_OK) LOGI_T(DECODERTAG) << "destroy decoder success";
}

void tf_decoder_wrapper::run_kws_thread(){
  LOGI_T(DECODERTAG)<< "enter decoder run_kws_thread  " << is_stop() << " ==";
  while(!is_stop()){
    if(CODE_STATUS::kCODE_OK != decoder_data_proc()){
      LOGE_T(DECODERTAG) << "decoder_data_proc error";
      return;
    }
  }
  LOGI_T(DECODERTAG)<< "end decoder run_kws_thread";
}

CODE_STATUS tf_decoder_wrapper::decoder_data_proc(){
  uint32_t count = clip_duration_ms * sample_rate / 1000;
  LOGD_T(DECODERTAG) << "decoder_data_proc data_size: "<<count;
  if(pbuf->read_buf(decoder_data,count) != count){
    LOGD_T(DECODERTAG)<< "failed to read the data, the buffer may not have enough data";
    usleep(clip_stride_ms * 100);
  }else{
    int64_t start=getCurrentTime();
    const int64 clip_duration_samples = (clip_duration_ms * sample_rate) / 1000;
    const int64 clip_stride_samples = (clip_stride_ms * sample_rate) / 1000;
    pbuf->pop_buf((clip_stride_ms * sample_rate) / 1000);
    const int64 current_time_ms = frame_num * clip_stride_ms;
    ++frame_num;
    //tensorflow::StreamingAccuracyStats previous_stats;
    std::copy(decoder_data, decoder_data + clip_duration_samples, audio_data_tensor->flat<float>().data());
    // Actually run the audio through the model.
    std::vector<Tensor> outputs;
    Status run_status = session->Run({{input_data_name, *audio_data_tensor},
                                      {input_rate_name, *sample_rate_tensor}},
                                     {output_name}, {}, &outputs);
    if (!run_status.ok()) {
      LOGE_T(DECODERTAG) << "Running model failed: " << run_status;
      return CODE_STATUS::kCODE_ERROR;
    }
    
    string found_command;
    float score;
    bool is_new_command;
   /* f1<<current_time_ms<<": ";
    for(int i=0;i<8;++i){
      f1<<decoder_data[i]<<" ";
    }
    f1<<std::endl;*/
    Status recognize_status = recognize_commands->ProcessLatestResults(
        outputs[0], current_time_ms, &found_command, &score, &is_new_command);
    if (!recognize_status.ok()) {
      LOGE_T(DECODERTAG) << "Recognition processing failed: " << recognize_status;
      return CODE_STATUS::kCODE_ERROR;
    }
    if(is_new_command){
      if(found_command != "_silence_")
        LOGI_T(DECODERTAG) << (frame_num-1) * clip_stride_ms << " ms: " << found_command << " : " << score;
    }
    time_ave = time_ave + (getCurrentTime() - start);
  }
  return CODE_STATUS::kCODE_OK;
}
int  tf_decoder_wrapper::runrun(){

  string wav="./record.wav";
  string wav_string;
  Status read_wav_status = tensorflow::ReadFileToString(
      tensorflow::Env::Default(), wav, &wav_string);
  if (!read_wav_status.ok()) {
    LOG(ERROR) << read_wav_status;
    return -1;
  }
  std::cout<<std::endl;
  std::vector<float> audio_data;
  uint32 sample_count;
  uint16 channel_count;
  uint32 sample_rate;
  Status decode_wav_status = tensorflow::wav::DecodeLin16WaveAsFloatVector(
      wav_string, &audio_data, &sample_count, &channel_count, &sample_rate);
  if (!decode_wav_status.ok()) {
    LOG(ERROR) << decode_wav_status;
    return -1;
  }
  if (channel_count != 1) {
    LOG(ERROR) << "Only mono .wav files can be used, but input has "
               << channel_count << " channels.";
    return -1;
  }

  const int64 clip_duration_samples = (clip_duration_ms * sample_rate) / 1000;
  const int64 clip_stride_samples = (clip_stride_ms * sample_rate) / 1000;
  Tensor audio_data_tensor(tensorflow::DT_FLOAT,
                           tensorflow::TensorShape({clip_duration_samples, 1}));

  Tensor sample_rate_tensor(tensorflow::DT_INT32, tensorflow::TensorShape({}));
  sample_rate_tensor.scalar<int32>()() = sample_rate;

  tensorflow::RecognizeCommands recognize_commands(
      labels_list, average_window_ms, detection_threshold, suppression_ms);

 // std::vector<std::pair<string, int64>> all_found_words;
  //tensorflow::StreamingAccuracyStats previous_stats;
  int i=0;
  const int64 audio_data_end = (sample_count - clip_duration_samples);
  for (int64 audio_data_offset = 0; audio_data_offset < audio_data_end;
       audio_data_offset += clip_stride_samples) {
    const float* input_start = &(audio_data[audio_data_offset]);
    const float* input_end = input_start + clip_duration_samples;
    std::copy(input_start, input_end, audio_data_tensor.flat<float>().data());
    // Actually run the audio through the model.
    std::vector<Tensor> outputs;
    Status run_status = session->Run({{input_data_name, audio_data_tensor},
                                      {input_rate_name, sample_rate_tensor}},
                                     {output_name}, {}, &outputs);
    if (!run_status.ok()) {
      LOG(ERROR) << "Running model failed: " << run_status;
      return -1;
    }

    const int64 current_time_ms = (audio_data_offset * 1000) / sample_rate;
    //f2<<current_time_ms<<": ";
    //for(int i=0;i<8;++i){
    //  f2<<input_start[i]<<" ";
    //}
    //f2<<std::endl;
    string found_command;
    float score;
    bool is_new_command;
    Status recognize_status = recognize_commands.ProcessLatestResults(
        outputs[0], current_time_ms, &found_command, &score, &is_new_command);
    if (!recognize_status.ok()) {
      LOG(ERROR) << "Recognition processing failed: " << recognize_status;
      return -1;
    }

    if (is_new_command ) {
     // all_found_words.push_back({found_command, current_time_ms});
      if(found_command != "_silence_")
        std::cout<< current_time_ms << "ms: " << found_command << ": " << score << std::endl;
    }
  }
}
// Reads a model graph definition from disk, and creates a session object you
// can use to run it.
Status tf_decoder_wrapper::load_graph(const string& graph_file_name,
                 std::unique_ptr<tensorflow::Session>* session) {
  tensorflow::GraphDef graph_def;
  Status load_graph_status =
      ReadBinaryProto(tensorflow::Env::Default(), graph_file_name, &graph_def);
  if (!load_graph_status.ok()) {
    return tensorflow::errors::NotFound("Failed to load compute graph at '",
                                        graph_file_name, "'");
  }
  session->reset(tensorflow::NewSession(tensorflow::SessionOptions()));
  Status session_create_status = (*session)->Create(graph_def);
  if (!session_create_status.ok()) {
    return session_create_status;
  }
  return Status::OK();
}

// Takes a file name, and loads a list of labels from it, one per line, and
// returns a vector of the strings.
Status tf_decoder_wrapper::read_labels_file(const string& file_name, std::vector<string>* result) {
  std::ifstream file(file_name);
  if (!file) {
    return tensorflow::errors::NotFound("Labels file '", file_name,
                                        "' not found.");
  }
  result->clear();
  string line;
  while (std::getline(file, line)) {
    result->push_back(line);
  }
  return Status::OK();
}


}  // namespace kws_ztx
