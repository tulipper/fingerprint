#pragma once
#include "complex.h"

complex* FFT_CooleyTukey(complex* x, int N, int N1, int N2);
complex* FFT_GoodThomas(complex* x, int N, int N1, int N2);
complex* DFT_naive(complex* x, int N);
void FFT2(complex **inout, int height, int width);
void IFFT2(complex **inout, int height, int width);
