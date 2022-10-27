/* service_gaussblur.h */

#ifndef __SERVICE_GAUSSBLUR_H__
#define __SERVICE_GAUSSBLUR_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

int32_t run_service_gaussblur_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t size_filter);
int32_t run_service_gaussblur_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t size_filter);
int32_t run_service_gaussblur_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t size_filter);

#endif /* __SERVICE_GAUSSBLUR_H__ */
