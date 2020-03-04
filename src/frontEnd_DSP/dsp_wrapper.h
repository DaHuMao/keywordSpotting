// create by tongxiao.zhang
#ifndef SRC_DENOISE_DSP_WRAPPER_H_
#define SRC_DENOISE_DSP_WRAPPER_H_
#include "interface_define.h"
#include "ptr_operation.hpp"
namespace kws_ztx {
class dsp_wrapper : public dsp_base {
 public:
  ~dsp_wrapper();
  explicit dsp_wrapper(customDataBase* p_custom, int32_t channel_nums);
  CODE_STATUS init_dsp(void* param);
  CODE_STATUS process_dsp(void* data, size_t dataLen,
                                  kws_data_flow* data_flow);
  CODE_STATUS destroy_dsp();

 private:

 private:
};
}  //  namespace kws_ztx

#endif  // SRC_DENOISE_DSP_WRAPPER_H_
