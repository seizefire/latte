#include <stdio.h>
#include <stdint.h>

extern uint8_t FILESTREAM_ERROR;

uint8_t read_uint8(FILE* file);
uint16_t read_uint16_le(FILE* file);
uint32_t read_uint32_le(FILE* file);
char* read_string(FILE* file, size_t length);
void read_to_buffer(FILE* file, char* buffer, size_t length);
void write_uint8(FILE* file, uint8_t number);
void write_uint16_le(FILE* file, uint16_t number);
void write_uint32_le(FILE* file, uint32_t number);
void write_string(FILE* file, char* string);
void write_string_const(FILE* file, const char* string);