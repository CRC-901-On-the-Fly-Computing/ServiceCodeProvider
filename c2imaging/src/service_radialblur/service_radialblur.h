/* service_radialblur.h */

#ifndef __SERVICE_RADIALBLUR_H__
#define __SERVICE_RADIALBLUR_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <stdlib.h>
#include <math.h>
#include <string.h>

int32_t run_service_radialblur_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_radialblur_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_radialblur_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_radialblur_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);

// internal maxfile interface(s)
void *run_service_radialblur_max_k1(uint16_t *dataIn, uint32_t rows, uint32_t columns, uint32_t alignedPicSize);
void *run_service_radialblur_max_k2_1(void *engine, uint32_t rows, uint32_t columns, uint32_t alignedPicSize);
void *run_service_radialblur_max_k2_2(void *engine, uint32_t rows, uint32_t columns, uint32_t alignedPicSize);
void run_service_radialblur_max_k3(void *engine, uint16_t *dataOut, uint32_t rows, uint32_t columns, uint32_t alignedPicSize);

#endif /* __SERVICE_RADIALBLUR_H__ */
