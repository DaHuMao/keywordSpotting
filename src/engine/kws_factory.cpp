// Copyright horizon 2019
// create by zhang.tongxiao
//
#include "engine/kws_factory.h"

#define FACTORY "KWSPRODUCT"
namespace kws_ztx {
factory factory::_instance;
kws_config_s* config_base::_cfg = nullptr;

factory::factory()
    :  _dsp(nullptr) {}
factory::~factory() {}
class testclass : public thread_base{
  virtual void run_kws_thread(){
    usleep(1000*1000);
    LOGI_T("run======")<<"++++++";
  }
};
CODE_STATUS factory::product(kws_config_s* hrsc_cg) {
  _cfg = hrsc_cg;
  
  tf_decoder_wrapper* decoder_ptr = new tf_decoder_wrapper(nullptr,1);
  _decoder.push_back(decoder_ptr);
  _dsp = new dsp_wrapper(nullptr,1);
  testclass* ptt = new testclass;
  thread_base* td_ptr = decoder_ptr;
  _td_base.push_back(new kws_thread(td_ptr));
  _td_base.push_back(new kws_thread(&KwsEngine::instance()));
  LOGI_T(FACTORY) << "decoder thread addr: "<<decoder_ptr<<" "<<td_ptr;
  LOGI_T(FACTORY) << "engine thread addr: "<< &KwsEngine::instance();
  KwsEngine::instance().set_decoder_v(&_decoder);
  KwsEngine::instance().set_dsp(_dsp);
  KwsEngine::instance().set_thread_v(&_td_base);

  return CODE_STATUS::kCODE_OK;
}
void factory::destroy() {
  LOGI_T(KWSAG) << "DESTROY";
  for(auto& decoder_e : _decoder){
    decoder_e->destroy_decoder();
    save_free(&decoder_e);
  }

  LOGI_T(KWSAG) << "_DSP";
  _dsp->destroy_dsp();
  save_free(&_dsp);

  LOGI_T(KWSAG) << "_THREAD_BASE";
  for(auto& td_e : _td_base){
    save_free(&td_e);
  }
  LOGI_T(KWSAG) << "DESTROY END";
}
factory& factory::getInstance() { return _instance; }
}  // namespace kws_ztx
