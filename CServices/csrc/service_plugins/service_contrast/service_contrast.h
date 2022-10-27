/* service_contrast.h */

#ifndef __SERVICE_CONTRAST_H__
#define __SERVICE_CONTRAST_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <stdio.h>

int32_t run_service_contrast_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t factor);
int32_t run_service_contrast_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t factor);
int32_t run_service_contrast_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t factor);
int32_t run_service_contrast_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t factor);

// internal maxfile interface(s)
void *run_service_contrast_max_k1(uint16_t *dataIn, uint32_t rows, uint32_t columns, uint32_t alignedPicSize, uint16_t *minVal, uint16_t *maxVal);
void run_service_contrast_max_k2(void *engine, uint16_t *dataOut, uint32_t rows, uint32_t columns, uint32_t alignedPicSize, float x);
void run_service_contrast_max_k3(uint16_t *dataOut, uint16_t *dataIn, uint32_t columns, uint32_t rows, float x);

#endif /* __SERVICE_CONTRAST_H__ */
