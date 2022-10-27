/* service_cutselection_fpga.c */

#include "service_cutselection.h"

int32_t run_service_cutselection_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns,  int32_t p1_x, int32_t p1_y, int32_t p2_x, int32_t p2_y) {

	uint32_t targetPicSize = (p2_x - p1_x + 1) * (p2_y - p1_y + 1);
	uint32_t alignedTargetPicSize = (targetPicSize / 2 + 1) * 2;
	PixelPacket *pixpack_temp		= malloc(alignedTargetPicSize * sizeof(PixelPacket));
	if (NULL == pixpack_temp) {
		fprintf(stderr, "Couldn't allocate memory. (file: %s, line: %i)\n", __FILE__, __LINE__);
		return -1;
	}


	if (p1_x > p2_x || p1_y > p2_y || p1_x < 0 || p1_y < 0 || p2_x < 0 || p2_y < 0 || p2_x > columns || p1_x > columns || p2_y > rows || p1_y > rows) {
		// invalid parameters, because P1 is not the Point of the left corner of the selected area or P1 / P2 are not Points of the source
		fprintf(stderr, "invalid parameters, P1 is not the Point of the left corner of the selected area or P1 / P2 are not Points of the source");
		return -1;
	}

	if ((targetPicSize*sizeof(PixelPacket)) % 16 != 0) {
		run_service_cutselection_max((uint16_t *)pixpack_temp, (uint16_t *)pixpack_source, rows, columns, alignedTargetPicSize, p1_x, p1_y, p2_x, p2_y);
		memcpy(pixpack_target, pixpack_temp, (p2_x - p1_x + 1) * (p2_y - p1_y + 1)*sizeof(PixelPacket));
	} else {
		run_service_cutselection_max((uint16_t *)pixpack_target, (uint16_t *)pixpack_source, rows, columns, targetPicSize, p1_x, p1_y, p2_x, p2_y);
	}
	return 0;
}

