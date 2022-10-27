/* service_rgbchannelmixer_fpga.c */

#include "service_rgbchannelmixer.h"

int32_t run_service_rgbchannelmixer_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t channel, float r_percent, float g_percent, float b_percent) {

	run_service_rgbchannelmixer_max((uint16_t *)pixpack_target, (uint16_t *)pixpack_source, rows * columns, channel, r_percent, g_percent, b_percent);

	return 0;
}
