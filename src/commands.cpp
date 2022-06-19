#include "jre.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "commands.hpp"

#include <cstring>

void commands::add(char* name, char* path){
	
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