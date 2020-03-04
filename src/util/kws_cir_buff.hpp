#ifndef _SRC_UTIL_KWS_CIR_BUFF_H_
#define _SRC_UTIL_KWS_CIR_BUFF_H_
#include<stdint.h>
#include<iostream>
#include <string.h>
#include "kws_lock.h"

template<class T>
class kws_cir_buf{
public:
  kws_cir_buf(uint64_t _size);
  ~kws_cir_buf();
  uint64_t read_buf(T* tar_ptr,uint64_t tar_size);
  uint64_t pop_buf(uint64_t _size);
  uint64_t write_buf(T* tar_ptr,uint64_t tar_size);
  bool full();
  uint64_t capacity();
  uint64_t size();
  void clear();
  void print();
private:
  kws_lock lock;
  T* ptr;
  uint64_t cur_index;
  uint64_t buf_size;
  uint64_t cur_size;
};

template<class T>
kws_cir_buf<T>::kws_cir_buf(uint64_t _size):buf_size(_size),cur_size(0),
                                         ptr(nullptr),cur_index(0){
  ptr=new T[buf_size];
}

template<class T>
kws_cir_buf<T>::~kws_cir_buf(){
  delete[] ptr;
}

template<class T>
uint64_t kws_cir_buf<T>::read_buf(T* tar_ptr,uint64_t tar_size){
  auto tmp=kws_power_lock(&lock);
  if(tar_size > cur_size) return 0;
  int64_t rest = (cur_index + tar_size) - buf_size;
  if(rest <= 0){
    memcpy(tar_ptr,ptr+cur_index,tar_size*sizeof(T));
  }else{
     uint64_t tmp_size = buf_size - cur_index;
     memcpy(tar_ptr,ptr+cur_index,tmp_size*sizeof(T));
     memcpy(tar_ptr+tmp_size, ptr, rest*sizeof(T));
  }
  return tar_size;
}


template<class T>
uint64_t kws_cir_buf<T>::pop_buf(uint64_t _size){
  auto tmp = kws_power_lock(&lock);
  if(_size > cur_size) return 0;
  cur_size -= _size;
  cur_index = (cur_index + _size)%buf_size;
  return _size;
}


template<class T>
uint64_t kws_cir_buf<T>::write_buf(T* tar_ptr,uint64_t tar_size){
  auto tmp=kws_power_lock(&lock);
  if(cur_size + tar_size > buf_size) return 0;
  uint64_t endindex = cur_index + cur_size;
  if(endindex < buf_size){
    if(endindex + tar_size < buf_size)
      memcpy(ptr + endindex,tar_ptr,tar_size*sizeof(T));
    else{
      uint64_t tmp_size = buf_size - endindex;
      memcpy(ptr + endindex,tar_ptr,tmp_size*sizeof(T));
      memcpy(ptr,tar_ptr  + tmp_size,(tar_size - tmp_size)*sizeof(T));
    }
  }else{
    uint64_t index=endindex%buf_size;
    memcpy(ptr+index,tar_ptr,tar_size*sizeof(T));
  }
  cur_size += tar_size;
  return tar_size;
}


template<class T>
bool kws_cir_buf<T>::full(){
  auto tmp=kws_power_lock(&lock);
  return cur_size == buf_size;
}


template<class T>
uint64_t kws_cir_buf<T>::capacity(){
  return buf_size;
}


template<class T>
uint64_t kws_cir_buf<T>::size(){
  auto tmp=kws_power_lock(&lock);
  return cur_size;
}


template<class T>
void kws_cir_buf<T>::clear(){
  auto tmp=kws_power_lock(&lock);
  cur_size = 0;
  buf_size = 0;
  cur_index = 0;
}


template<class T>
void kws_cir_buf<T>::print(){
  auto tmp=kws_power_lock(&lock);
  //std::cout<<"cur_size: "<<cur_size<<" cur_index: "<<cur_index<<std::endl;
  for(int i=0;i<cur_size;++i){
    std::cout<<ptr[(cur_index+i)%buf_size]<<" ";
  }
  std::cout<<std::endl;
}
#endif // _SRC_UTIL_KWS_CIR_BUFF_H_
