#ifndef _SRC_UTIL_AUDIO_FORMAT_TRANS_
#define _SRC_UTIL_AUDIO_FORMAT_TRANS_

#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#include<chrono>
void int16_t_to_float(int16_t* in,float* out,size_t size);
uint64_t getCurrentTime();
#endif // _SRC_UTIL_AUDIO_FORMAT_TRANS_
