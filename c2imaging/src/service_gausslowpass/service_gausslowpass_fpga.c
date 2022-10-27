/* service_gausslowpass_fpga.c */

#include "service_gausslowpass.h"

int32_t run_service_gausslowpass_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	run_service_gausslowpass_max((uint16_t *)pixpack_target, (uint16_t *)pixpack_source, (uint32_t)rows, (uint32_t)columns);
	
	return 0;
}
