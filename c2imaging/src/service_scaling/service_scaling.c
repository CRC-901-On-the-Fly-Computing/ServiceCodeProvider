/* service_scaling.c */

#include "service_scaling.h"

typedef int32_t (*run_service_scaling_gpu_t)(PixelPacket *, PixelPacket *, int32_t, int32_t, int32_t, int32_t);

void run_service(char resource, void *handler_gpu, Image **target_images, int32_t *target_images_size, Image *source_images, int32_t source_images_size, double *params, int32_t *status_code) {
	*target_images = (Image *)malloc(sizeof(Image));
	if (NULL == *target_images) {
		fprintf(stderr, "!!! ERROR: Cannot allocate memory.");
		*status_code = -1;
		return;
	}
	
	int32_t rows				= source_images[0].rows;
	int32_t columns				= source_images[0].columns;
	int32_t scale_up_factor		= (int32_t)params[0];
	int32_t scale_down_factor	= (int32_t)params[1];
	int32_t new_rows			= 0;
	int32_t new_columns			= 0;
	
	if(rows%scale_down_factor != 0) { new_rows = (rows*scale_up_factor)/scale_down_factor+1; }
	else new_rows = (rows*scale_up_factor)/scale_down_factor;
	if(columns%scale_down_factor != 0) { new_columns = (columns*scale_up_factor)/scale_down_factor+1; }
	else new_columns = (columns*scale_up_factor)/scale_down_factor;
	
	(*target_images)[0].rows	= new_rows;
	(*target_images)[0].columns	= new_columns;
	*target_images_size			= 1;
	(*target_images)[0].image	= malloc((*target_images)[0].rows * (*target_images)[0].columns * sizeof(PixelPacket));
	if (NULL == (*target_images)[0].image) {
		fprintf(stderr, "!!! ERROR: Cannot allocate memory.");
		*status_code = -1;
		return;
	}
	
	run_service_scaling_gpu_t run_service_scaling_gpu;
	char *dlsym_error;
	
	switch (resource) {
		case 'c':
			*status_code = run_service_scaling_cpu((*target_images)[0].image, source_images[0].image, source_images[0].rows, source_images[0].columns, (int32_t)params[0], (int32_t)params[1]);
			break;
		case 'g':
			#ifdef PGCC
			handler_gpu = dlopen("../bin/plugins/libservice_scaling_gpu.so", RTLD_LAZY | RTLD_GLOBAL);
			if (NULL == handler_gpu) {
				fprintf(stderr, "Cannot open library libservice_scaling_gpu.so: %s (file: %s, line: %i)\n", dlerror(), __FILE__, __LINE__);
				exit(EXIT_FAILURE);
			}
			#endif
			run_service_scaling_gpu = (run_service_scaling_gpu_t) dlsym(handler_gpu, "run_service_scaling_gpu");
			dlsym_error = dlerror();
			if (dlsym_error) {
				fprintf(stderr, "Cannot load symbol \"run_service_scaling_gpu\": %s (file: %s, line: %i)\n", dlsym_error, __FILE__, __LINE__);
				break;
			}
			*status_code = run_service_scaling_gpu((*target_images)[0].image, source_images[0].image, source_images[0].rows, source_images[0].columns, (int32_t)params[0], (int32_t)params[1]);
			#ifdef PGCC
			dlclose(handler_gpu);
			#endif
			break;
		case 'f':
			fprintf(stderr, "!!! ERROR: Scaling has no FPGA implementation.");
			*status_code = -1;
			break;
		case 's':
		default:
			*status_code = run_service_scaling_scpu((*target_images)[0].image, source_images[0].image, source_images[0].rows, source_images[0].columns, (int32_t)params[0], (int32_t)params[1]);
			break;
	}
}

void get_service_ids(char ***id_strings, int32_t *id_strings_size) {
	char *strings[] = {
		"SC",
		"sc",
		"SCALING",
		"scaling"
	};
	*id_strings_size = 4;
	
	malloc_copy_string_array(id_strings, strings, *id_strings_size);
}

void get_service_info(char ***info_strings, int32_t *info_strings_size) {
	char *strings[] = {
		"Resize input image by factor scale_up/scale_down.\n", "Scaling is not available on the FPGA."
	};
	*info_strings_size = 2;
	
	malloc_copy_string_array(info_strings, strings, *info_strings_size);
}

void get_service_params(char ***param_strings, int32_t *param_strings_size) {
	char *strings[] = {"scale_up_factor", "scale_down_factor"};
	*param_strings_size = 2;
	
	malloc_copy_string_array(param_strings, strings, *param_strings_size);
}

void get_service_resources(char ***resource_strings, int32_t *resource_strings_size) {
	char *strings[] = {
		"CPU", "SCPU", "GPU",
		"cpu", "scpu", "gpu",
		"C"  , "S"   , "G"  ,
		"c"  , "s"   , "g"  
	};
	*resource_strings_size = 12;
	
	malloc_copy_string_array(resource_strings, strings, *resource_strings_size);
}

int32_t get_number_of_input_images(void) {
	return 1;
}

int32_t get_number_of_output_images(void) {
	return 1;
}
