/* service_negative_fpga.c */

#include "service_negative.h"

int32_t run_service_negative_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	run_service_negative_max((uint16_t *)pixpack_target, (uint16_t *)pixpack_source, columns * rows);
	return 0;
}
