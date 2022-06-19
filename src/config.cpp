#include <filesystem>

#include "jvm.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "filesystem.hpp"

const char sep = std::filesystem::path::preferred_separator;

uint32_t next_id = 0;
uint32_t current_id = 0xffffffffU;

std::map<uint32_t, jvm> jvms = std::map<uint32_t, jvm>();
std::map<uint16_t, uint32_t> jdk_defaults = std::map<uint16_t, uint32_t>();
std::map<uint16_t, uint32_t> jre_defaults = std::map<uint16_t, uint32_t>();

bool load_config(){
	std::string path = assembly_directory + sep + "jvm_v1.cfg";
	jvms.clear();
	jdk_defaults.clear();
	jre_defaults.clear();
	bfinput_stream stream = bfinput_stream(path);
	if(!stream.good()){
		if(!std::filesystem::exists(path)){
			return true;
		}
		logger::error("Unable to read config");
		return false;
	}
	if(stream.read_string(4) != "JVM#"){
		logger::error("Corrupted config");
		return false;
	}
	if(stream.read_uint32_le() != 0xa70269f0u){
		logger::error("Corrupted config");
		return false;
	}
	current_id = stream.read_uint32_le();
	next_id = stream.read_uint32_le();
	uint32_t jvm_count = stream.read_uint32_le();
	uint16_t jdk_default_count = stream.read_uint16_le();
	uint16_t jre_default_count = stream.read_uint16_le();
	for(uint32_t i = 0; i < jvm_count; ++i){
		jvm inst = jvm();
		uint32_t id = stream.read_uint32_le();
		uint8_t name_length = stream.read_uint8();
		uint16_t path_length = stream.read_uint16_le();
		inst.name = stream.read_string(name_length);
		inst.path = stream.read_string(path_length);
		inst.major_version = stream.read_uint16_le();
		inst.minor_version = stream.read_uint16_le();
		inst.patch_version = stream.read_uint16_le();
		inst.build_number = stream.read_uint16_le();
		inst.vendor = stream.read_uint8();
		inst.implementation = stream.read_uint8();
		inst.architecture = stream.read_uint8();
		uint8_t flags = stream.read_uint8();
		inst.jre = flags & 1 == 1;
		jvms.insert_or_assign(id, inst);
	}
	for(uint16_t j = 0; j < jdk_default_count; j++){
		uint16_t version = stream.read_uint16_le();
		uint32_t id = stream.read_uint32_le();
		jdk_defaults.insert_or_assign(version, id);
	}
	for(uint16_t k = 0; k < jre_default_count; k++){
		uint16_t version = stream.read_uint16_le();
		uint32_t id = stream.read_uint32_le();
		jre_defaults.insert_or_assign(version, id);
	}
	stream.close();
	return true;
}
bool save_config(){
	bfoutput_stream stream = bfoutput_stream(assembly_directory + sep + "jvm_v1.cfg", std::ios::trunc);
	if(!stream.good()){
		logger::error("Unable to write config");
		return false;
	}
	stream.write_string("JVM#");
	stream.write_uint32_le(0xa70269f0u);
	stream.write_uint32_le(current_id);
	stream.write_uint32_le(next_id);
	stream.write_uint32_le(jvms.size());
	stream.write_uint16_le(jdk_defaults.size());
	stream.write_uint16_le(jre_defaults.size());
	for(auto jvm_pair : jvms){
		jvm jvm_element = jvm_pair.second;
		stream.write_uint32_le(jvm_pair.first);
		stream.write_uint8(jvm_element.name.length());
		stream.write_uint16_le(jvm_element.path.length());
		stream.write_string(jvm_element.name);
		stream.write_string(jvm_element.path);
		stream.write_uint16_le(jvm_element.major_version);
		stream.write_uint16_le(jvm_element.minor_version);
		stream.write_uint16_le(jvm_element.patch_version);
		stream.write_uint16_le(jvm_element.build_number);
		stream.write_uint8(jvm_element.vendor);
		stream.write_uint8(jvm_element.implementation);
		stream.write_uint8(jvm_element.architecture);
		stream.write_uint8(jvm_element.jre ? 1 : 0);
	}
	for(auto jdk_default : jdk_defaults){
		stream.write_uint16_le(jdk_default.first);
		stream.write_uint32_le(jdk_default.second);
	}
	for(auto jre_default : jre_defaults){
		stream.write_uint16_le(jre_default.first);
		stream.write_uint32_le(jre_default.second);
	}
	stream.close();
	return true;
}