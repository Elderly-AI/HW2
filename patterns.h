//#include "patterns.h"

unsigned int pattern_entity(const char *array, 
const char *pattern, const size_t pattern_len);

unsigned int pattern_entity_in_slice(const char *f_array, 
const char *s_array, const char *pattern, const size_t pattern_len);

unsigned long long int pattern_entity_in_array(const char **array, 
const size_t array_size, const char *pattern);