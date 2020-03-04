// created by tongxiao.zhang
#ifndef SRC_INTERFACE_INTERFACE_H_
#define SRC_INTERFACE_INTERFACE_H_
#include "kwslog/log.hpp"
#include "include/kws_sdk.h"
#include "util/ptr_operation.hpp"
namespace kws_ztx {

typedef uint32_t (*input_data_cb)(char* ptr,uint16_t size);

class kws_data_flow{
public: 
  kws_data_flow():ptr(nullptr),size(0){}
  ~kws_data_flow(){
    delete[] ptr;
  }
  void resize(size_t _size){
    if(nullptr != ptr)
      delete[] ptr;
    ptr = new int16_t[size]; 
  }
  int16_t* ptr;
  size_t size;
};

class thread_base{
public:
  virtual void run_kws_thread() = 0;
  inline void start(){stop_thread = false;}
  inline void stop(){ stop_thread = true;}
  inline bool is_stop() const{return stop_thread;}
private:
  bool stop_thread;
};

class config_base {
 protected:
  static kws_config_s* _cfg;
};

class customDataBase : public config_base {
 public:
  virtual ~customDataBase() {}
  virtual void insert_deno_param(void** p_param, size_t p_num) = 0;
  virtual void insert_deco_param(void** p_param, size_t p_num) = 0;
  virtual CODE_STATUS set_dsp(void* param) = 0;
  virtual CODE_STATUS set_decoder(void* param) = 0;
};

typedef void (*enginCallbackFunc)(void* dataFromHesr);
class decoder_base : public config_base {
 public:
  virtual ~decoder_base() {}
  virtual CODE_STATUS init_decoder(enginCallbackFunc enginCallback_wakeup,
                                       void* param) = 0;
  virtual CODE_STATUS start_decoder() = 0;
  virtual CODE_STATUS process_decoder(void* data, size_t dataLen) = 0;
  virtual CODE_STATUS stop_decoder() = 0;
  virtual CODE_STATUS destroy_decoder() = 0;

 protected:
  enginCallbackFunc m_enginCallback_wakeup;
};

class dsp_base : public config_base {
 public:
  virtual ~dsp_base() {}
  virtual CODE_STATUS init_dsp(void* param) = 0;
  virtual CODE_STATUS process_dsp(void* data, size_t dataLen,
                                          kws_data_flow* outData) = 0;
  virtual CODE_STATUS destroy_dsp() = 0;
 protected:
  customDataBase* _custom_data;
};
}  // namespace kws_ztx
#endif  // SRC_INTERFACE_INTERFACE_H_
