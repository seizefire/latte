#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#ifdef _WIN32
	const char* sep = "\\";
#else
	const char* sep = "/";
#endif

bool file_exists(const char* path){
	struct stat st;
	if(stat(path, &st)){

	}
}
char* read_entire_file(const char* path){
	struct stat st;
	if(stat(path, &st) == 0){
		if(st.st_mode & S_IFREG > 0){
			FILE* file = fopen(path, "r+");
			long size = st.st_size;
			char* buffer = malloc(size + 1);
			buffer[size] = 0;
			fread(buffer, sizeof(char), size, file);
			fclose(file);
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
	char* java_path = malloc(len + 10);
	char* javaw_path = malloc(len + 11);
	java_path[len + 9] = 0;
	javaw_path[len + 10] = 0;
	memcpy(java_path, path, len);
	memcpy(javaw_path, path, len);
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