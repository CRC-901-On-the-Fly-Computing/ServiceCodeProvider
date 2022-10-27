/* service_morphedgedetection_fpga.c */

#include "service_morphedgedetection.h"

int32_t run_service_morphedgedetection_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t matrix_size) {
	
	if (matrix_size > 42 || matrix_size < 1) {
		fprintf(stderr, "ERROR: matrix_size, has to be at least 1 and maximum 42!\n");
		return -1;
	}
	
	if (columns > 6000) {
		fprintf(stderr, "ERROR: Image is too large for the FPGA Version!\n");
		return -1;
	}
	
	run_service_morphedgedetection_max((uint16_t *)pixpack_target, (uint16_t *) pixpack_source, rows, columns, matrix_size);
	
	return 0;

}
