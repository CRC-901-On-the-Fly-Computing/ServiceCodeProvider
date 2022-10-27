/* service_fft.h */

#ifndef __SERVICE_FFT_H__
#define __SERVICE_FFT_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <stdio.h>
#include <math.h>
#include <complex.h>

int32_t run_service_fft_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t scale_factor);
int32_t run_service_fft_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t scale_factor);
int32_t run_service_fft_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t scale_factor);

#endif /* __SERVICE_FFT_H__ */
