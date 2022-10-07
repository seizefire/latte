#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t FILESTREAM_ERROR = 0;

// 0 = good
// 1 = end of file
// 2 = write error
// 255 = unknown

uint8_t read_uint8(FILE* file){
	unsigned char buf[1] = {0};
	if(fread(buf, sizeof(unsigned char), 1, file) < 1){
		if(feof(file)){
			FILESTREAM_ERROR = 1;
			return 0;
		}
		clearerr(file);
		FILESTREAM_ERROR = 255;
		return 0;
	}
	FILESTREAM_ERROR = 0;
	return buf[0];
}
uint16_t read_uint16_le(FILE* file){
	unsigned char buf[2] = {0, 0};
	if(fread(buf, sizeof(unsigned char), 2, file) < 2){
		if(feof(file)){
			FILESTREAM_ERROR = 1;
			return 0;
		}
		clearerr(file);
		FILESTREAM_ERROR = 255;
		return 0;
	}
	FILESTREAM_ERROR = 0;
	return buf[0] + (buf[1] << 8);
}
uint32_t read_uint32_le(FILE* file, uint32_t* number){
	unsigned char buf[4] = {0, 0, 0, 0};
	if(fread(buf, sizeof(unsigned char), 4, file) < 4){
		if(feof(file)){
			FILESTREAM_ERROR = 1;
			return 0;
		}
		clearerr(file);
		FILESTREAM_ERROR = 255;
		return 0;
	}
	FILESTREAM_ERROR = 0;
	return buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
}
char* read_string(FILE* file, size_t length){
	char* buf = malloc(length + 1);
	buf[length] = 0;
	read_to_buffer(file, buf, length);
	if(FILESTREAM_ERROR > 0){
		free(buf);
		return NULL;
	}
	return buf;
}
void read_to_buffer(FILE* file, char* buffer, size_t length){
	if(fread(buffer, sizeof(char), length, file) < length){
		if(feof(file)){
			FILESTREAM_ERROR = 1;
			return;
		}
		clearerr(file);
		FILESTREAM_ERROR = 255;
		return;
	}
	FILESTREAM_ERROR = 0;
}
void write_uint8(FILE* file, uint8_t number){
	char buf[1] = {(char) number};
	FILESTREAM_ERROR = 0;
	if(fwrite(buf, sizeof(char), 1, file) < 1){
		clearerr(file);
		FILESTREAM_ERROR = 2;
	}
}
void write_uint16_le(FILE* file, uint16_t number){
	char buf[2] = {(char) (number & 255), (char) ((number >> 8) & 255)};
	FILESTREAM_ERROR = 0;
	if(fwrite(buf, sizeof(char), 2, file) < 2){
		clearerr(file);
		FILESTREAM_ERROR = 2;
	}
}
void write_uint32_le(FILE* file, uint32_t number){
	char buf[4] = {(char) (number & 255), (char) ((number >> 8) & 255), (char) ((number >> 16) & 255), (char) ((number >> 24) & 255)};
	FILESTREAM_ERROR = 0;
	if(fwrite(buf, sizeof(char), 4, file) < 4){
		clearerr(file);
		FILESTREAM_ERROR = 2;
	}
}
void write_string(FILE* file, char* string){
	size_t length = strlen(string);
	FILESTREAM_ERROR = 0;
	if(length == 0){
		return;
	}
	if(fwrite(string, sizeof(char), length, file) < length){
		clearerr(file);
		FILESTREAM_ERROR = 2;
	}
}
void write_string_const(FILE* file, const char* string){
	size_t length = strlen(string);
	FILESTREAM_ERROR = 0;
	if(length == 0){
		return;
	}
	if(fwrite(string, sizeof(char), length, file) < length){
		clearerr(file);
		FILESTREAM_ERROR = 2;
	}
}