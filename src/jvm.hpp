#pragma once

#include <map>
#include <string>
#include <vector>

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

typedef struct jvm_history_entry_ {
	u64 timestamp;
	u8 action;
	u8 extra_info_size;
	char* extra_info;
} jvm_history_entry;

typedef struct jvm_ {
	u32 id;
	std::string name;
	std::string version;
	u64 build_identifier;
	u16 major_version;
	u16 minor_version;
	u16 patch_version;
	u16 build_number;
	u16 build_year;
	u8 build_month;
	u8 build_date;
	u8 vendor;
	u8 implementation;
	u8 architecture;
	u64 add_timestamp;
	std::map<std::string, std::string> extra_attributes;
	std::vector<jvm_history_entry> history;
	bool is_jre;
	bool is_major_default;
	bool is_build_default;
} jvm;