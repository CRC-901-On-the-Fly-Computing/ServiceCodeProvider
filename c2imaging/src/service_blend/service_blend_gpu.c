/* service_blend_gpu.c */

#include "service_blend.h"

int32_t run_service_blend_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source1, PixelPacket *pixpack_source2, int32_t rows1, int32_t columns1, int32_t rows2, int32_t columns2, float dirX, float dirY, float x, float y, int32_t mode) {
	
	float length,n_x,n_y,lineValue,doubleN;
	
	if(mode < 0 || mode > 19){
		fprintf(stderr, "mode must be between 0 and 19.");
		return -1;			
	}	

	if(dirX == 0 && dirY == 0){ // error: direction vector cannot be null
		return 1;
	}
	else{
		length = sqrtf(dirX * dirX + dirY * dirY);
		n_x = dirX / length;
		n_y = dirY / length;
		lineValue = n_x * x + n_y * y;
		doubleN = n_x * n_x + n_y * n_y;
	}

	
	#pragma acc kernels copyin(pixpack_source1[0:columns1*rows1]) copyin(pixpack_source2[0:columns2*rows2]) copyout(pixpack_target[0:columns1*rows1])
	{
		int32_t i, j, pos_ij, pos_t_ij;
		if(mode < 10){
			if(mode == 0) // simple blend - linear
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float distance = -(lineValue - n_x * (float)j - n_y * (float)i) / doubleN;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < 0 || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							uint16_t blendRed = pixpack_source2[pos_t_ij].red;
							uint16_t blendGreen = pixpack_source2[pos_t_ij].green;
							uint16_t blendBlue = pixpack_source2[pos_t_ij].blue;
							if (distance < length) {
								float relation = distance / length;
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * blendBlue);
							} else {
								pixpack_target[pos_ij].red	= blendRed;
								pixpack_target[pos_ij].green	= blendGreen;
								pixpack_target[pos_ij].blue	= blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 1) // lighten blend - linear
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float distance = -(lineValue - n_x * (float)j - n_y * (float)i) / doubleN;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < 0 || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							uint16_t blendRed = pixpack_source2[pos_t_ij].red;
							if(pixpack_source2[pos_t_ij].red < pixpack_source1[pos_ij].red)
								blendRed = pixpack_source1[pos_ij].red;

							uint16_t blendGreen = pixpack_source2[pos_t_ij].green;
							if(pixpack_source2[pos_t_ij].green < pixpack_source1[pos_ij].green)
								blendGreen = pixpack_source1[pos_ij].green;

							uint16_t blendBlue = pixpack_source2[pos_t_ij].blue;
							if(pixpack_source2[pos_t_ij].blue < pixpack_source1[pos_ij].blue)
								blendBlue = pixpack_source1[pos_ij].blue;
							if (distance < length) {
								float relation = distance / length;
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * blendBlue);
							} else {
								pixpack_target[pos_ij].red	= blendRed;
								pixpack_target[pos_ij].green	= blendGreen;
								pixpack_target[pos_ij].blue	= blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 2) // darken blend - linear
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float distance = -(lineValue - n_x * (float)j - n_y * (float)i) / doubleN;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < 0 || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							uint16_t blendRed = pixpack_source2[pos_t_ij].red;
							if(blendRed > pixpack_source1[pos_ij].red)
								blendRed = pixpack_source1[pos_ij].red;

							uint16_t blendGreen = pixpack_source2[pos_t_ij].green;
							if(blendGreen > pixpack_source1[pos_ij].green)
								blendGreen = pixpack_source1[pos_ij].green;

							uint16_t blendBlue = pixpack_source1[pos_ij].blue;
							if(blendBlue > pixpack_source1[pos_ij].blue)
								blendBlue = pixpack_source1[pos_ij].blue;
							if (distance < length) {
								float relation = distance / length;
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * blendBlue);
							} else {
								pixpack_target[pos_ij].red	= blendRed;
								pixpack_target[pos_ij].green	= blendGreen;
								pixpack_target[pos_ij].blue	= blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 3) // multiply blend - linear
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float distance = -(lineValue - n_x * (float)j - n_y * (float)i) / doubleN;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < 0 || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							uint16_t blendRed = (uint32_t)pixpack_source2[pos_t_ij].red * (uint32_t)pixpack_source1[pos_ij].red / (uint32_t) 65535;
							uint16_t blendGreen = (uint32_t)pixpack_source2[pos_t_ij].green * (uint32_t)pixpack_source1[pos_ij].green / (uint32_t) 65535;
							uint16_t blendBlue = (uint32_t)pixpack_source2[pos_t_ij].blue * (uint32_t)pixpack_source1[pos_ij].blue / (uint32_t) 65535;
							if (distance < length) {
								float relation = distance / length;
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * blendBlue);
							} else {
								pixpack_target[pos_ij].red	= blendRed;
								pixpack_target[pos_ij].green	= blendGreen;
								pixpack_target[pos_ij].blue	= blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 4) // white blend - linear
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float distance = -(lineValue - n_x * (float)j - n_y * (float)i) / doubleN;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < 0 || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							uint16_t blendRed = 65535;
							uint16_t blendGreen = 65535;
							uint16_t blendBlue = 65535;
							if (distance < length) {
								float relation = distance / length;
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * blendBlue);
							} else {
								pixpack_target[pos_ij].red	= blendRed;
								pixpack_target[pos_ij].green	= blendGreen;
								pixpack_target[pos_ij].blue	= blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 5) // add blend - linear
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float distance = -(lineValue - n_x * (float)j - n_y * (float)i) / doubleN;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < 0 || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							uint32_t blendRed = (((uint32_t)pixpack_source1[pos_ij].red + (uint32_t)pixpack_source2[pos_t_ij].red));
							uint32_t blendGreen = (((uint32_t)pixpack_source1[pos_ij].green + (uint32_t)pixpack_source2[pos_t_ij].green));
							uint32_t blendBlue = (((uint32_t)pixpack_source1[pos_ij].blue + (uint32_t)pixpack_source2[pos_t_ij].blue));
							if(blendRed > 65535)
								blendRed = 65535;
							if(blendGreen > 65535)
								blendGreen = 65535;
							if(blendBlue > 65535)
								blendBlue = 65535;
							if (distance < length) {
								float relation = distance / length;
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * (uint32_t)blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * (uint32_t)blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * (uint32_t)blendBlue);
							} else {
								pixpack_target[pos_ij].red	= (uint32_t)blendRed;
								pixpack_target[pos_ij].green	= (uint32_t)blendGreen;
								pixpack_target[pos_ij].blue	= (uint32_t)blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 6) // subtract blend - linear
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float distance = -(lineValue - n_x * (float)j - n_y * (float)i) / doubleN;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < 0 || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							int32_t blendRed = (((int32_t)pixpack_source1[pos_ij].red + (uint32_t)pixpack_source2[pos_t_ij].red));
							int32_t blendGreen = (((int32_t)pixpack_source1[pos_ij].green + (uint32_t)pixpack_source2[pos_t_ij].green));
							int32_t blendBlue = (((int32_t)pixpack_source1[pos_ij].blue + (uint32_t)pixpack_source2[pos_t_ij].blue));
							if(blendRed > 65535)
								blendRed = blendRed - 65535;
							else
								blendRed = 0;
							if(blendGreen > 65535)
								blendGreen = blendGreen - 65535;
							else
								blendGreen = 0;
							if(blendBlue > 65535)
								blendBlue = blendBlue - 65535;
							else
								blendBlue = 0;
							if (distance < length) {
								float relation = distance / length;
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * (uint16_t)blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * (uint16_t)blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * (uint16_t)blendBlue);
							} else {
								pixpack_target[pos_ij].red	= (uint16_t)blendRed;
								pixpack_target[pos_ij].green	= (uint16_t)blendGreen;
								pixpack_target[pos_ij].blue	= (uint16_t)blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 7) // overlay blend - linear
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float distance = -(lineValue - n_x * (float)j - n_y * (float)i) / doubleN;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < 0 || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							int32_t blendRed = (int32_t)pixpack_source2[pos_t_ij].red;
							int32_t blendGreen = (int32_t)pixpack_source2[pos_t_ij].green;
							int32_t blendBlue = (int32_t)pixpack_source2[pos_t_ij].blue;
							if(blendRed < 32768)
								blendRed = 2 * pixpack_source1[pos_ij].red * blendRed / 65535;
							else
								blendRed = 65535 - 2 * (65535 - pixpack_source1[pos_ij].red) * (65535 - blendRed) / 65535;
							if(blendGreen < 32768)
								blendGreen = 2 * pixpack_source1[pos_ij].green * blendGreen / 65535;
							else
								blendGreen = 65535 - 2 * (65535 - pixpack_source1[pos_ij].green) * (65535 - blendGreen) / 65535;
							if(blendBlue < 32768)
								blendBlue = 2 * pixpack_source1[pos_ij].blue * blendBlue / 65535;
							else
								blendBlue = 65535 - 2 * (65535 - pixpack_source1[pos_ij].blue) * (65535 - blendBlue) / 65535;
							if (distance < length) {
								float relation = distance / length;
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * (uint16_t)blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * (uint16_t)blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * (uint16_t)blendBlue);
							} else {
								pixpack_target[pos_ij].red	= (uint16_t)blendRed;
								pixpack_target[pos_ij].green	= (uint16_t)blendGreen;
								pixpack_target[pos_ij].blue	= (uint16_t)blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 8) // exclusion blend - linear
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float distance = -(lineValue - n_x * (float)j - n_y * (float)i) / doubleN;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < 0 || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							int32_t blendRed = ((int32_t)pixpack_source1[pos_ij].red + (int32_t)pixpack_source2[pos_t_ij].red - 2 * (int32_t)pixpack_source1[pos_ij].red * (int32_t)pixpack_source2[pos_t_ij].red / 65535);
							int32_t blendGreen = ((int32_t)pixpack_source1[pos_ij].green + (int32_t)pixpack_source2[pos_t_ij].green - 2 * (int32_t)pixpack_source1[pos_ij].green * (int32_t)pixpack_source2[pos_t_ij].green / 65535);
							int32_t blendBlue = ((int32_t)pixpack_source1[pos_ij].blue + (int32_t)pixpack_source2[pos_t_ij].blue - 2 * (int32_t)pixpack_source1[pos_ij].blue * (int32_t)pixpack_source2[pos_t_ij].blue / 65535);
							if (distance < length) {
								float relation = distance / length;
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * (uint16_t)blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * (uint16_t)blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * (uint16_t)blendBlue);
							} else {
								pixpack_target[pos_ij].red	= (uint16_t)blendRed;
								pixpack_target[pos_ij].green	= (uint16_t)blendGreen;
								pixpack_target[pos_ij].blue	= (uint16_t)blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 9) // reflect blend - linear
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float distance = -(lineValue - n_x * (float)j - n_y * (float)i) / doubleN;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < 0 || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							int32_t blendRed = (int32_t)pixpack_source2[pos_t_ij].red;
							int32_t blendGreen = (int32_t)pixpack_source2[pos_t_ij].green;
							int32_t blendBlue = (int32_t)pixpack_source2[pos_t_ij].blue;
							if (blendRed != 65535) {
							blendRed = (int32_t)((float)pixpack_source1[pos_ij].red * pixpack_source1[pos_ij].red / (65535 - blendRed));
								if (blendRed > 65535)
									blendRed = 65535;
							}
							if (blendBlue != 65535) {
								blendBlue = (int32_t)((float)pixpack_source1[pos_ij].blue * pixpack_source1[pos_ij].blue / (65535 - blendBlue));
								if (blendBlue > 65535)
									blendBlue = 65535;
							}	
							if (blendGreen != 65535) {
								blendGreen = (int32_t)((float)pixpack_source1[pos_ij].green * pixpack_source1[pos_ij].green / (65535 - blendGreen));
								if (blendGreen > 65535)
									blendGreen = 65535;
							}
							if (distance < length) {
								float relation = distance / length;
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * (uint16_t)blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * (uint16_t)blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * (uint16_t)blendBlue);
							} else {
								pixpack_target[pos_ij].red	= (uint16_t)blendRed;
								pixpack_target[pos_ij].green	= (uint16_t)blendGreen;
								pixpack_target[pos_ij].blue	= (uint16_t)blendBlue;
							}
						}
					}
				}
			}
		}
		else{
			float inner = dirX;
			float outer = dirY;
			if(inner > outer){
				outer = inner;
				inner = dirY;
			}
			if(mode == 10) // simple blend - circular
			//#pragma omp parallel
			{
			
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float dif_x = (float) j - x;
						float dif_y = (float) i - y;
						float distance = sqrtf(dif_x * dif_x + dif_y * dif_y) ;
				
						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < inner || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else if (distance < outer) {
							float relation = (distance - inner) / (outer - inner);
							pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * pixpack_source2[pos_t_ij].red);
							pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * pixpack_source2[pos_t_ij].green);
							pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * pixpack_source2[pos_t_ij].blue);
						} else {
							pixpack_target[pos_ij].red	= pixpack_source2[pos_t_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source2[pos_t_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source2[pos_t_ij].blue;
						}
					}
				}
			}
			else if(mode == 11) // lighten blend - circular
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float dif_x = (float) j - x;
						float dif_y = (float) i - y;
						float distance = sqrtf(dif_x * dif_x + dif_y * dif_y) ;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < inner || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							uint16_t blendRed = pixpack_source2[pos_t_ij].red;
							if(pixpack_source2[pos_t_ij].red < pixpack_source1[pos_ij].red)
								blendRed = pixpack_source1[pos_ij].red;

							uint16_t blendGreen = pixpack_source2[pos_t_ij].green;
							if(pixpack_source2[pos_t_ij].green < pixpack_source1[pos_ij].green)
								blendGreen = pixpack_source1[pos_ij].green;

							uint16_t blendBlue = pixpack_source2[pos_t_ij].blue;
							if(pixpack_source2[pos_t_ij].blue < pixpack_source1[pos_ij].blue)
								blendBlue = pixpack_source1[pos_ij].blue;
							if (distance < outer) {
								float relation = (distance - inner) / (outer - inner);
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * blendBlue);
							} else {
								pixpack_target[pos_ij].red	= blendRed;
								pixpack_target[pos_ij].green	= blendGreen;
								pixpack_target[pos_ij].blue	= blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 12) // darken blend - circular
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float dif_x = (float) j - x;
						float dif_y = (float) i - y;
						float distance = sqrtf(dif_x * dif_x + dif_y * dif_y) ;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < inner || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							uint16_t blendRed = pixpack_source2[pos_t_ij].red;
							if(blendRed > pixpack_source1[pos_ij].red)
								blendRed = pixpack_source1[pos_ij].red;

							uint16_t blendGreen = pixpack_source2[pos_t_ij].green;
							if(blendGreen > pixpack_source1[pos_ij].green)
								blendGreen = pixpack_source1[pos_ij].green;

							uint16_t blendBlue = pixpack_source1[pos_ij].blue;
							if(blendBlue > pixpack_source1[pos_ij].blue)
								blendBlue = pixpack_source1[pos_ij].blue;
							if (distance < outer) {
								float relation = (distance - inner) / (outer - inner);
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * blendBlue);
							} else {
								pixpack_target[pos_ij].red	= blendRed;
								pixpack_target[pos_ij].green	= blendGreen;
								pixpack_target[pos_ij].blue	= blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 13) // multiply blend - circular
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float dif_x = (float) j - x;
						float dif_y = (float) i - y;
						float distance = sqrtf(dif_x * dif_x + dif_y * dif_y) ;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < inner || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							uint16_t blendRed = (uint32_t)pixpack_source2[pos_t_ij].red * (uint32_t)pixpack_source1[pos_ij].red / (uint32_t) 65535;
							uint16_t blendGreen = (uint32_t)pixpack_source2[pos_t_ij].green * (uint32_t)pixpack_source1[pos_ij].green / (uint32_t) 65535;
							uint16_t blendBlue = (uint32_t)pixpack_source2[pos_t_ij].blue * (uint32_t)pixpack_source1[pos_ij].blue / (uint32_t) 65535;
							if (distance < outer) {
								float relation = (distance - inner) / (outer - inner);
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * blendBlue);
							} else {
								pixpack_target[pos_ij].red	= blendRed;
								pixpack_target[pos_ij].green	= blendGreen;
								pixpack_target[pos_ij].blue	= blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 14) // white blend - circular
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float dif_x = (float) j - x;
						float dif_y = (float) i - y;
						float distance = sqrtf(dif_x * dif_x + dif_y * dif_y) ;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < inner || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							uint16_t blendRed = 65535;
							uint16_t blendGreen = 65535;
							uint16_t blendBlue = 65535;
							if (distance < outer) {
								float relation = (distance - inner) / (outer - inner);
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * blendBlue);
							} else {
								pixpack_target[pos_ij].red	= blendRed;
								pixpack_target[pos_ij].green	= blendGreen;
								pixpack_target[pos_ij].blue	= blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 15) // add blend - circular
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float dif_x = (float) j - x;
						float dif_y = (float) i - y;
						float distance = sqrtf(dif_x * dif_x + dif_y * dif_y) ;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < inner || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							uint32_t blendRed = (((uint32_t)pixpack_source1[pos_ij].red + (uint32_t)pixpack_source2[pos_t_ij].red));
							uint32_t blendGreen = (((uint32_t)pixpack_source1[pos_ij].green + (uint32_t)pixpack_source2[pos_t_ij].green));
							uint32_t blendBlue = (((uint32_t)pixpack_source1[pos_ij].blue + (uint32_t)pixpack_source2[pos_t_ij].blue));
							if(blendRed > 65535)
								blendRed = 65535;
							if(blendGreen > 65535)
								blendGreen = 65535;
							if(blendBlue > 65535)
								blendBlue = 65535;
							if (distance < outer) {
								float relation = (distance - inner) / (outer - inner);
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * (uint32_t)blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * (uint32_t)blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * (uint32_t)blendBlue);
							} else {
								pixpack_target[pos_ij].red	= (uint32_t)blendRed;
								pixpack_target[pos_ij].green	= (uint32_t)blendGreen;
								pixpack_target[pos_ij].blue	= (uint32_t)blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 16) // subtract blend - circular
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float dif_x = (float) j - x;
						float dif_y = (float) i - y;
						float distance = sqrtf(dif_x * dif_x + dif_y * dif_y) ;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < inner || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							int32_t blendRed = (((int32_t)pixpack_source1[pos_ij].red + (uint32_t)pixpack_source2[pos_t_ij].red));
							int32_t blendGreen = (((int32_t)pixpack_source1[pos_ij].green + (uint32_t)pixpack_source2[pos_t_ij].green));
							int32_t blendBlue = (((int32_t)pixpack_source1[pos_ij].blue + (uint32_t)pixpack_source2[pos_t_ij].blue));
							if(blendRed > 65535)
								blendRed = blendRed - 65535;
							else
								blendRed = 0;
							if(blendGreen > 65535)
								blendGreen = blendGreen - 65535;
							else
								blendGreen = 0;
							if(blendBlue > 65535)
								blendBlue = blendBlue - 65535;
							else
								blendBlue = 0;
							if (distance < outer) {
								float relation = (distance - inner) / (outer - inner);
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * (uint16_t)blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * (uint16_t)blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * (uint16_t)blendBlue);
							} else {
								pixpack_target[pos_ij].red	= (uint16_t)blendRed;
								pixpack_target[pos_ij].green	= (uint16_t)blendGreen;
								pixpack_target[pos_ij].blue	= (uint16_t)blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 17) // overlay blend - circular
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float dif_x = (float) j - x;
						float dif_y = (float) i - y;
						float distance = sqrtf(dif_x * dif_x + dif_y * dif_y) ;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < inner || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							int32_t blendRed = (int32_t)pixpack_source2[pos_t_ij].red;
							int32_t blendGreen = (int32_t)pixpack_source2[pos_t_ij].green;
							int32_t blendBlue = (int32_t)pixpack_source2[pos_t_ij].blue;
							if(blendRed < 32768)
								blendRed = 2 * pixpack_source1[pos_ij].red * blendRed / 65535;
							else
								blendRed = 65535 - 2 * (65535 - pixpack_source1[pos_ij].red) * (65535 - blendRed) / 65535;
							if(blendGreen < 32768)
								blendGreen = 2 * pixpack_source1[pos_ij].green * blendGreen / 65535;
							else
								blendGreen = 65535 - 2 * (65535 - pixpack_source1[pos_ij].green) * (65535 - blendGreen) / 65535;
							if(blendBlue < 32768)
								blendBlue = 2 * pixpack_source1[pos_ij].blue * blendBlue / 65535;
							else
								blendBlue = 65535 - 2 * (65535 - pixpack_source1[pos_ij].blue) * (65535 - blendBlue) / 65535;
							if (distance < outer) {
								float relation = (distance - inner) / (outer - inner);
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * (uint16_t)blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * (uint16_t)blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * (uint16_t)blendBlue);
							} else {
								pixpack_target[pos_ij].red	= (uint16_t)blendRed;
								pixpack_target[pos_ij].green	= (uint16_t)blendGreen;
								pixpack_target[pos_ij].blue	= (uint16_t)blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 18) // exclusion blend - circular
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float dif_x = (float) j - x;
						float dif_y = (float) i - y;
						float distance = sqrtf(dif_x * dif_x + dif_y * dif_y) ;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < inner || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							int32_t blendRed = ((int32_t)pixpack_source1[pos_ij].red + (int32_t)pixpack_source2[pos_t_ij].red - 2 * (int32_t)pixpack_source1[pos_ij].red * (int32_t)pixpack_source2[pos_t_ij].red / 65535);
							int32_t blendGreen = ((int32_t)pixpack_source1[pos_ij].green + (int32_t)pixpack_source2[pos_t_ij].green - 2 * (int32_t)pixpack_source1[pos_ij].green * (int32_t)pixpack_source2[pos_t_ij].green / 65535);
							int32_t blendBlue = ((int32_t)pixpack_source1[pos_ij].blue + (int32_t)pixpack_source2[pos_t_ij].blue - 2 * (int32_t)pixpack_source1[pos_ij].blue * (int32_t)pixpack_source2[pos_t_ij].blue / 65535);
							if (distance < outer) {
								float relation = (distance - inner) / (outer - inner);
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * (uint16_t)blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * (uint16_t)blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * (uint16_t)blendBlue);
							} else {
								pixpack_target[pos_ij].red	= (uint16_t)blendRed;
								pixpack_target[pos_ij].green	= (uint16_t)blendGreen;
								pixpack_target[pos_ij].blue	= (uint16_t)blendBlue;
							}
						}
					}
				}
			}
			else if(mode == 19) // reflect blend - circular
			{
				#pragma acc loop independent
				for (i=0; i<rows1; ++i) {
					#pragma acc loop independent private(pos_ij) private(pos_t_ij)
					for (j=0; j<columns1; ++j) {
						float dif_x = (float) j - x;
						float dif_y = (float) i - y;
						float distance = sqrtf(dif_x * dif_x + dif_y * dif_y) ;

						pos_ij = i*columns1+j;
						pos_t_ij = i*columns2+j;
						if (distance < inner || i >= rows2 || j >= columns2) {
							pixpack_target[pos_ij].red	= pixpack_source1[pos_ij].red;
							pixpack_target[pos_ij].green	= pixpack_source1[pos_ij].green;
							pixpack_target[pos_ij].blue	= pixpack_source1[pos_ij].blue;
						} else {
							int32_t blendRed = (int32_t)pixpack_source2[pos_t_ij].red;
							int32_t blendGreen = (int32_t)pixpack_source2[pos_t_ij].green;
							int32_t blendBlue = (int32_t)pixpack_source2[pos_t_ij].blue;
							if (blendRed != 65535) {
							blendRed = (int32_t)((float)pixpack_source1[pos_ij].red * pixpack_source1[pos_ij].red / (65535 - blendRed));
							if (blendRed > 65535)
								blendRed = 65535;
							}
							if (blendBlue != 65535) {
								blendBlue = (int32_t)((float)pixpack_source1[pos_ij].blue * pixpack_source1[pos_ij].blue / (65535 - blendBlue));
								if (blendBlue > 65535)
									blendBlue = 65535;
							}
							if (blendGreen != 65535) {
								blendGreen = (int32_t)((float)pixpack_source1[pos_ij].green * pixpack_source1[pos_ij].green / (65535 - blendGreen));
								if (blendGreen > 65535)
									blendGreen = 65535;
							}
							if (distance < outer) {
								float relation = (distance - inner) / (outer - inner);
								pixpack_target[pos_ij].red	= ((float)pixpack_source1[pos_ij].red * (1 - relation) + relation * (uint16_t)blendRed);
								pixpack_target[pos_ij].green	= ((float)pixpack_source1[pos_ij].green * (1 - relation) + relation * (uint16_t)blendGreen);
								pixpack_target[pos_ij].blue	= ((float)pixpack_source1[pos_ij].blue * (1 - relation) + relation * (uint16_t)blendBlue);
							} else {
								pixpack_target[pos_ij].red	= (uint16_t)blendRed;
								pixpack_target[pos_ij].green	= (uint16_t)blendGreen;
								pixpack_target[pos_ij].blue	= (uint16_t)blendBlue;
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
