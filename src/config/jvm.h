#pragma once
#include <cstdint>

typedef struct jvm_ {
	uint32_t id;
	char name[32];
	char version[32];
	char alt_version[32];
	uint16_t major_version;
	uint16_t minor_version;
	uint16_t patch_version;
	uint16_t build_number;
	uint16_t build_year;
	uint8_t build_month;
	uint8_t build_date;
	uint8_t vendor;
	uint8_t implementation;
	uint8_t architecture;
	uint8_t flags;
} jvm;

void clean_jvm(jvm* entry);
void flag_jvm_for_deletion(jvm* entry);
void unflag_jvm_for_deletion(jvm* entry);