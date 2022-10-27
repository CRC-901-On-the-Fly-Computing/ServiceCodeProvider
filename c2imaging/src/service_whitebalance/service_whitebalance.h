/* service_whitebalance.h */

#ifndef __SERVICE_WHITEBALANCE_H__
#define __SERVICE_WHITEBALANCE_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <stdio.h>
#include <math.h>
#include <string.h>

int32_t run_service_whitebalance_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t p);
int32_t run_service_whitebalance_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t p);
int32_t run_service_whitebalance_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t p);
int32_t run_service_whitebalance_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t p);

// internal maxfile interface(s)
void *run_service_whitebalance_max_k1(uint16_t *dataIn, uint32_t rows, uint32_t columns, uint32_t alignedPicSize, float p, float* Rm, float* Gm, float* Bm);
void run_service_whitebalance_max_k2(void *engine, uint16_t *dataOut, uint32_t rows, uint32_t columns, uint32_t alignedPicSize, float Rm, float Gm, float Bm);
	
#endif /* __SERVICE_MAXRGB_H__ */
