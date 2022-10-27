/* service_rotate.h */

#ifndef __SERVICE_ROTATE_H__
#define __SERVICE_ROTATE_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

int32_t run_service_rotate_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float degree);
int32_t run_service_rotate_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float degree);
int32_t run_service_rotate_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float degree);
int32_t run_service_rotate_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float degree);

// internal maxfile interface(s)
void run_service_rotate_max(int32_t *dataOut, uint32_t rows, uint32_t columns, float sin, float cos);

#endif /* __SERVICE_ROTATE_H__ */
