#include "util/kws_thread.h"
#include <string.h>

namespace kws_ztx {
int32_t kws_thread::start() {
  int32_t ret = 0;
  _is_detach = false;
  td_b->start();
  pthread_attr_t attr;
  int stacksize =
      4 * 1024 * 1024; /*thread 堆栈设置为80M，stacksize以字节为单位。*/
  ret = pthread_attr_init(&attr); /*初始化线程属性*/
  if (ret != 0) return -1;

  ret = pthread_attr_setstacksize(&attr, stacksize);
  if (ret != 0) return -1;
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  ret = pthread_create(&_thread_handle, &attr, run, this);
  if (0 != ret) {
    LOGE_T(TAG) << "create thread failed error num is" << ret
                << ",start thread failed:" << strerror(ret);
  }
  //pthread_setname_np(_thread_handle, "hobot_audio_thread");
  pthread_attr_destroy(&attr);
  LOGE_T(TAG) << "create thread success";
  return ret;
}

int32_t kws_thread::stop() {
  int32_t ret = 0;
  LOGI_T(TAG) << "stop thread enter is_detach " << _is_detach;
  if(nullptr != td_b) td_b->stop();
  if (!_is_detach) {
    LOGI_T(TAG) << "start pthread join"<<td_b;
    ret = pthread_join(_thread_handle, NULL);
    if (0 != ret) {
      LOGE_T(TAG) << "stop thread failed error num is " << ret
                  << ",stop thread failed:" << strerror(ret);
    }
    LOGI_T(TAG) << "stop thread success";
    _thread_handle = 0;
  }
  return ret;
}

int32_t kws_thread::detach() {
  // LOGI_T(TAG) << "enter detach";
  if (_is_detach == true) {
    return 0;
  }
  _is_detach = true;
  int32_t ret = 0;
  if (0 == _thread_handle) {
    return ret;
  }
  ret = pthread_detach(_thread_handle);
  if (0 != ret) {
    LOGE_T(TAG) << "detach thread failed error num is" << ret
                << ",detach thread failed:" << strerror(ret);
  }
  // LOGI_T(TAG) << "detach ret is " << ret;
  return ret;
}

void *kws_thread::run(void *param) {
  kws_thread *pThis = static_cast<kws_thread *>(param);
  if (pThis && pThis->td_b) {
    LOGI_T(TAG)<< "pthis: "<<pThis->td_b;
    pThis->td_b->run_kws_thread();
  } else {
    LOGE_T(TAG) << "pThis is NULL";
  }
  pthread_exit(NULL);
  return NULL;
}
}  // namespace kws_ztx
