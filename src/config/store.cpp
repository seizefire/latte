#include "store.hpp"

#include <cstring>
#include <fstream>

extern "C" {
	#include "../utils/buffer.h"
	#include "../logging/logger.h"
	#include "../filesystem/fileapi.h"
}

namespace config {
	u16 version;
	u32 next_jvm_id = 0;
	u32 current_jvm_id = UINT32_MAX;

	std::vector<jvm> jvms = std::vector<jvm>();
	std::map<u16, jvm&> jdk_major_default_map = std::map<u16, jvm&>();
	std::map<u16, jvm&> jre_major_default_map = std::map<u16, jvm&>();
	std::map<u64, jvm&> jdk_build_default_map = std::map<u64, jvm&>();
	std::map<u64, jvm&> jre_build_default_map = std::map<u64, jvm&>();

	bool auto_set_build_default = true;
	bool auto_set_path_jvm = true;
	bool auto_set_version_default = true;
	bool console_color_enabled = true;
	bool registry_enabled = true;
	bool tracking_enabled = true;

	u32 get_id_from_name(std::string name){
		for(auto vm : jvms){
			if(vm.name == name){
				return vm.id;
			}
		}
		return UINT32_MAX;
	}
	void load_latte_config(std::string path){
		if(!fapi_can_read(path.c_str())){
			if(fapi_exists(path.c_str())){
				logger_fatal("Unable to read config. Make sure latte has adequate permissions to access its containing directory (maybe use administrator mode).");
				exit(0);
			}else{
				logger_error("Config not found");
			}
			return;
		}
		jvms.clear();
		jdk_build_default_map.clear();
		jdk_major_default_map.clear();
		jre_build_default_map.clear();
		jre_major_default_map.clear();
		std::ifstream stream = std::ifstream(path, std::ios_base::binary);
		char* data = new char[17];
		stream.read(data, 17);
		if(memcmp(data, "L[", 2) != 0){
			logger_fatal("Malformed config");
			exit(0);
		}
		version = read_u16le(data + 2);
		u32 jvm_count = read_u32le(data + 4);
		current_jvm_id = read_u32le(data + 8);
		next_jvm_id = read_u32le(data + 12);
		u8 flags = read_u8(data + 16);
		auto_set_build_default = (flags & 1) != 0;
		auto_set_path_jvm = (flags & 2) != 0;
		auto_set_version_default = (flags & 4) != 0;
		console_color_enabled = (flags & 8) != 0;
		registry_enabled = (flags & 16) != 0;
		tracking_enabled = (flags & 32) != 0;
		for(u32 i = 0; i < jvm_count; ++i){
			jvm vm = jvm();
			delete[] data;
			data = new char[34];
			stream.read(data, 34);
			vm.id = read_u32le(data);
			u8 name_length = read_u8(data + 4);
			u8 version_length = read_u8(data + 5);
			vm.major_version = read_u16le(data + 6);
			vm.minor_version = read_u16le(data + 8);
			vm.patch_version = read_u16le(data + 10);
			vm.build_number = read_u16le(data + 12);
			vm.build_identifier = (((u64) vm.major_version) << 48) | (((u64) vm.minor_version) << 32) | (((u64) vm.patch_version) << 16) | (u64) vm.build_number;
			vm.build_year = read_u16le(data + 14);
			vm.build_month = read_u8(data + 16);
			vm.build_date = read_u8(data + 17);
			vm.vendor = read_u8(data + 18);
			vm.implementation = read_u8(data + 19);
			vm.architecture = read_u8(data + 20);
			u8 flags = read_u8(data + 21);
			vm.add_timestamp = read_u64le(data + 22);
			vm.is_jre = (flags & 1) != 0;
			vm.is_build_default = (flags & 2) != 0;
			vm.is_major_default = (flags & 4) != 0;
			u8 extra_attribute_count = read_u8(data + 30);
			u8 history_count = read_u8(data + 31);
			u32 length = 32;
			vm.extra_attributes = std::map<std::string, std::string>();
			for(u8 j = 0; j < extra_attribute_count; ++j){
				u8 key_length = read_u8(data + length);
				u8 value_length = read_u8(data + length + 1);
				length = key_length + value_length;
				delete[] data;
				data = new char[length + 2];
				stream.read(data, length + 2);
				std::string key = std::string(data, key_length);
				std::string value = std::string(data + key_length, value_length);
				vm.extra_attributes.insert_or_assign(key, value);
			}
			vm.history = std::vector<jvm_history_entry>();
			for(u16 k = 0; k < history_count; ++k){
				jvm_history_entry history = jvm_history_entry();
				history.action = read_u8(data + length);
				history.extra_info_size = read_u8(data + length + 1);
				length = history.extra_info_size + 8;
				delete[] data;
				data = new char[length + 2];
				stream.read(data, length + 2);
				history.timestamp = read_u64le(data);
				history.extra_info = new char[history.extra_info_size];
				memcpy(history.extra_info, data + 8, history.extra_info_size);
				vm.history.push_back(history);
			}
			if(vm.is_build_default){
				if(vm.is_jre){
					jre_build_default_map.insert_or_assign(vm.build_identifier, vm);
				}else{
					jdk_build_default_map.insert_or_assign(vm.build_identifier, vm);
				}
			}
			if(vm.is_major_default){
				if(vm.is_jre){
					jre_major_default_map.insert_or_assign(vm.major_version, vm);
				}else{
					jdk_major_default_map.insert_or_assign(vm.major_version, vm);
				}
			}
			stream.seekg(-2, std::ios_base::cur);
			jvms.push_back(vm);
		}
		delete[] data;
		stream.close();
	}
	void save_latte_config(std::string path){
		if(!fapi_can_write(path.c_str())){
			logger_error("Unable to write to config");
			return;
		}
		std::ofstream stream = std::ofstream(path, std::ios_base::binary);
		char* data = new char[17];
		memcpy(data, "L[", 2);
		write_u16le(data + 2, version);
		write_u32le(data + 4, jvms.size());
		write_u32le(data + 8, current_jvm_id);
		write_u32le(data + 12, next_jvm_id);
		write_u8(data + 16, (auto_set_build_default ? 1 : 0) |
							(auto_set_path_jvm ? 2 : 0) |
							(auto_set_version_default ? 4 : 0) |
							(console_color_enabled ? 8 : 0) |
							(registry_enabled ? 16 : 0) |
							(tracking_enabled ? 32 : 0));
		stream.write(data, 17);
		delete[] data;
		for(auto vm : jvms){
			data = new char[32];
			write_u32le(data, vm.id);
			write_u8(data + 4, vm.name.length());
			write_u8(data + 5, vm.version.length());
			write_u16le(data + 6, vm.major_version);
			write_u16le(data + 8, vm.minor_version);
			write_u16le(data + 10, vm.patch_version);
			write_u16le(data + 12, vm.build_number);
			write_u16le(data + 14, vm.build_year);
			write_u8(data + 16, vm.build_month);
			write_u8(data + 17, vm.build_date);
			write_u8(data + 18, vm.vendor);
			write_u8(data + 19, vm.implementation);
			write_u8(data + 20, vm.architecture);
			write_u8(data + 21, (vm.is_jre ? 1 : 0) |
								(vm.is_build_default ? 2 : 0) |
								(vm.is_major_default ? 4 : 0));
			write_u64le(data + 22, vm.add_timestamp);
			write_u8(data + 30, vm.extra_attributes.size());
			write_u8(data + 31, vm.history.size());
			stream.write(data, 32);
			delete[] data;
			for(auto pair : vm.extra_attributes){
				u8 key_length = pair.first.length();
				u8 value_length = pair.second.length();
				u16 length = key_length + value_length + 2;
				data = new char[length];
				write_u8(data, key_length);
				write_u8(data + 1, value_length);
				memcpy(data + 2, pair.first.c_str(), key_length);
				memcpy(data + key_length + 2, pair.second.c_str(), value_length);
				stream.write(data, length);
				delete[] data;
			}
			for(auto entry : vm.history){
				u16 length = entry.extra_info_size + 10;
				data = new char[length];
				write_u8(data, entry.action);
				write_u8(data + 1, entry.extra_info_size);
				write_u64le(data + 2, entry.timestamp);
				memcpy(data + 10, entry.extra_info, entry.extra_info_size);
				stream.write(data, length);
				delete[] data;
			}
		}
		stream.write("\x00\x00\x00", 4);
		stream.close();
	}
}