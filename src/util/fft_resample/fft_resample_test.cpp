#include "fft_resample.h"
#include "util/audio_format_trans.h"
#include <stdio.h>

int main(int argc,char* argv[]){
  size_t in_rate = 48000,out_rate = 16000;
  const char* in_file ="./in.pcm";
  const char* out_file = "out.pcm";
  FILE* fin = fopen(in_file,"rb");
  FILE* fout = fopen(out_file,"wb");
  if(!fin || !fout){
    printf("open file error\n");
    return 0;
  }
  fft_resample resampler(in_rate,out_rate);
  if(0 != resampler.init()) {
    printf("resample init error\n");
    return 0;
  }
  int read_size=0;
  int insize = resampler.get_one_frame_size();
  int outsize = insize * out_rate / in_rate;
  int16_t* audio_buffer = new int16_t[insize];
  float* out_buff = new float[outsize];
  float* in_buff = new float[insize];
  while(read_size = fread(audio_buffer,sizeof(short),insize,fin)){
    int16_t_to_float(audio_buffer,in_buff,insize); 
    resampler.process(in_buff,out_buff,insize);
    fwrite(out_buff,sizeof(float),outsize,fout);
  }
  delete[] audio_buffer;
  delete[] in_buff;
  delete[] out_buff;
  return 0;
}
