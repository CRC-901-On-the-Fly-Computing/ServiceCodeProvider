/* service_grey_fpga.c */

#include "service_grey.h"

int32_t run_service_grey_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {

	run_service_grey_max((uint16_t *)pixpack_target, (uint16_t *)pixpack_source, (uint32_t)(rows*columns));
	return 0;
}
