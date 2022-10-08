#include "winfileapi.h"

#include <fileapi.h>

#if defined(WIN32) || defined(_WIN32) 
#define PATH_SEPARATOR "\\" 
#else 
#define PATH_SEPARATOR "/" 
#endif 


char buffer1[MAX_PATH];
char buffer2[MAX_PATH];

char* realpath(char* path){
	memset(buffer1, 0, MAX_PATH);
	memset(buffer2, 0, MAX_PATH);
	GetFullPathNameA((LPCSTR) path, MAX_PATH, buffer1, buffer2);
	int length = strlen(buffer1) + strlen(buffer2) + 1;
	char fullPath = malloc(length);
	memset(fullPath, 0, length);
	memcpy(fullPath, buffer1, strlen(buffer1));
	if(strlen(buffer2) > 0){
		strcat(fullPath, PATH_SEPARATOR);
		strcat(fullPath, buffer2);
	}
	return fullPath;
}