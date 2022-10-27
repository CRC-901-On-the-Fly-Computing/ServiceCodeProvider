/* service_contrast_fpga.c */

#include "service_contrast.h"

int32_t run_service_contrast_fpga(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t factor) {
	int32_t i, j, pos;
	uint16_t *minValout=malloc(sizeof(uint16_t));
	uint16_t *maxValout=malloc(sizeof(uint16_t));
	uint16_t minVal;
	uint16_t maxVal;
	float x;
	uint32_t burst_size_in_bytes	= 384;
	uint32_t burst_size				= 384/sizeof(PixelPacket);
	uint32_t pic_size				= columns * rows;
	uint32_t alignedPicSize		= ((columns * rows)/burst_size + 1)*burst_size;
	
	PixelPacket *pixpack_temp		= malloc(alignedPicSize * sizeof(PixelPacket));
	if (NULL == pixpack_temp) {
		fprintf(stderr, "Couldn't allocate memory. (file: %s, line: %i)\n", __FILE__, __LINE__);
		return -1;
	}
	
	memcpy(pixpack_temp, pixpack_source, pic_size * sizeof(PixelPacket));
	
	void *engine = NULL;
	
	if( (factor < -128) || (factor > 127) ){ //factor out of range
		fprintf(stderr, "factor out of range.");
		return 1;			
	}						
	
	if( factor == -128 ) {
		//auto-contrast, no brightness-change
		engine = run_service_contrast_max_k1((uint16_t *) pixpack_temp, rows, columns, alignedPicSize, minValout, maxValout);
		minVal=*minValout;
		maxVal=*maxValout;
	}
	else{
		if( factor > 0 )
			x = (float) factor*factor / 127;
		else
			x = ((float) factor) / 127;
	}
	
	if( ( factor == 128 ) || ( factor == -128 ) ) {
		//auto-contrast: calc x-value
		
		int32_t absBorder;
		if( minVal < (65535-maxVal) )
			absBorder = minVal;
		else
			absBorder = 65535 - maxVal;

		x = -(float)absBorder / (float)(absBorder-32767);

		run_service_contrast_max_k2(engine, (uint16_t *) pixpack_target, rows, columns, alignedPicSize, x);
	}
	else{
		run_service_contrast_max_k3((uint16_t *) pixpack_target, (uint16_t *) pixpack_source, columns, rows, x);
	}

	free(maxValout);
	free(minValout);
	free(pixpack_temp);
	
	return 0;
}
