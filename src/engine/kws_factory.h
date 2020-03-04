// create by zhang.tongxiao
//
#ifndef SRC_IMPL_KWS_FACTORY_H_
#define SRC_IMPL_KWS_FACTORY_H_

#include "decoder/tf_decoder_wrapper.h"
#include "frontEnd_DSP/dsp_wrapper.h"
#include "engine/kws_engine.h"
namespace kws_ztx {

class factory : public config_base {
 public:
  factory();
  ~factory();
  CODE_STATUS product(kws_config_s* hrsc_cg);
  void destroy();
  static factory& getInstance();

 private:
  std::vector<decoder_base*> _decoder;
  std::vector<kws_thread*> _td_base;
  dsp_base* _dsp;
  static factory _instance;
};
}  // namespace kws_ztx

#endif  // SRC_IMPL_KWS_FACTORY_H_
