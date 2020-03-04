#ifndef SRC_IMPL_KWS_ENGINE_H_
#define SRC_IMPL_KWS_ENGINE_H_
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <deque>
#include <fstream>
#include <string>
#include <vector>
#include "include/kws_sdk.h"
#include "interface_define.h"
#include "util/stringHandle.h"
#include "util/ptr_operation.hpp"
#include "util/kws_thread.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID


#endif

#define KWSAG "KWS_ENGINE"
#define KWSAGTEST "KWSTEST"
namespace kws_ztx {

typedef enum class KWS_ENGINE_PARAM{
  kINPUT_DATA_CB=0

}KWS_ENGINE_PARAM;

class KwsEngine : public config_base, public thread_base {
 public:
  static KwsEngine &instance() { return _s_instance; }
  ~KwsEngine();

  std::vector<decoder_base *>* _v_decoder;

  std::vector<kws_thread *>* _v_kws_thread;
  dsp_base *_denoise;

  kws_data_flow data_flow;


  void set_decoder_v(std::vector<decoder_base*> *_v) { _v_decoder = _v; }
  void set_thread_v(std::vector<kws_thread*> *_v) { _v_kws_thread = _v; }
  void set_dsp(dsp_base *denoise) { _denoise = denoise; }

  CODE_STATUS init();

  CODE_STATUS set_engine_param(void* param,KWS_ENGINE_PARAM kmode);

  int32_t startEngine();

  int32_t processAuioData(char* rawData,int16_t raw_size);

  int32_t destoryEngine();

  int32_t stop();

  static void wakeupCallback(void *dataFromHesr) {

  }

  int32_t process_auioData(char* rawData,uint32_t raw_size); 

  virtual void run_kws_thread();
 private:
  KwsEngine();

  KwsEngine(const KwsEngine &obj);

  const KwsEngine &operator=(const KwsEngine &obj);

 private:
  bool _is_run;

  static bool _is_stop;

  std::string _log_save_path;  // 日志保存路径


  char *_one_frame_data;

  size_t _one_frame_size;

  input_data_cb _input_data_cb;



  // current sdk status
  static KWS_STATUS _engine_status;

  static KwsEngine _s_instance;
};

}  //  namespace kws_ztx
#endif  // SRC_IMPL_KWS_ENGINE_H_
