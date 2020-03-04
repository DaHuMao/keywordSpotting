#include "fft_resample.h"

#ifndef min
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
fft_resample::fft_resample(size_t samp_in,size_t samp_out)
  :handle(nullptr),
  inSampleRate(samp_in),
  outSampleRate(samp_out),
  nFFT(2){

}


fft_resample::~fft_resample(){
  FFT_Resampler_Free();
}

int fft_resample::init(){
  handle = new FFT_Resampler_Handle;
  return FFT_Resampler_Init();
}

int fft_resample::process(const float* in,float* out,int size){
  if(size != handle->inFrameSize) {
    LOGE_T(SAMPERTAG) << "input size is not equal "<<handle->inFrameSize;
    return -1;
  }
  return FFT_Resampler_Proc(in, out);
}

int fft_resample::get_one_frame_size(){
  return handle->inFrameSize;
}


void fft_resample::FFT_Resampler_Free()
{
    if (handle) {
        if (handle->inFifo) {
            free(handle->inFifo);
            handle->inFifo = NULL;
        }

        if (handle->inPlan) {
            free(handle->inPlan);
            handle->inPlan = NULL;
        }

        if (handle->outPlan) {
            free(handle->outPlan);
            handle->outPlan = NULL;
        }

        if (handle->samples) {
            free(handle->samples);
            handle->samples = NULL;
        }

        if (handle->synthesisMem) {
            free(handle->synthesisMem);
            handle->synthesisMem = NULL;
        }

        if (handle->inWindowing) {
            free(handle->inWindowing);
            handle->inWindowing = NULL;
        }

        if (handle->outWindowing) {
            free(handle->outWindowing);
            handle->outWindowing = NULL;
        }
        delete handle;
        handle = nullptr;
    }
}

void fft_resample::FFT_Resampler_reset()
{
    if (handle)
        handle->pos = 0;
}

int fft_resample::FFT_Resampler_Init()
{
    if (handle) {
        handle->pos = 0;
        if (outSampleRate < inSampleRate) {
            nFFT = nFFT * inSampleRate * 128 / outSampleRate;
        }
        else {
            nFFT = nFFT * outSampleRate * 128 / inSampleRate;
        }
        nFFT += (nFFT % 2);
        handle->inFrameSize = nFFT;
        handle->inWindowSize = nFFT * 2;
        handle->inSampleRate = inSampleRate;
        handle->inWindowing = (float *) calloc(handle->inFrameSize, sizeof(float));
        handle->inPlan = stb_fft_real_plan_dft_1d(handle->inWindowSize);
        handle->outSampleRate = outSampleRate;
        handle->outFrameSize = handle->inFrameSize * outSampleRate / inSampleRate;
        handle->outWindowSize = handle->outFrameSize * 2;
        handle->outWindowing = (float *) calloc(handle->outFrameSize, sizeof(float));
        handle->outPlan = stb_fft_real_plan_dft_1d(handle->outWindowSize);
        handle->inFifo = (float *) calloc(max(handle->inWindowSize, handle->outWindowSize), sizeof(float));
        handle->samples = (cmplx *) calloc(max(handle->inWindowSize, handle->outWindowSize), sizeof(cmplx));
        handle->synthesisMem = (float *) calloc(handle->outFrameSize, sizeof(float));
        if ((handle->inFifo == NULL) || (handle->inPlan == NULL) || (handle->outPlan == NULL)
            || (handle->samples == NULL)
            || (handle->synthesisMem == NULL) || (handle->inWindowing == NULL) || (handle->outWindowing == NULL)
            ) {
            FFT_Resampler_Free();
            return -1;
        }
        float norm = 1.0f / handle->inWindowSize;
        for (size_t i = 0; i < handle->inFrameSize; i++) {
            double t = sin(.5 * M_PI * (i + .5) / handle->inFrameSize);
            handle->inWindowing[i] = (float) sin(.5 * M_PI * t * t) * norm;
        }
        for (size_t i = 0; i < handle->outFrameSize; i++) {
            double t = sin(.5 * M_PI * (i + .5) / handle->outFrameSize);
            handle->outWindowing[i] = (float) sin(.5 * M_PI * t * t);
        }
        return 0;
    }
    return -1;
}

int fft_resample::FFT_Resampler_Proc(const float *input, float *output)
{
    if ((input == NULL) || (handle == NULL) || (output == NULL)) {
        return -1;
    }
    float *inFifo = handle->inFifo;
    float *synthesis_mem = handle->synthesisMem;
    for (size_t i = 0; i < handle->inFrameSize; i++) {
        inFifo[i] *= handle->inWindowing[i];
        inFifo[handle->inWindowSize - 1 - i] = input[handle->inFrameSize - 1 - i] * handle->inWindowing[i];
    }
    stb_fft_r2c_exec(handle->inPlan, inFifo, handle->samples);
    if (handle->outWindowSize < handle->inWindowSize) {
        int half_output = (handle->outWindowSize / 2);
        int diff_size = handle->inWindowSize - handle->outWindowSize;
        memset(handle->samples + half_output, 0, diff_size * sizeof(cmplx));
    }
    else if (handle->outWindowSize > handle->inWindowSize) {
        int half_input = handle->inWindowSize / 2;
        int diff_size = handle->outWindowSize - handle->inWindowSize;
        memmove(handle->samples + half_input + diff_size, handle->samples + half_input,
                half_input * sizeof(cmplx));
        memset(handle->samples + half_input, 0, diff_size * sizeof(cmplx));
    }
    stb_fft_c2r_exec(handle->outPlan, handle->samples, inFifo);
    for (size_t i = 0; i < handle->outFrameSize; i++) {
        output[i] = inFifo[i] * handle->outWindowing[i] + synthesis_mem[i];
        inFifo[handle->outWindowSize - 1 - i] *= handle->outWindowing[i];
    }
    memcpy(synthesis_mem, inFifo + handle->outFrameSize, handle->outFrameSize * sizeof(float));
    memcpy(inFifo, input, handle->inFrameSize * sizeof(float));
    if (handle->pos == 0) {
        handle->pos++;
        return 0;
    }
    handle->pos++;
    return 0;
}
