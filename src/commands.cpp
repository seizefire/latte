#include <cstring>

#include "commands.hpp"
#include "config/store.hpp"

extern "C" {
	#include "config/release.h"
	#include "logging/logger.h"
	#include "filesystem/fileapi.h"
}

void scan_release_file(jvm &vm, std::string path){
	std::string release_path = path + PATH_SEPARATOR + "release";
	if(!fapi_is_file(release_path.c_str())){
		logger_warn("JVM does not contain a \"releases\" file. Advanced information will not be available");
		return;
	}
	char* buffer = fapi_read_entire_file(release_path.c_str());
	char* version = find_property_value(buffer, std::string("JAVA_VERSION").c_str());
	vm.version = std::string(version);
	logger_logm(SC_INFO, "Found JVM version: ", vm.version.c_str());
	scan_for_implementation(buffer, &vm.vendor, &vm.implementation);
	scan_for_version(version, &vm.major_version, &vm.minor_version, &vm.patch_version, &vm.build_number, &vm.build_identifier);
	scan_for_build_date(buffer, &vm.build_date, &vm.build_month, &vm.build_year);
	free(version);
	free(buffer);
}

void commands::add(char* name, char* path){
	if(config::get_id_from_name(name) != UINT32_MAX){
		logger_error("Name is already taken");
		return;
	}
	jvm vm = jvm();
	vm.name = std::string(name);
	scan_release_file(vm, path);
	logger_logm(SC_INFO, "Major Version: ", std::to_string(vm.major_version).c_str());
	logger_logm(SC_INFO, "Minor Version: ", std::to_string(vm.minor_version).c_str());
	logger_logm(SC_INFO, "Patch Version: ", std::to_string(vm.patch_version).c_str());
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