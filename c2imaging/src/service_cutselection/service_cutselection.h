/* service_cutselection.h */

#ifndef __SERVICE_CUTSELECTION_H__
#define __SERVICE_CUTSELECTION_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

#include <stdio.h>

int32_t run_service_cutselection_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns,  int32_t p1_x, int32_t p1_y, int32_t p2_x, int32_t p2_y);
int32_t run_service_cutselection_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns,  int32_t p1_x, int32_t p1_y, int32_t p2_x, int32_t p2_y);
int32_t run_service_cutselection_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns,  int32_t p1_x, int32_t p1_y, int32_t p2_x, int32_t p2_y);
int32_t run_service_cutselection_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns,  int32_t p1_x, int32_t p1_y, int32_t p2_x, int32_t p2_y);

// internal maxfile interface(s)
void run_service_cutselection_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t rows, uint32_t columns, uint32_t alignedTargetPicSize, uint32_t p1_x, uint32_t p1_y, uint32_t p2_x, uint32_t p2_y);
	
#endif /* __SERVICE_CUTSELECTION_H__ */
