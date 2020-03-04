#include "engine/kws_engine.h"
#include <sys/stat.h>
#include <iostream>
#include <string>

namespace kws_ztx{
KwsEngine KwsEngine::_s_instance;

KwsEngine::KwsEngine()
    : _is_run(false),
      _input_data_cb(NULL)
      {
  std::cout << "Constructor KwsEngine" << std::endl;
}

CODE_STATUS KwsEngine::init() {
  int32_t ret = 0;
  

  if (_denoise->init_dsp(nullptr) != CODE_STATUS::kCODE_OK) {
    LOGE_T(KWSAG) << "failed init denoise ";
    return CODE_STATUS::kCODE_ERROR;
  }
  _one_frame_size = 30 * _cfg->sample_rate * _cfg->data_bit_wide / 1000;
  _one_frame_data = new char[_one_frame_size];
  //data_flow.resize(_one_frame_size * 2);
 // data_flow.size = _one_frame_size / sizeof(short);
  data_flow.ptr = reinterpret_cast<int16_t*>(_one_frame_data);
  data_flow.size=_one_frame_size/sizeof(int16_t);
  LOGI_T(KWSAG) << "enter createKwsEngine";
  int16_t decoder_index = 0;
  for (auto &_decoder : *_v_decoder) {
    if (_decoder->init_decoder(wakeupCallback, nullptr) != CODE_STATUS::kCODE_OK) {
      LOGE_T(KWSAG) << "failded initDecoder" << ++decoder_index;
      return CODE_STATUS::kCODE_ERROR;
    }
  }
  return CODE_STATUS::kCODE_OK;
}

int32_t KwsEngine::startEngine() {
  LOGI_T(KWSAG) << "prepare start engine";
  int32_t ret = -1;
  if (!_is_stop || _is_run) {
    LOGD_T(KWSAG) << "engine alread start";
    return ret;
  }
  int16_t decoder_index = 0;
  for (auto &_decoder : *_v_decoder) {
    if (_decoder->start_decoder() != CODE_STATUS::kCODE_OK) {
      LOGE_T(KWSAG) << "failed start decoder " << ++decoder_index;
      return ret;
    }
  }
  for(auto & _kws_thread : *_v_kws_thread){
    if(nullptr != _kws_thread)
      _kws_thread->start();
    else
      LOGE_T(KWSAG) << "failed start a thread";
  }
  _is_stop = false;
  _is_run = true;
  return 0;
}




int32_t KwsEngine::process_auioData(char *rawData, uint32_t raw_size) {
  int32_t ret = -1;
  if (_is_stop) return ret;

  LOGD_T(KWSAG) << "audio process enter";

  auto hisfRes = CODE_STATUS::kCODE_OK;
  LOGD_T(KWSAG) << "data_size: " << data_flow.size;
  hisfRes = _denoise->process_dsp(rawData, raw_size, &data_flow);
  if (hisfRes != CODE_STATUS::kCODE_OK) {
    LOGE_T(KWSAG) << "dsp proc error ret is " << ret;
    return -1;
  }
  // 数据送入解码器解析是否包含唤醒词
  int16_t decoder_index = 0;
  for (auto &_decoder : *_v_decoder) {
    if (_decoder->process_decoder(reinterpret_cast<void *>(&data_flow), sizeof(kws_data_flow)) !=
        CODE_STATUS::kCODE_OK)
      LOGE_T(KWSAG) << "failed processDecoder" << ++decoder_index;
  }

  return 0;
}




CODE_STATUS KwsEngine::set_engine_param(void* param,KWS_ENGINE_PARAM kmode){
  switch(kmode){
    case KWS_ENGINE_PARAM::kINPUT_DATA_CB:
          _input_data_cb = reinterpret_cast<input_data_cb>(param);
          break;
    default: 
          return CODE_STATUS::kCODE_ERROR;
  }
  return CODE_STATUS::kCODE_OK;
}

void KwsEngine::run_kws_thread(){
  LOGI_T(KWSAG)<< "enter kws_engine thread";
  while(!is_stop()){
    if(_input_data_cb(_one_frame_data,_one_frame_size) == _one_frame_size){
      if(0 != process_auioData(_one_frame_data,_one_frame_size)){
        LOGE_T(KWSAG) << "failed process_auioData";
        return ;
      } 
    } else{
        LOGE_T(KWSAG) << "failed read data from _input_data_cb";
    }
  }
  LOGI_T(KWSAG)<< "end kws_engine thread";
}

KWS_STATUS KwsEngine::_engine_status = KWS_STATUS::kUNWAKE_UP;

bool KwsEngine::_is_stop = true;

int32_t KwsEngine::stop() {
  int ret = -1;
  for(auto & _kws_thread : *_v_kws_thread){
    if(nullptr != _kws_thread)
      _kws_thread->stop();
  }

  for (auto &_decoder : *_v_decoder) {
    _decoder->stop_decoder();
  }
  _is_stop = true;
  return 0;
}


int32_t KwsEngine::destoryEngine(){}

KwsEngine::~KwsEngine() {

}
}  // namespace kws_ztx
