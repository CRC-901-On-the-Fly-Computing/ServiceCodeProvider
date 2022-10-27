/* service_maxrgb_fpga.c */

#include "service_maxrgb.h"

int32_t run_service_maxrgb_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t mode){
	if (mode<1 || mode > 4) { // Wrong Parameter;
		fprintf(stderr, " Parameter hat to be between 1 and 4");
		return -1;
	}
	run_service_max_maxrgb((uint16_t *)pixpack_target, (uint16_t *) pixpack_source, columns * rows, mode);
	
	return 0;
}
