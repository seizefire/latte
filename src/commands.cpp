#include "config.h"
#include "logger.h"
#include "commands.h"

#include <cstring>

void commands::debug(char* key){
	if(strcmp(key, "config") == 0){
		load_config();
		logger::log("JVMs: " + std::to_string(jvms.size()));
		logger::log("JDK Count: " + std::to_string(jdk_defaults.size()));
		logger::log("JRE Count: " + std::to_string(jre_defaults.size()));
		logger::log("Current ID: " + std::to_string(current_id));
		logger::log("Next ID: " + std::to_string(next_id));
	}
}