/* service_sobel.h */

#ifndef __SERVICE_SOBEL_H__
#define __SERVICE_SOBEL_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>
#include <math.h>

int32_t run_service_sobel_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_sobel_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_sobel_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_sobel_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_sobel_overlay(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);

// internal maxfile interface(s)
void run_service_sobel_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t rows, uint32_t columns);

#endif /* __SERVICE_SOBEL_H__ */
