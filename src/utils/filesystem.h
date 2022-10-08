#include <stdbool.h>

char* read_entire_file(const char* path);
bool is_directory(const char* path);
bool is_regular_file(const char* path);
bool is_valid_jre(const char* path);
bool compare_paths(const char* path1, const char* path2);