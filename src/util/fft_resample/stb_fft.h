// fast fourier transform library (suitable for power of 2 and non-power of 2) Public domain. See "unlicense" statement at the end of this file.
// stb_fft - v0.11 - 2018-12-28
//
// ZhiHan Gao - 200759103@qq.com
// USAGE
//
// This is a single-file library. To use it, do something like the following in one .c file.
//     #define STB_FFT_IMPLEMENTAION
//     #include "stb_fft.h"
//
//
// You can then #include this file in other parts of the program as you would with any other header file.
//
// The default real type is float.
// To define a macro named USE_DOUBLE_TYPE to change it to double.
// You can read the following function: STB_FFT,STB_IFFT,STB_FFT_R2C,STB_IFFT_C2R for easily usage
//
//

#ifndef _stb_fft_h_
#define _stb_fft_h_
#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_DOUBLE_TYPE
#define stb_real_t double
#else
#define stb_real_t float
#endif

typedef struct {
    stb_real_t real;
    stb_real_t imag;
} cmplx;

typedef struct {
    int count;
    int* radix;
    int* remainder;
    int* offsets;
} stb_fft_stages;

typedef struct {
    int N;
    cmplx* twiddles;
    cmplx* twiddles_ordered;
    stb_fft_stages stages;
} stb_fft_plan;

typedef struct {
    stb_fft_plan* half_plan;
    cmplx* buffer;
    cmplx* twiddles;
} stb_fft_real_plan;

stb_fft_plan* stb_fft_plan_dft_1d(int N);

stb_fft_real_plan* stb_fft_real_plan_dft_1d(int N);

void stb_fft_r2c_exec(stb_fft_real_plan* plan, const stb_real_t* in, cmplx* out);

void stb_fft_c2r_exec(stb_fft_real_plan* plan, const cmplx* in, stb_real_t* out);

void stb_fft_exec(const stb_fft_plan* plan, cmplx* in, cmplx* out);

void stb_ifft_exec(const stb_fft_plan* plan, cmplx* in, cmplx* out);

// for easily usage
void STB_FFT(cmplx* input, cmplx* output, int n);

void STB_IFFT(cmplx* input, cmplx* output, int n);

void STB_FFT_R2C(stb_real_t* input, cmplx* output, int n);

void STB_IFFT_C2R(cmplx* input, stb_real_t* output, int n);

#ifdef __cplusplus
}
#endif
#endif

