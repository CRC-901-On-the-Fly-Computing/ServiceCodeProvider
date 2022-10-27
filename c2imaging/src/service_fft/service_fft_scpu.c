/* service_fft_scpu.c */

#include "service_fft.h"

double PI;
typedef float complex cpl;

//Algorithm after Cooley-Tukey FFT
void fft_ini(cpl buf[], cpl out[], int n, int step) {
	int32_t i = 0;
	//Recursiv FFT only 2 to n pixel are allowed in rows and columns
	if (step < n) {
		//radix-2 Decimation in time FFT recursiv O(nlog(n))
		fft_ini(out, buf, n, step * 2);	//even index
		fft_ini(out + step, buf + step, n, step * 2);	//odd index
			for (i = 0; i < n; i += 2 * step) {
				//Here is the fft
				cpl t = cexp(-I * PI * i / n) * out[i + step];
				//The periodicity property of the fft works well here
				buf[i / 2]     = out[i] + t; //First N/2-1 Values
				buf[(i + n) / 2] = out[i] - t; //Second N/2-1 Values
			}
	}
}

void fft(cpl buf[], int n) {
	int32_t i = 0;
	cpl *out = malloc(n * sizeof(cpl));
	//Temporary Buffer for the transformed row/column
		for (i = 0; i < n; i++) out[i] = buf[i];
	fft_ini(buf, out, n, 1);
	free(out);
}


int32_t run_service_fft_scpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t scale_factor) {
	int32_t i, j, pos, pos2;
	int32_t temp = 0;
	int32_t i_test = 0;
	int rows2, columns2; //Half the number of rows/columns
	double pixelvalue;
	PI = atan2(1, 1) * 4;

	if( scale_factor < 0){ //factor out of range
		fprintf(stderr, "scale_factor needs tob be a positive integer.");
		return -1;			
	}

	double log_rows = log2(rows);
	double log_columns = log2(columns);
	int32_t log_rows2 = log_rows;
	int32_t log_columns2 = log_columns;
	if( (log_rows-(double) log_rows2!=0) || (log_columns-(double) log_columns2!=0)){ //wrong image size
		fprintf(stderr, "The input Image must have a width and height to a power of two!");
		return -1;			
	}


	cpl **out;
	cpl *buf_all = (cpl *) malloc(columns * rows * sizeof(cpl));
	if (NULL == buf_all) {
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	cpl *buf = (cpl *) malloc(columns * sizeof(cpl));
	if (NULL == buf) {
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}

	cpl *buf_rows = (cpl *) malloc(rows * sizeof(cpl));
		if (NULL == buf_rows) {
		fprintf(stderr, "Couldn't allocate memory. The Algorithm will Exit! (file: %s, line: %i) \n", __FILE__, __LINE__);
		return -1;
	}


		for (i = 0; i < rows; ++i) {
			//Store data temporarily in buffer
			for (j = 0; j < columns; ++j) {
				pos = i * columns + j;
				buf[j] = pixpack_source[pos].red;
				buf_all[pos] = 0;
			}
			//1D-FFT
			fft(buf, columns);
			//Write back to buffer
			for (j = 0; j < columns; ++j) {
				pos = i * columns + j;
				buf_all[pos] = buf[j];
			}
		}

		//2D-FFT transformed columns
		for (j = 0; j < columns; ++j) {
			for (i = 0; i < rows; ++i) {
				pos = i * columns + j;
				buf_rows[i] = buf_all[pos];
			}
			fft(buf_rows, rows);
			for (i = 0; i < rows; ++i) {
				pos = i * columns + j;
				buf_all[pos] = buf_rows[i];
			}
		}

		//Shifting for Zero-Frequency in the middle of the picture
		rows2 = rows / 2;     //Half the number of rows/columns
		columns2 = columns / 2;   //Half the number of rows/columns
		cpl tmp13, tmp24;
		for (j = 0; j < columns2; ++j) {
			for (i = 0; i < rows2; ++i) {
				//cpl tmp13, tmp24;
				pos = i * columns + j;
				tmp13         = buf_all[pos];
				pos2 = (i + rows2) * columns + (columns2 + j);
				buf_all[pos]       = buf_all[pos2];
				buf_all[pos2] = tmp13;

				pos = (i + rows2) * columns + j;
				tmp24         = buf_all[pos];
				pos2 = i * columns + (columns2 + j);
				buf_all[pos]    = buf_all[pos2];
				buf_all[pos2]    = tmp24;

			}
		}

		for (j = 0; j < columns; ++j) {
			for (i = 0; i < rows; ++i) {
				pos = i * columns + j;
				pixelvalue = cabs(buf_all[pos])/scale_factor;
				if(pixelvalue <= 65535){
					pixpack_target[pos].red = pixelvalue;
					pixpack_target[pos].green = pixelvalue;
					pixpack_target[pos].blue = pixelvalue;
				}else{
					pixpack_target[pos].red =65535;
					pixpack_target[pos].green = 65535;
					pixpack_target[pos].blue = 65535;
				}
				pixpack_target[pos].opacity = 0;
			}
		}

	
	free(buf_all);
	free(buf);
	free(buf_rows);
	return 0;
}
