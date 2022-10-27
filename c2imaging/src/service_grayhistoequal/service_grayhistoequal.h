/* service_grayhistoequal.h */

#ifndef __SERVICE_GRAYHISTOEQUAL_H__
#define __SERVICE_GRAYHISTOEQUAL_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

int32_t run_service_grayhistoequal_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_grayhistoequal_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_grayhistoequal_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_grayhistoequal_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);

// internal maxfile interface(s)
void run_service_grayhistoequal_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t picSize, uint16_t min, uint16_t max);

#endif /* __SERVICE_GRAYHISTOEQUAL_H__ */
