/* service_maxrgb.h */

#ifndef __SERVICE_MAXRGB_H__
#define __SERVICE_MAXRGB_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <stdio.h>

int32_t run_service_maxrgb_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mode);
int32_t run_service_maxrgb_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mode);
int32_t run_service_maxrgb_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mode);
int32_t run_service_maxrgb_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mode);

// internal maxfile interface(s)
void run_service_max_maxrgb(uint16_t *dataOut, uint16_t *dataIn, uint32_t picSize, uint32_t mode);
	
#endif /* __SERVICE_MAXRGB_H__ */
