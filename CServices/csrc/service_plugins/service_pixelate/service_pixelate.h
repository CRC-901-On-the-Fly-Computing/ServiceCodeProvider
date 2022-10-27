/* service_pixelate.h */

#ifndef __SERVICE_PIXELATE_H__
#define __SERVICE_PIXELATE_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <stdio.h>

int32_t run_service_pixelate_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t area);
int32_t run_service_pixelate_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t area);
int32_t run_service_pixelate_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t area);
int32_t run_service_pixelate_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t area);

// internal maxfile interface(s)
void run_service_pixelate_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t *validPos, uint32_t rows, uint32_t columns, uint32_t area);

#endif /* __SERVICE_PIXELATE_H__ */
