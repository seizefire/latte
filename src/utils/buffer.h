#pragma once

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

u8 read_u8(char* buffer);
u16 read_u16be(char* buffer);
u16 read_u16le(char* buffer);
u32 read_u32be(char* buffer);
u32 read_u32le(char* buffer);
u64 read_u64be(char* buffer);
u64 read_u64le(char* buffer);

void write_u8(char* buffer, u8 number);
void write_u16be(char* buffer, u16 number);
void write_u16le(char* buffer, u16 number);
void write_u32be(char* buffer, u32 number);
void write_u32le(char* buffer, u32 number);
void write_u64be(char* buffer, u64 number);
void write_u64le(char* buffer, u64 number);