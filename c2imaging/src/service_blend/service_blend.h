/* service_blend.h */

#ifndef __SERVICE_BLEND_H__
#define __SERVICE_BLEND_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>
#include <math.h>
#include <stdio.h>

int32_t run_service_blend_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source1, PixelPacket *pixpack_source2, int32_t rows1, int32_t columns1, int32_t rows2, int32_t columns2, float dirX, float dirY, float x, float y, int32_t mode);
int32_t run_service_blend_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source1, PixelPacket *pixpack_source2, int32_t rows1, int32_t columns1, int32_t rows2, int32_t columns2, float dirX, float dirY, float x, float y, int32_t mode);
int32_t run_service_blend_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source1, PixelPacket *pixpack_source2, int32_t rows1, int32_t columns1, int32_t rows2, int32_t columns2, float dirX, float dirY, float x, float y, int32_t mode);
int32_t run_service_blend_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source1, PixelPacket *pixpack_source2, int32_t rows1, int32_t columns1, int32_t rows2, int32_t columns2, float dirX, float dirY, float x, float y, int32_t mode);

// internal maxfile interface(s)
void run_service_blend_max(uint16_t *dataOut, uint16_t *dataIn1, uint16_t *dataIn2, uint32_t rows1, uint32_t columns1, uint32_t rows2, uint32_t columns2, float dirX, float dirY, float x, float y, uint8_t mode);

#endif /* __SERVICE_BLEND_H__ */
