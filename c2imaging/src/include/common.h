/* common.h */

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void malloc_copy_string_array(char ***target_array, char **source_array, int32_t array_size);
void free_string_array(char ***array, int32_t array_size);

#endif /* __COMMON_H__ */
