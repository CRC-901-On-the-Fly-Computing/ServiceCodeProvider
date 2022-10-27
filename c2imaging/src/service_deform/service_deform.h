/* service_deform.h */

#ifndef __SERVICE_DEFORM_H__
#define __SERVICE_DEFORM_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <stdio.h>
#include <math.h>

int32_t run_service_deform_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float radius, float angle, int32_t positionx, int32_t positiony, int32_t mode);
int32_t run_service_deform_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float radius, float angle, int32_t positionx, int32_t positiony, int32_t mode);
int32_t run_service_deform_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float radius, float angle, int32_t positionx, int32_t positiony, int32_t mode);
int32_t run_service_deform_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float radius, float angle, int32_t positionx, int32_t positiony, int32_t mode);

// internal maxfile interface(s)
void run_service_deform_max_k1(uint32_t rows, uint32_t columns, int32_t picSize, int32_t midx, int32_t midy, float rad, float angle, uint32_t *posTemp);
void run_service_deform_max_k2(uint32_t rows, uint32_t columns, int32_t picSize, int32_t midx, int32_t midy, float rad, float angle, uint32_t *posTemp);

#endif /* __SERVICE_DEFORM_H__ */
