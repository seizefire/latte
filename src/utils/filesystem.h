#include <stdbool.h>

char* read_entire_file(const char* path);
bool is_directory(const char* path);
bool is_regular_file(const char* path);
bool is_valid_jre(const char* path);