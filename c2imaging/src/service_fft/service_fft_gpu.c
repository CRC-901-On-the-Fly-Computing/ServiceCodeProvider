/* service_fft_gpu.c */

#include "service_fft.h"


typedef float complex cpl;

int32_t rev(int32_t inp, int32_t numbits, int32_t rows, int32_t columns) {
	int32_t input = inp;
	int32_t output = 0;
	int32_t index = 0;
	int32_t temp;
	char binaer[numbits];
	char c;
	//Binary Representation of the number
	for (index = 0; index < numbits; index += 1) {
		temp = pow(2, numbits-1 - index);
		if (temp <= input) {
			input = input - temp;
			binaer[numbits-1 - index] = '1';
		} else {
			binaer[numbits-1 - index] = '0';
		}
	}	
	//Here is the Butterfly-Operation
	for (index = 0; index < numbits / 2; index += 1) {
		c = binaer[index];
		binaer[index] = binaer[numbits-1 - index];
		binaer[numbits-1 - index] = c;
	}
	//From binary to decimal
	for (index = 0; index < numbits; index += 1) {
		temp = 0;
		if (binaer[index] == '1') {
			temp = pow(2, index);
			output = temp+output;
		}
	}
	return output;
}

void bit_reverse_copy(cpl buf[], cpl out[], int32_t n2, int32_t rows, int32_t columns) {
	int32_t k;
	int32_t n= log2(n2);
	int32_t input;
	for (k = 0; k < n2 - 1; k += 1) {
		input = rev(k, n, rows,columns);
		out[input] = buf[k]; //Butterfly Operation
	}
}

//Iterative FFT: See Cormen, Leiserson, River, Stein - Introduction to Algorithm
void gpu_fft_ini(cpl buf[], cpl out[], int32_t n, int32_t rows, int32_t columns) {
	int32_t m;
	int32_t s;
	int32_t k;
	int32_t j;
	int32_t i;
	cpl wm;
	cpl w;
	cpl t;
	cpl u;
	double PI = atan2(1, 1) * 4;
	bit_reverse_copy(buf, out, n, rows,columns);
	for (s = 1; s <= log2(n); s += 1) {
		m = pow(2, s);
		wm = cexp(-2 * I * PI / m);
		for (k = 0; k <= n - 1; k += m) {
			w = 1;
			for (j = 0; j <= m / 2 - 1; j += 1) {
				t = w * out[k + j + m / 2];
				u = out[k + j];
				out[k + j] = u + t;
				out[k + j + m / 2] = u - t;
				w = w * wm;
			}
		}
	}
}

void gpu_fft(cpl buf[], int32_t n, int32_t rows, int32_t columns)
{
	cpl out[n];
	int32_t i2;
	//Temporary Buffer for the transformed row/column
	for (i2 = 0; i2 < n; i2++){
		out[i2] = buf[i2];
	}
	gpu_fft_ini(out, buf, n, rows,columns);
}

int32_t run_service_fft_gpu(PixelPacket *pixpack_target, PixelPacket *pixpack_source, int32_t rows, int32_t columns, int32_t scale_factor) {
	int32_t i, j, hj, pos, pos2;
	int32_t rows2, columns2;  //Half the number of rows/columns
	double pixelvalue;

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

	cpl tmp13, tmp24;
	rows2 = rows / 2;     //Half the number of rows
	columns2 = columns / 2;    //Half the number of columns	
	
	for (i=0; i<rows; ++i) {	
		//Store data temporarily in buffer
		for (j=0; j<columns; ++j) {
			pos = i*columns+j;
			buf[j] = pixpack_source[pos].red;
			buf_all[pos] = 0;
		}
		//1D-FFT	
		gpu_fft(buf, columns,rows,columns);
		//Write back to buffer
		for (hj=0; hj<columns; ++hj) {
			pos = i*columns+hj;
			buf_all[pos] = buf[hj];
		}
	}

	//2D-FFT transformed columns
	//#pragma acc kernels copyin(buf_rows[0:rows]) copyin(buf_all[0:columns*rows])
		//{
		for (j=0; j<columns; ++j) {
			for (i=0; i<rows; ++i) {
				pos = i*columns+j;
				buf_rows[i] = buf_all[pos];
			}
			//gpu_fft(buf_rows, rows, rows,columns);
			//====================================
				cpl out[rows];
				int32_t i2;
				//Temporary Buffer for the transformed row/column
				for (i2 = 0; i2 < rows; i2++){
					out[i2] = buf_rows[i2];
				}
				//gpu_fft_ini(out, buf_rows, rows, rows,columns);
				//==============================================
					int32_t m, s, k, z;
					cpl wm;
					cpl w;
					cpl t;
					cpl u;
					double PI = atan2(1, 1) * 4;
					//bit_reverse_copy(out, buf_rows, rows, rows,columns);
					//===================================================
						int32_t k2;
						int32_t n2= log2(rows);
						int32_t input;
						for (k2 = 0; k2 < rows - 1; k2 += 1) {
							input = rev(k2, n2, rows,columns);
							buf_rows[input] = out[k2]; //Butterfly Operation
						}
					//===================================================
					for (s = 1; s <= log2(rows); s += 1) {
						m = pow(2, s);
						//wm = cexp(-2 * I * PI / m);
						wm = cos(-2*PI/m) + I *sin(-2*PI/m);
						for (k = 0; k <= rows - 1; k += m) {
							w = 1;
							for (z = 0; z <= m / 2 - 1; z += 1) {
								t = w * buf_rows[k + z + m / 2];
								u = buf_rows[k + z];
								buf_rows[k + z] = u + t;
								buf_rows[k + z + m / 2] = u - t;
								w = w * wm;
							}
						}
					}
				//==============================================

			//====================================
			for (i=0; i<rows; ++i) {
				pos = i*columns+j;
				buf_all[pos] = buf_rows[i];
			}
		}
	//}
		//Shifting for Zero-Frequency in the middle of the picture
		#pragma acc kernels copyin(buf_all[0:columns*rows]) copyout(pixpack_target[0:columns*rows])
		{
			#pragma acc loop independent
			for (j=0; j<columns2; ++j) {
				#pragma acc loop independent private(pos,pos2,tmp13,tmp24)
				for (i=0; i<rows2; ++i) {
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
		
			#pragma acc loop independent
			for (j = 0; j < columns; ++j) {
				#pragma acc loop independent private(pos, pixelvalue)
				for (i = 0; i < rows; ++i) {
					pos = i * columns + j;
					pixelvalue = sqrt((creal(buf_all[pos])*creal(buf_all[pos])+cimag(buf_all[pos])*cimag(buf_all[pos])))/scale_factor;
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
		}
	free(buf_all);
	free(buf);
	free(buf_rows);
	return 0;
}
