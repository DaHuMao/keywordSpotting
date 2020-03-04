// Copyright horizon 2019
#include "dsp_wrapper.h"
#include <string>
#define DSPTAG "DSP_WRAPPER"
namespace kws_ztx {
dsp_wrapper::dsp_wrapper(customDataBase* p_custom, int32_t channel_nums)
     {
  _custom_data = p_custom;
}
dsp_wrapper::~dsp_wrapper() {
}
CODE_STATUS dsp_wrapper::init_dsp(void* param) {
  return CODE_STATUS::kCODE_OK;
}

CODE_STATUS dsp_wrapper::process_dsp(void* data, size_t data_len,
                                             kws_data_flow* data_flow) {
  //memcpy(data_flow->ptr,data,data_len);
  LOGD_T(DSPTAG) << "process_dsp success ";
  return CODE_STATUS::kCODE_OK;
}
CODE_STATUS dsp_wrapper::destroy_dsp() {
  return CODE_STATUS::kCODE_OK;
}

}  // namespace kws_ztx
