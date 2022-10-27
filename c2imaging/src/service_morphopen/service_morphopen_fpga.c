/* service_morphopen_fpga.c */

#include "service_morphopen.h"

int32_t run_service_morphopen_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns) {
	uint32_t burst_size_in_bytes	= 384;
	uint32_t burst_size				= 384/sizeof(PixelPacket);
	uint32_t pic_size				= columns * rows;
	uint32_t aligned_pic_size		= ((columns * rows)/burst_size + 1)*burst_size;
	
	PixelPacket *pixpack_temp		= malloc(aligned_pic_size * sizeof(PixelPacket));
	if (NULL == pixpack_temp) {
		fprintf(stderr, "Couldn't allocate memory. (file: %s, line: %i)\n", __FILE__, __LINE__);
		return -1;
	}
	
	memcpy(pixpack_temp, pixpack_source, pic_size * sizeof(PixelPacket));
	
	void *engine = NULL;
	
	// erosion
	engine = run_service_morphopen_1_max((uint16_t *)pixpack_temp, (uint32_t)rows, (uint32_t)columns, (uint32_t)aligned_pic_size);
	
	// dilatation
	run_service_morphopen_2_max(engine, (uint16_t *)pixpack_target, (uint32_t)rows, (uint32_t)columns, (uint32_t)aligned_pic_size);
	
	free(pixpack_temp);
	
	return 0;
}
