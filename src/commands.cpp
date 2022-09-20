extern "C" {
	#include "utils/filesystem.h"
	#include "utils/release.h"
}

#include "config.hpp"
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
	char* value = find_property_value(buffer, "JAVA_VERSION");
	logger::info("Found JVM version: " + std::string(value));
	int* numbers = scan_version_string(value);
	for(int index = 0; index < 4; index++){
		logger::info(std::to_string(numbers[index]));
	}
}

void commands::add(char* name, char* path){
	if(!is_valid_jre(path)){
		logger::error("Path does not point to a valid JRE/JDK");
		return;
	}
	if(get_id_from_name(name) != UINT32_MAX){
		logger::error("Name is already taken");
		return;
	}
	jvm vm = jvm();
	vm.name = std::string(name);
	vm.path = std::string(path);
	scan_release_file(vm, path);
}
void commands::clean(){
	
}
void commands::current(){
	
}
void commands::debug(char* key){
	logger::log("command: debug");
	if(strcmp(key, "config") == 0){
		load_config();
		logger::log("JVMs: " + std::to_string(jvms.size()));
		logger::log("JDK Count: " + std::to_string(jdk_defaults.size()));
		logger::log("JRE Count: " + std::to_string(jre_defaults.size()));
		logger::log("Current ID: " + std::to_string(current_id));
		logger::log("Next ID: " + std::to_string(next_id));
	}
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