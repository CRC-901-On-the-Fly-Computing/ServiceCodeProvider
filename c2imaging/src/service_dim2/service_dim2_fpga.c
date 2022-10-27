/* service_dim2_fpga.c */

#include "service_dim2.h"

int32_t run_service_dim2_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, float factor) {
	
	if (factor < 0) {
		fprintf(stderr, "Wrong parameter! Factor needs to be larger than 0\n");
		return -1;
	}
	
	run_service_dim2_max((uint16_t *)pixpack_target, (uint16_t *)pixpack_source, (uint32_t)(rows*columns), factor);
	
	return 0;
}
