/* service_morphopen.h */

#ifndef __SERVICE_MORPHOPEN_H__
#define __SERVICE_MORPHOPEN_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

int32_t run_service_morphopen_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_morphopen_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_morphopen_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);
int32_t run_service_morphopen_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns);

// internal maxfile interface(s)
void *run_service_morphopen_1_max(uint16_t *dataIn, uint32_t rows, uint32_t columns, uint32_t alignedPicSize);
void run_service_morphopen_2_max(void *engine, uint16_t *dataOut, uint32_t rows, uint32_t columns, uint32_t alignedPicSize);

#endif /* __SERVICE_MORPHOPEN_H__ */
