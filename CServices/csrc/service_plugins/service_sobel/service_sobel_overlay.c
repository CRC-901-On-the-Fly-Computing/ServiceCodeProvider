/* service_sobel_overlay.c */

#include "service_sobel.h"

int32_t run_service_sobel_overlay(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	run_service_sobel_fpga(pixpack_target, pixpack_source, rows, columns);
	
	return 0;
}
