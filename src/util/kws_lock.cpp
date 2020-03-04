#include "kws_lock.h"

kws_lock::kws_lock(){
  pthread_mutex_init(&_mutex,nullptr);
}
kws_lock::~kws_lock(){
  pthread_mutex_destroy(&_mutex);
}
void kws_lock::lock(){
  pthread_mutex_lock(&_mutex);
}
void kws_lock::unlock(){
  pthread_mutex_unlock(&_mutex);
}

kws_power_lock::kws_power_lock(kws_lock* left):_lock(left){
  _lock->lock();
}
kws_power_lock::~kws_power_lock(){
  _lock->unlock();
}
