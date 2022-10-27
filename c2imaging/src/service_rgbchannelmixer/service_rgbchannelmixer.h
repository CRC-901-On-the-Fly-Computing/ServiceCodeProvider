/* service_rgbchannelmixer. */

#ifndef __SERVICE_RGBCHANNELMIXER_H__
#define __SERVICE_RGBCHANNELMIXER_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <math.h>

int32_t run_service_rgbchannelmixer_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t channel, float r_percent, float g_percent, float b_percent);
int32_t run_service_rgbchannelmixer_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t channel, float r_percent, float g_percent, float b_percent);
int32_t run_service_rgbchannelmixer_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t channel, float r_percent, float g_percent, float b_percent);
int32_t run_service_rgbchannelmixer_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t channel, float r_percent, float g_percent, float b_percent);

// internal maxfile interface(s)
void run_service_rgbchannelmixer_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t picSize, uint32_t channel, float r_percent, float g_percent, float b_percent);

#endif /* __SERVICE_RGBCHANNELMIXER_H__ */
