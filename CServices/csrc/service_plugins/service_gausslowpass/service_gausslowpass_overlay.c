/* service_gausslowpass_overlay.c */

#include "service_gausslowpass.h"

int32_t run_service_gausslowpass_overlay(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	run_service_gausslowpass_fpga(pixpack_target, pixpack_source, rows, columns);
	
	return 0;
}
