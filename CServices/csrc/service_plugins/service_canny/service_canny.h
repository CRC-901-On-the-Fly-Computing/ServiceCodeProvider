/* service_canny.h */

#ifndef __SERVICE_CANNY_H__
#define __SERVICE_CANNY_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <math.h>
#include <stdio.h>

#define PI 3.14159265

int32_t run_service_canny_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float Thigh, float Tlow);
int32_t run_service_canny_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float Thigh, float Tlow);
int32_t run_service_canny_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float Thigh, float Tlow);

#endif /* __SERVICE_CANNY_H__ */
