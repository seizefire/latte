#include "buffer.h"

u8 read_u8(char* buffer){
	return (u8) buffer[0];
}
u16 read_u16be(char* buffer){
	return ((u16) ((u8) buffer[0]) << 8) | (u16) ((u8) buffer[1]);
}
u16 read_u16le(char* buffer){
	return ((u16) ((u8) buffer[1]) << 8) | (u16) ((u8) buffer[0]);
}
u32 read_u32be(char* buffer){
	return ((u32) ((u8) buffer[0]) << 24) | ((u32) ((u8) buffer[1]) << 16) | ((u32) ((u8) buffer[2]) << 8) | (u32) ((u8) buffer[3]);
}
u32 read_u32le(char* buffer){
	return ((u32) ((u8) buffer[3]) << 24) | ((u32) ((u8) buffer[2]) << 16) | ((u32) ((u8) buffer[1]) << 8) | (u32) ((u8) buffer[0]);
}
u64 read_u64be(char* buffer){
	return ((u64) ((u8) buffer[0]) << 56) | ((u64) ((u8) buffer[1]) << 48) | ((u64) ((u8) buffer[2]) << 40) | ((u64) ((u8) buffer[3]) << 32) | ((u64) ((u8) buffer[4]) << 24) | ((u64) ((u8) buffer[5]) << 16) | ((u64) ((u8) buffer[6]) << 8) | (u64) ((u8) buffer[7]);
}
u64 read_u64le(char* buffer){
	return ((u64) ((u8) buffer[7]) << 56) | ((u64) ((u8) buffer[6]) << 48) | ((u64) ((u8) buffer[5]) << 40) | ((u64) ((u8) buffer[4]) << 32) | ((u64) ((u8) buffer[3]) << 24) | ((u64) ((u8) buffer[2]) << 16) | ((u64) ((u8) buffer[1]) << 8) | (u64) ((u8) buffer[0]);
}

void write_u8(char* buffer, u8 number){
	buffer[0] = (char) number;
}
void write_u16be(char* buffer, u16 number){
	buffer[0] = (char) (number >> 8 & 255);
	buffer[1] = (char) (number & 255);
}
void write_u16le(char* buffer, u16 number){
	buffer[0] = (char) (number & 255);
	buffer[1] = (char) (number >> 8 & 255);
}
void write_u32be(char* buffer, u32 number){
	buffer[0] = (char) (number >> 24 & 255);
	buffer[1] = (char) (number >> 16 & 255);
	buffer[2] = (char) (number >> 8 & 255);
	buffer[3] = (char) (number & 255);
}
void write_u32le(char* buffer, u32 number){
	buffer[0] = (char) (number & 255);
	buffer[1] = (char) (number >> 8 & 255);
	buffer[2] = (char) (number >> 16 & 255);
	buffer[3] = (char) (number >> 24 & 255);
}
void write_u64be(char* buffer, u64 number){
	buffer[0] = (char) (number >> 56 & 255);
	buffer[1] = (char) (number >> 48 & 255);
	buffer[2] = (char) (number >> 40 & 255);
	buffer[3] = (char) (number >> 32 & 255);
	buffer[4] = (char) (number >> 24 & 255);
	buffer[5] = (char) (number >> 16 & 255);
	buffer[6] = (char) (number >> 8 & 255);
	buffer[7] = (char) (number & 255);
}
void write_u64le(char* buffer, u64 number){
	buffer[0] = (char) (number & 255);
	buffer[1] = (char) (number >> 8 & 255);
	buffer[2] = (char) (number >> 16 & 255);
	buffer[3] = (char) (number >> 24 & 255);
	buffer[4] = (char) (number >> 32 & 255);
	buffer[5] = (char) (number >> 40 & 255);
	buffer[6] = (char) (number >> 48 & 255);
	buffer[7] = (char) (number >> 56 & 255);
}