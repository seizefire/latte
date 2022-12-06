#include <strings.h>
#include <Windows.h>

#include "registry.hpp"

bool set_current(jvm* entry){
	return false;
}
bool set_jdk_default(jvm* entry){
	HKEY* key;
	std::string key_name;
	if(entry->major_version <= 8){
		key_name = std::string("SOFTWARE\\JavaSoft\\Java Development Kit\\1.") + std::to_string(entry->major_version);
	}else{
		key_name = std::string("SOFTWARE\\JavaSoft\\JDK\\") + std::to_string(entry->major_version);
	}
	LONG result = RegCreateKeyExA(HKEY_LOCAL_MACHINE, key_name.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, key, NULL);
	if(result > 0){
		return false;
	}
	return true;
}
bool set_jre_default(jvm* entry){
	return false;
}