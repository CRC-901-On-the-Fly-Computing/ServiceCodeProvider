/* service_gausslowpass2.h */

#ifndef __SERVICE_GAUSSLOWPASS2_H__
#define __SERVICE_GAUSSLOWPASS2_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>
#include <stdio.h>

int32_t run_service_gausslowpass2_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mask_size);
int32_t run_service_gausslowpass2_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mask_size);
int32_t run_service_gausslowpass2_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mask_size);
int32_t run_service_gausslowpass2_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mask_size);

// internal maxfile interface(s)
void run_service_gausslowpass2_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t *convMatr, uint32_t rows, uint32_t columns, uint32_t sum);

#endif /* __SERVICE_GAUSSLOWPASS2_H__ */
