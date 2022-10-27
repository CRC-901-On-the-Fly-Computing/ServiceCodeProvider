/* service_dim.h */

#ifndef __SERVICE_DIM_H__
#define __SERVICE_DIM_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

int32_t run_service_dim_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_dim_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_dim_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_dim_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);

// internal maxfile interface(s)
void run_service_dim_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t picSize);

#endif /* __SERVICE_DIM_H__ */
