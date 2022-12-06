#include <stdio.h>
#include <fileapi.h>
#include <sys/stat.h>

#include "fileapi.h"

#define BUFFER_SIZE 2048
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

u8 u8_buffer[BUFFER_SIZE];
u16 u16_buffer[BUFFER_SIZE];
u32 fapi_error = 0;

struct stat st;

bool fapi_can_read(const char* path){
	if(fapi_is_file(path)){
		FILE* file = fopen(path, "r");
		if(file == NULL){
			fapi_error = errno;
			return false;
		}
		fclose(file);
		return true;
	}
	return false;
}
bool fapi_can_write(const char* path){
	if(fapi_is_file(path) || fapi_error == ENOENT){
		bool exists = fapi_error == 0;
		FILE* file = fopen(path, exists ? "a" : "w");
		if(file == NULL){
			fapi_error = errno;
			return false;
		}
		fclose(file);
		if(!exists){
			remove(path);
		}
		return true;
	}
	return false;
}
bool fapi_exists(const char* path){
	fapi_error = 0;
	int error = stat(path, &st);
	if(error == 0){
		return true;
	}else{
		fapi_error = errno;
		return false;
	}
}
unsigned long long fapi_get_file_size(const char* path){
	if(fapi_is_file(path)){
		return st.st_size;
	}
	return __UINT64_MAX__;
}
char* fapi_follow_symlink(const char* path){
	if(!fapi_exists(path)){
		return NULL;
	}
	int path_length = strlen(path) + 1;
	u16* wide_path = calloc(path_length, 2);
	memset(u8_buffer, 0, BUFFER_SIZE);
	memset(u16_buffer, 0, BUFFER_SIZE * 2);
	mbstowcs(wide_path, path, path_length);
	GetFullPathNameW(wide_path, BUFFER_SIZE, u16_buffer, NULL);
	wcstombs(u8_buffer, u16_buffer, BUFFER_SIZE);
	int length = strlen(u8_buffer) + 1;
	char* fullPath = malloc(length);
	memcpy(fullPath, u8_buffer, length);
	free(wide_path);
	return fullPath;
}
bool fapi_is_directory(const char* path){
	if(!fapi_exists(path)){
		return false;
	}
	return (st.st_mode & S_IFDIR) != 0;
}
bool fapi_is_file(const char* path){
	if(!fapi_exists(path)){
		return false;
	}
	return (st.st_mode & S_IFREG) != 0;
}
char* fapi_read_entire_file(const char* path){
	if(!fapi_is_file(path)){
		return NULL;
	}
	FILE* file = fopen(path, "r");
	char* buffer = malloc(st.st_size);
	if(fread(buffer, 1, st.st_size, file) < st.st_size){
		free(buffer);
		return NULL;
	}
	fclose(file);
	return buffer;
}