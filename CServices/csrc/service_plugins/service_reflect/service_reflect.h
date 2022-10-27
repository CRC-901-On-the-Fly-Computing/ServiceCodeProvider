/* service_reflect.h */

#ifndef __SERVICE_REFLECT_H__
#define __SERVICE_REFLECT_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <math.h>

int32_t run_service_reflect_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float angle);
int32_t run_service_reflect_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float angle);
int32_t run_service_reflect_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float angle);
int32_t run_service_reflect_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float angle);

// internal maxfile interface(s)
void run_service_reflect_max(int32_t *dataOut, uint32_t rows, uint32_t columns, float sin, float cos);

#endif /* __SERVICE_REFLECT_H__ */
