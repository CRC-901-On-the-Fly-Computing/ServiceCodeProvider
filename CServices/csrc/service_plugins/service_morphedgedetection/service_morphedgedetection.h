/* service_morphedgedetection.h */

#ifndef __SERVICE_MORPHEDGEDETECTION_H__
#define __SERVICE_MORPHEDGEDETECTION_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>
#include <stdio.h>

int32_t run_service_morphedgedetection_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t matrix_size);
int32_t run_service_morphedgedetection_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t matrix_size);
int32_t run_service_morphedgedetection_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t matrix_size);
int32_t run_service_morphedgedetection_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t matrix_size);
int32_t run_service_morphedgedetection_overlay(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t matrix_size);

// internal maxfile interface(s)
void run_service_morphedgedetection_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t rows, uint32_t columns, uint32_t matrix_size);

#endif /* __SERVICE_MORPHEDGEDETECTION_H__ */
