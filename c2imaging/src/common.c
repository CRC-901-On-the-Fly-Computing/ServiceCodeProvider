/* common.c */

#include "include/common.h"

void malloc_copy_string_array(char ***target_array, char **source_array, int32_t array_size) {
	*target_array = (char **)malloc(array_size * sizeof(char *));
	if (NULL == *target_array) {
		fprintf(stderr, "Cannot allocate memory. (file: %s, line: %i)\n", __FILE__, __LINE__);
		return;
	}
	
	int i;
	for (i=0; i<array_size; ++i) {
		(*target_array)[i] = (char *)malloc(strlen(source_array[i])+1);
		if (NULL == (*target_array)[i]) {
			fprintf(stderr, "Cannot allocate memory. (file: %s, line: %i)\n", __FILE__, __LINE__);
			free(*target_array);
			return;
		}
		memset((*target_array)[i], 0, strlen(source_array[i])+1);
		strncpy((*target_array)[i], source_array[i], strlen(source_array[i]));
	}
}

void free_string_array(char ***array, int32_t array_size) {
	int i;
	for (i=0; i<array_size; ++i) {
		free((*array)[i]);
	}
	
	free(*array);
	*array = NULL;
}
