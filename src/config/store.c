#include "store.h"
#include "../utils/filesystem.h"
#include "../utils/filestreams.h"

#include <stdlib.h>
#include <string.h>

jvm* jvms = NULL;
uint32_t jvm_count = 0;
uint32_t next_jvm_id = 0;
jvm* current_jvm = NULL;

uint16_t jdk_default_count = 0;
uint16_t jre_default_count = 0;
jvm** jdk_defaults = NULL;
jvm** jre_defaults = NULL;
jvm** normal_jvms = NULL;

void load_latte_config(char* path){
	if(jvms != NULL){
		free(jvms);
	}
	if(jdk_defaults != NULL){
		free(jdk_defaults);
	}
	if(jre_defaults != NULL){
		free(jre_defaults);
	}
	if(normal_jvms != NULL){
		free(normal_jvms);
	}
	if(!is_regular_file(path)){
		jvms = malloc(sizeof(jvm));
		jvms[0].flags = 1;
		jvm_count = 0;
		next_jvm_id = 0;
		current_jvm = NULL;
		jdk_default_count = 0;
		jre_default_count = 0;
		jdk_defaults = malloc(sizeof(jvm*));
		jre_defaults = malloc(sizeof(jvm*));
		normal_jvms = malloc(sizeof(jvm*));
		jdk_defaults[0] = NULL;
		jre_defaults[0] = NULL;
		normal_jvms[0] = NULL;
		return;
	}
	FILE* stream = fopen(path, "r");
	jvm_count = read_uint32_le(stream);
	next_jvm_id = read_uint32_le(stream);
	uint32_t current_jvm_id = read_uint32_le(stream);
	jdk_default_count = read_uint16_le(stream);
	jre_default_count = read_uint16_le(stream);
	jvms = malloc(sizeof(jvm) * (jvm_count + 1));
	jdk_defaults = malloc(sizeof(jvm) * (jdk_default_count + 1));
	jdk_defaults[jdk_default_count] = NULL;
	jre_defaults = malloc(sizeof(jvm) * (jre_default_count + 1));
	jre_defaults[jre_default_count] = NULL;
	normal_jvms = malloc(sizeof(jvm) * (jvm_count - jdk_default_count - jre_default_count + 1));
	uint32_t jdkdi = 0, jredi = 0, nordi = 0;
	for(uint32_t i = 0; i < jvm_count; ++i){
		clean_jvm(&jvms[i]);
		jvms[i].id = read_uint32_le(stream);
		uint8_t name_length = read_uint8(stream);
		uint8_t version_length = read_uint8(stream);
		uint8_t alt_version_length = read_uint8(stream);
		read_to_buffer(stream, jvms[i].name, name_length);
		read_to_buffer(stream, jvms[i].version, version_length);
		if(alt_version_length > 0){
			read_to_buffer(stream, jvms[i].alt_version, alt_version_length);
		}
		jvms[i].major_version = read_uint16_le(stream);
		jvms[i].minor_version = read_uint16_le(stream);
		jvms[i].patch_version = read_uint16_le(stream);
		jvms[i].build_number = read_uint16_le(stream);
		jvms[i].build_year = read_uint16_le(stream);
		jvms[i].build_month = read_uint8(stream);
		jvms[i].build_date = read_uint8(stream);
		jvms[i].vendor = read_uint8(stream);
		jvms[i].implementation = read_uint8(stream);
		jvms[i].architecture = read_uint8(stream);
		jvms[i].flags = read_uint8(stream);
		if(jvms[i].id == current_jvm_id){
			current_jvm = &jvms[i];
		}
		if(jvms[i].flags & 2 == 0){
			jdk_defaults[jdkdi++] = &jvms[i];
		}else if(jvms[i].flags & 4 == 0){
			jre_defaults[jredi++] = &jvms[i];
		}else{
			normal_jvms[nordi++] = &jvms[i];
		}
	}
}
void save_latte_config(char* path){
	FILE* stream = fopen(path, "w");
	write_uint32_le(stream, jvm_count);
	write_uint32_le(stream, next_jvm_id);
	write_uint32_le(stream, current_jvm == NULL ? UINT32_MAX : current_jvm->id);
	write_uint16_le(stream, jdk_default_count);
	write_uint16_le(stream, jre_default_count);
	for(uint32_t i = 0; i < jvm_count; ++i){
		if(jvms[i].flags & 1 == 1){
			continue;
		}
		write_uint32_le(stream, jvms[i].id);
		write_uint8(stream, strlen(jvms[i].name));
		write_uint8(stream, strlen(jvms[i].version));
		write_uint8(stream, strlen(jvms[i].alt_version));
		write_string(stream, jvms[i].name);
		write_string(stream, jvms[i].version);
		write_string(stream, jvms[i].alt_version);
		write_uint16_le(stream, jvms[i].major_version);
		write_uint16_le(stream, jvms[i].minor_version);
		write_uint16_le(stream, jvms[i].patch_version);
		write_uint16_le(stream, jvms[i].build_number);
		write_uint16_le(stream, jvms[i].build_year);
		write_uint8(stream, jvms[i].build_month);
		write_uint8(stream, jvms[i].build_date);
		write_uint8(stream, jvms[i].vendor);
		write_uint8(stream, jvms[i].implementation);
		write_uint8(stream, jvms[i].architecture);
		write_uint8(stream, jvms[i].flags);
	}
}