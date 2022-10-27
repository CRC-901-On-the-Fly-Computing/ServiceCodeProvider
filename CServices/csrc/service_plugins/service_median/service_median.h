/* service_median.h */

#ifndef __SERVICE_MEDIAN_H__
#define __SERVICE_MEDIAN_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <stdio.h>

int32_t run_service_median_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t filter);
int32_t run_service_median_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t filter);
int32_t run_service_median_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t filter);
int32_t run_service_median_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t filter);

// internal maxfile interface(s)
void run_service_median3_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t rows, uint32_t columns);
void run_service_median5_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t rows, uint32_t columns);
void run_service_median7_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t rows, uint32_t columns);

#endif /* __SERVICE_MEDIAN_H__ */
