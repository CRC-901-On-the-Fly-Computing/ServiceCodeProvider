/* service_resize.h */

#ifndef __SERVICE_RESIZE_H__
#define __SERVICE_RESIZE_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

int32_t run_service_resize_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t resize_factor);
int32_t run_service_resize_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t resize_factor);
int32_t run_service_resize_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t resize_factor);
int32_t run_service_resize_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t resize_factor);

// internal maxfile interface(s)
void run_service_resize_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t rows, uint32_t columns);

#endif /* __SERVICE_RESIZE_H__ */
