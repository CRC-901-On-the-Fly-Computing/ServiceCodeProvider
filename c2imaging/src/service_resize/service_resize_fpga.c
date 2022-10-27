/* service_resize_fpga.c */

#include "service_resize.h"

int32_t run_service_resize_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t resize_factor) {
	int32_t new_rows	= rows/resize_factor;
	int32_t new_columns	= columns/resize_factor;
	
	// MaxJ project has a static resize_factor of 2!
	run_service_resize_max((uint16_t *)pixpack_target, (uint16_t *)pixpack_source, (uint32_t)rows, (uint32_t)columns);
	
	return 0;
}
