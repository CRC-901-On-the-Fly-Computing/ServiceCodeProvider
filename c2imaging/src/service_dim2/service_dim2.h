/* service_dim2.h */

#ifndef __SERVICE_DIM2_H__
#define __SERVICE_DIM2_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <stdio.h>

int32_t run_service_dim2_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float factor);
int32_t run_service_dim2_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float factor);
int32_t run_service_dim2_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float factor);
int32_t run_service_dim2_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float factor);

// internal maxfile interface(s)
void run_service_dim2_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t picSize, float factor);

#endif /* __SERVICE_DIM2_H__ */
