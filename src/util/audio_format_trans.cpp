#include "audio_format_trans.h"
void int16_t_to_float(int16_t* in,float* out,size_t size){
  for(int i=0;i<size;++i){
    out[i]=static_cast<float>(in[i])/32768;
  }
}
uint64_t getCurrentTime() {
  auto epoch = std::chrono::system_clock::now().time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
}
