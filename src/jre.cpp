#include <filesystem>

#include "jre.h"

const char sep = std::filesystem::path::preferred_separator;

bool is_valid_jre(std::string path){
	return std::filesystem::exists(path) && std::filesystem::exists(path + sep + "bin" + sep + "java.exe") && std::filesystem::exists(path + sep + "bin" + sep + "javaw.exe");
}