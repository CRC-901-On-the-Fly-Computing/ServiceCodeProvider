/* service_morphedgedetection_overlay.c */

#include "service_morphedgedetection.h"

int32_t run_service_morphedgedetection_overlay(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t matrix_size) {
        run_service_morphedgedetection_fpga(pixpack_target, pixpack_source, rows, columns, matrix_size);
	
	return 0;

}
