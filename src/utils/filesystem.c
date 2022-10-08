#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "winfileapi.h"

#ifdef _WIN32
	const char* sep = "\\";
#else
	const char* sep = "/";
#endif

bool file_exists(const char* path){
	struct stat st;
	if(stat(path, &st)){
		return true;
	}
	return false;
}
char* read_entire_file(const char* path){
	struct stat st = {0};
	if(stat(path, &st) == 0){
		if((st.st_mode & S_IFREG) > 0){
			FILE* file = fopen(path, "rb");
			long size = st.st_size;
			char* buffer = malloc(size + 1);
			buffer[size] = 0;
			fseek(file, 0L, SEEK_SET);
			int bytesRead = fread(buffer, sizeof(char), size, file);
			fclose(file);
			if(bytesRead < size){
				free(buffer);
				return NULL;
			}
			return buffer;
		}
	}
	return NULL;
}
bool is_regular_file(const char* path){
	struct stat st;
	if(stat(path, &st) == 0){
		if(st.st_mode & S_IFREG > 0){
			return true;
		}
	}
	return false;
}
bool is_directory(const char* path){
	struct stat st;
	if(stat(path, &st) == 0){
		if(st.st_mode & S_IFDIR > 0){
			return true;
		}
	}
	return false;
}
bool is_valid_jre(const char* path){
	int len = strlen(path);
	char* java_path = malloc(len + 14);
	char* javaw_path = malloc(len + 15);
	java_path[len + 13] = 0;
	javaw_path[len + 14] = 0;
	memset(java_path, 0, len + 14);
	memset(javaw_path, 0, len + 15);
	memcpy(java_path, path, len);
	memcpy(javaw_path, path, len);
	strcat(java_path, sep);
	strcat(javaw_path, sep);
	strcat(java_path, "bin");
	strcat(javaw_path, "bin");
	strcat(java_path, sep);
	strcat(javaw_path, sep);
	strcat(java_path, "java.exe");
	strcat(javaw_path, "javaw.exe");
	if(is_directory(path) && is_regular_file(java_path) && is_regular_file(javaw_path)){
		free(java_path);
		free(javaw_path);
		return true;
	}
	free(java_path);
	free(javaw_path);
	return false;
}
bool compare_paths(const char* path1, const char* path2){
	char* fpath1 = realpath(path1);
	char* fpath2 = realpath(path2);
	bool val = (strcmp(fpath1, fpath2) == 0);
	free(fpath1);
	free(fpath2);
	return val;
}