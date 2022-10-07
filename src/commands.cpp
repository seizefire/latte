extern "C" {
	#include "utils/filesystem.h"
	#include "config/release.h"
	#include "config/store.h"
}

#include "logger.hpp"
#include "commands.hpp"

#include <regex>
#include <fstream>
#include <sstream>
#include <cstring>
#include <filesystem>

const char sep = std::filesystem::path::preferred_separator;

void scan_release_file(jvm &vm, std::string path){
	std::string release_path = path + sep + "release";
	if(!std::filesystem::exists(release_path)){
		logger::warn("JVM does not contain a \"releases\" file. Advanced information will not be available");
		return;
	}
	char* buffer = read_entire_file(release_path.c_str());
	char* version = find_property_value(buffer, "JAVA_VERSION");
	memcpy(vm.version, version, strlen(version));
	logger::info("Found JVM version: " + std::string(vm.version));
	scan_for_implementation(&vm, buffer);
	scan_for_version(&vm);
	scan_for_build_date(&vm, buffer);
	scan_for_alternate_version(&vm, buffer);
}

void commands::add(char* name, char* path){
	if(!is_valid_jre(path)){
		logger::error("Path does not point to a valid JRE/JDK");
		return;
	}
	/*if(get_id_from_name(name) != UINT32_MAX){
		logger::error("Name is already taken");
		return;
	}*/
	jvm vm = jvm();
	memcpy(vm.name, name, strlen(name));
	scan_release_file(vm, path);
	logger::info("Major Version: " + std::to_string(vm.major_version));
	logger::info("Minor Version: " + std::to_string(vm.minor_version));
	logger::info("Patch Version: " + std::to_string(vm.patch_version));
}
void commands::clean(){
	
}
void commands::current(){
	
}
void commands::install(char** argv, int argc){
	
}
void commands::list(){
	
}
void commands::move(char* name, char* path){
	
}
void commands::print(char* name){
	
}
void commands::reg(char* name){
	
}
void commands::rename(char* old_name, char* new_name){
	
}
void commands::remove(char* name, bool keep){
	
}
void commands::use(char* name){
	
}