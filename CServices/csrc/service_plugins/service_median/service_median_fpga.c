/* service_median_fpga.c */

#include "service_median.h"

int32_t run_service_median_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t filter) {

	if (filter % 2 == 0) {
		fprintf(stderr, "Wrong filter size, filter size hast to be odd");
		return -1;
	}

	switch (filter) {
		case 3:
			run_service_median3_max((uint16_t *)pixpack_target, (uint16_t *) pixpack_source, rows, columns);
			break;
		case 5:
			run_service_median5_max((uint16_t *)pixpack_target, (uint16_t *) pixpack_source, rows, columns);
			break;
		case 7:
			run_service_median7_max((uint16_t *)pixpack_target, (uint16_t *) pixpack_source, rows, columns);
			break;
		default:
			fprintf(stderr, "Wrong filter size, filter size hast to be 3,5 or 7 on the FPGA");
			return -1;
			break;
	}

	return 0;
}
