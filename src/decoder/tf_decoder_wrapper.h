// create by tongxiao.zhang
//
#ifndef SRC_DECODER_TF_DECODER_WRAPPER_H_
#define SRC_DECODER_TF_DECODER_WRAPPER_H_

#include <string.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <unordered_set>
#include <vector>

#include "interface_define.h"

#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/numbers.h"
#include "tensorflow/core/lib/strings/str_util.h"
#include "tensorflow/core/lib/wav/wav_io.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/command_line_flags.h"
//#include "accuracy_utils.h"
#include "recognize_commands.h"

#include "util/kws_cir_buff.hpp"
#include "util/audio_format_trans.h"
#include "stringHandle.h"
#include "fft_resample/fft_resample.h"
#include "circ_buff.h"
using tensorflow::Flag;
using tensorflow::Status;
using tensorflow::Tensor;
using tensorflow::int32;
using tensorflow::int64;
using tensorflow::string;
using tensorflow::uint16;
using tensorflow::uint32;
namespace kws_ztx {
class tf_decoder_wrapper : public decoder_base, public thread_base{
 public:
  explicit tf_decoder_wrapper(customDataBase *p_custom, int32_t decoder_num);
  ~tf_decoder_wrapper();
  CODE_STATUS init_decoder(enginCallbackFunc enginCallback_wakeup,
                               void *param);
  CODE_STATUS start_decoder();
  CODE_STATUS process_decoder(void *data, size_t dataLen);
  CODE_STATUS stop_decoder();
  CODE_STATUS destroy_decoder();

  CODE_STATUS decoder_data_proc();
  
  virtual void run_kws_thread();

 private:

  Status load_graph(const string& graph_file_name,
                 std::unique_ptr<tensorflow::Session>* session);
  Status read_labels_file(const string& file_name, std::vector<string>* result);

  void write_buf(float* buf,uint64_t size);
  int runrun();
 private:

  fft_resample* fft_resampler;
  float* res_cache_in;
  float* res_cache_out;
  tCircularBuffer* cir_cache_buf;
  size_t res_cache_in_size;
  size_t res_cache_out_size;

  int32 clip_duration_ms;
  int32 clip_stride_ms;
  int32 average_window_ms;
  int32 time_tolerance_ms;
  int32 suppression_ms;
  uint32 sample_rate;

  float detection_threshold;
  int32_t _decoder_num;
  uint64_t frame_num;

  string input_data_name;
  string input_rate_name;
  string output_name;

  std::unique_ptr<tensorflow::Session> session;
  Tensor* audio_data_tensor;
  Tensor* sample_rate_tensor;
  tensorflow::RecognizeCommands* recognize_commands;
  std::vector<string> labels_list;
  
  kws_cir_buf<float>* pbuf;
  float* decoder_data;
  float* cache_buf;

  static uint64_t _wake_up_times;
  static customDataBase *_custom_data;
  static enginCallbackFunc speechCallback;

};
}  // namespace kws_ztx

#endif  // SRC_DECODER_TF_DECODER_WRAPPER_H_
