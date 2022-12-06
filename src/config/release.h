#pragma once

#include <stdbool.h>

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

char* find_property_value(char* buffer, const char* key);
bool scan_for_implementation(char* buffer, u8* vendor, u8* implementation);
bool scan_for_build_date(char* buffer, u8* date, u8* month, u16* year);
bool scan_for_version(const char* version, u16* major_version, u16* minor_version, u16* patch_version, u16* build_number, u64* build_identifier);