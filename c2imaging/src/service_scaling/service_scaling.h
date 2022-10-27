/* service_scaling.h */

#ifndef __SERVICE_SCALING_H__
#define __SERVICE_SCALING_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <stdio.h>

int32_t run_service_scaling_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t scale_up_factor, int32_t scale_down_factor);
int32_t run_service_scaling_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t scale_up_factor, int32_t scale_down_factor);
int32_t run_service_scaling_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t scale_up_factor, int32_t scale_down_factor);

#endif /* __SERVICE_SCALING_H__ */
