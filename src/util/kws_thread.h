
#ifndef SRC_UTIL_KWS_THREAD_H_
#define SRC_UTIL_KWS_THREAD_H_

#include <pthread.h>
#include <stdint.h>
#include "interface_define.h"
#include "kwslog/log.hpp"

static const char* TAG = "KWSTHREAD";
namespace kws_ztx {
class kws_thread {
 public:
  kws_thread(thread_base* p_td) : td_b(p_td), _is_detach(false) {LOGI_T(TAG)<<"td_b: "<< p_td;}

  kws_thread(const kws_thread& left) = delete;

  const kws_thread& operator=(const kws_thread& left) = delete;

  ~kws_thread() { detach(); }
  int32_t start();

  int32_t stop();

  int32_t detach();



 protected:
  pthread_t _thread_handle;
  thread_base* td_b;
 private:


  static void* run(void* param);

 private:
  bool _is_detach;
};
}  // namespace kws_ztx

#endif  // SRC_UTIL_KWS_THREAD_H_
