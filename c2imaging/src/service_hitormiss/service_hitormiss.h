/* service_hitormiss.h */

#ifndef __SERVICE_HITORMISS_H__
#define __SERVICE_HITORMISS_H__

#include "../include/plugin.h"
#include "../include/common.h"
#include <dlfcn.h>

int32_t run_service_hitormiss_cpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source0, PixelPacket *pixpack_source1, PixelPacket *pixpack_source2, int32_t rows0, int32_t columns0,  int32_t rows1, int32_t columns1,  int32_t rows2, int32_t columns2, int32_t threshold);
int32_t run_service_hitormiss_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source0, PixelPacket *pixpack_source1, PixelPacket *pixpack_source2, int32_t rows0, int32_t columns0,  int32_t rows1, int32_t columns1,  int32_t rows2, int32_t columns2, int32_t threshold);
int32_t run_service_hitormiss_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source0, PixelPacket *pixpack_source1, PixelPacket *pixpack_source2, int32_t rows0, int32_t columns0,  int32_t rows1, int32_t columns1,  int32_t rows2, int32_t columns2, int32_t threshold);
int32_t run_service_hitormiss_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source0, PixelPacket *pixpack_source1, PixelPacket *pixpack_source2, int32_t rows0, int32_t columns0,  int32_t rows1, int32_t columns1,  int32_t rows2, int32_t columns2, int32_t threshold);

// internal maxfile interface(s)
void run_service_hitormiss_max(uint16_t *dataOut, uint16_t *dataIn, uint32_t rows, uint32_t columns, uint32_t threshold, int32_t *offsetarray_hit, int32_t *offsetarray_miss, uint32_t offsetarraysize_hit, uint32_t offsetarraysize_miss);

#endif /* __SERVICE_HITORMISS_H__ */
