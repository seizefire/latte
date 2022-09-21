#include <filesystem>

extern "C" {
	#include "utils/filestreams.h"
	#include "utils/filesystem.h"
}

#include "latte.hpp"
#include "config.hpp"
#include "logger.hpp"

const char sep = std::filesystem::path::preferred_separator;

uint32_t next_id = 0;
uint32_t current_id = UINT32_MAX;

std::map<uint32_t, jvm> jvms = std::map<uint32_t, jvm>();
std::map<uint16_t, uint32_t> jdk_defaults = std::map<uint16_t, uint32_t>();
std::map<uint16_t, uint32_t> jre_defaults = std::map<uint16_t, uint32_t>();

bool load_config(){
	std::string path = assembly_directory + sep + "lattev1.dat";
	jvms.clear();
	jdk_defaults.clear();
	jre_defaults.clear();
	if(!is_regular_file(path.c_str())){
		return false; // placeholder for now
	}
	FILE* stream = fopen(path.c_str(), "R");
	if(stream == NULL){
		if(!std::filesystem::exists(path)){
			return true;
		}
		logger::error("Unable to read config");
		return false;
	}
	if(read_string(stream, 8) != "LATTEV1#"){
		logger::error("Corrupted config");
		return false;
	}
	if(read_uint32_le(stream) != 0xa70269f0u){
		logger::error("Corrupted config");
		return false;
	}
	uint32_t jvm_count = read_uint32_le(stream);
	uint16_t jdk_default_count = read_uint16_le(stream);
	uint16_t jre_default_count = read_uint16_le(stream);
	current_id = read_uint32_le(stream);
	next_id = read_uint32_le(stream);
	for(uint32_t i = 0; i < jvm_count; ++i){
		jvm inst = jvm();
		uint32_t id = read_uint32_le(stream);
		uint8_t name_length = read_uint8(stream);
		uint16_t path_length = read_uint16_le(stream);
		inst.name = read_string(stream, name_length);
		inst.path = read_string(stream, path_length);
		inst.major_version = read_uint16_le(stream);
		inst.minor_version = read_uint16_le(stream);
		inst.patch_version = read_uint16_le(stream);
		inst.build_number = read_uint16_le(stream);
		inst.vendor = read_uint8(stream);
		inst.implementation = read_uint8(stream);
		inst.architecture = read_uint8(stream);
		uint8_t flags = read_uint8(stream);
		inst.jre = flags & 1 == 1;
		jvms.insert_or_assign(id, inst);
	}
	for(uint16_t j = 0; j < jdk_default_count; j++){
		uint16_t version = read_uint16_le(stream);
		uint32_t id = read_uint32_le(stream);
		jdk_defaults.insert_or_assign(version, id);
	}
	for(uint16_t k = 0; k < jre_default_count; k++){
		uint16_t version = read_uint16_le(stream);
		uint32_t id = read_uint32_le(stream);
		jre_defaults.insert_or_assign(version, id);
	}
	fclose(stream);
	return true;
}
bool save_config(){
	std::string path = assembly_directory + sep + "lattev1.dat";
	FILE* stream = fopen(path.c_str(), "w");
	if(stream == NULL){
		logger::error("Unable to write config");
		return false;
	}
	write_string(stream, "LATTEV1#");
	write_uint32_le(stream, 0xa70269f0u);
	write_uint32_le(stream, current_id);
	write_uint32_le(stream, next_id);
	write_uint32_le(stream, jvms.size());
	write_uint16_le(stream, jdk_defaults.size());
	write_uint16_le(stream, jre_defaults.size());
	for(auto jvm_pair : jvms){
		jvm jvm_element = jvm_pair.second;
		write_uint32_le(stream, jvm_pair.first);
		write_uint8(stream, jvm_element.name.length());
		write_uint16_le(stream, jvm_element.path.length());
		write_string_const(stream, jvm_element.name.c_str());
		write_string_const(stream, jvm_element.path.c_str());
		write_uint16_le(stream, jvm_element.major_version);
		write_uint16_le(stream, jvm_element.minor_version);
		write_uint16_le(stream, jvm_element.patch_version);
		write_uint16_le(stream, jvm_element.build_number);
		write_uint8(stream, jvm_element.vendor);
		write_uint8(stream, jvm_element.implementation);
		write_uint8(stream, jvm_element.architecture);
		write_uint8(stream, jvm_element.jre ? 1 : 0);
	}
	for(auto jdk_default : jdk_defaults){
		write_uint16_le(stream, jdk_default.first);
		write_uint32_le(stream, jdk_default.second);
	}
	for(auto jre_default : jre_defaults){
		write_uint16_le(stream, jre_default.first);
		write_uint32_le(stream, jre_default.second);
	}
	fclose(stream);
	return true;
}
uint32_t get_id_from_name(std::string name){
	for(auto jvm : jvms){
		if(jvm.second.name == name){
			return jvm.first;
		}
	}
	return UINT32_MAX;
}