#ifndef _SRC_UTIL_FFT_RESAMPLE_FFT_RESAMPLE_H
#define _SRC_UTIL_FFT_RESAMPLE_FFT_RESAMPLE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stb_fft.h"
#include "kwslog/log.hpp"
#define SAMPERTAG "FFT_RESAMPLE"
typedef struct
{
    int inFrameSize;
    int inWindowSize;
    int inSampleRate;
    float *inWindowing;
    stb_fft_real_plan *inPlan;
    int outFrameSize;
    int outWindowSize;
    int outSampleRate;
    float *outWindowing;
    stb_fft_real_plan *outPlan;
    float *inFifo;
    float *synthesisMem;
    cmplx *samples;
    int pos;
} FFT_Resampler_Handle;
class fft_resample{
public:
  fft_resample(size_t samp_in,size_t samp_out);
  fft_resample();
  ~fft_resample();
  int init();
  int process(const float* in,float* out,int size);
  int get_one_frame_size();

private:
  void FFT_Resampler_Free();
  void FFT_Resampler_reset();
  int FFT_Resampler_Init();
  int FFT_Resampler_Proc(const float *input, float *output);
  FFT_Resampler_Handle* handle;
  size_t inSampleRate;
  size_t outSampleRate;
  size_t nFFT;
};

#endif // _SRC_UTIL_FFT_RESAMPLE_FFT_RESAMPLE_H

