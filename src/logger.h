#pragma once
#include <string>

namespace logger {
	std::string dye(std::string text, unsigned char r, unsigned char g, unsigned char b);
	void help(std::string command, std::string arguments, std::string purpose);
	void log(std::string text);
	void warn(std::string text);
	void error(std::string text);
}