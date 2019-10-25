//#include "test.h"

size_t read_array(char ***array, const char *file_name);

bool emotion_test(const char** array, const size_t array_size,
const char *good_pattern, const char *bad_pattern);

bool fork_emotion_test(const char** array, const size_t array_size,
const char *good_pattern, const char *bad_pattern);

