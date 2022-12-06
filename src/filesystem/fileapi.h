#pragma once

#include <stdbool.h>

#if defined(_WIN64) || defined(_WIN32)
    #define PATH_SEPARATOR "\\"
#elif
    #define PATH_SEPARATOR "/"
#endif

extern unsigned int fapi_error;

bool fapi_can_read(const char* path);
bool fapi_can_write(const char* path);
bool fapi_exists(const char* path);
unsigned long long fapi_get_file_size(const char* path);
char* fapi_follow_symlink(const char* path);
bool fapi_is_directory(const char* path);
bool fapi_is_file(const char* path);
char* fapi_read_entire_file(const char* path);