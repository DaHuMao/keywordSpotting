#ifndef _SRC_UTIL_KWS_LOCK_H_
#define _SRC_UTIL_KWS_LOCK_H_
#include<pthread.h>
class kws_lock{
  public:
    kws_lock();
    ~kws_lock();
   void  lock();
   void  unlock();
  private:
    pthread_mutex_t _mutex;
};

class kws_power_lock{
public:
  kws_power_lock(kws_lock* left);
  //kws_power_lock(kws_lock)=delete;
  ~kws_power_lock();
private:
  kws_lock* _lock;
};

#endif // _SRC_UTIL_KWS_LOCK_H_
