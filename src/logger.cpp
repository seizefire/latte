#include <iostream>

#include "jvm.h"
#include "logger.h"

std::string logger::dye(std::string text, unsigned char r, unsigned char g, unsigned char b){
	if(!is_tty){
		return text;
	}
	return "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + text + "\x1b[0m";
}
void logger::help(std::string command, std::string arguments, std::string purpose){
	if(is_tty){
		logger::log("  \x1b[38;2;0;255;255mjvm \x1b[38;2;0;195;145m" + command + " \x1b[38;2;255;255;0m" + arguments + "\x1b[0m - " + purpose);
	}else{
		logger::log("  jvm " + command + " " + arguments + " - " + purpose);
	}
}
void logger::log(std::string text){
	std::cout << text << (is_tty ? "\x1b[0m" : "") << std::endl;
}
void logger::warn(std::string text){
	if(is_tty){
		std::cout << "\x1b[38;2;255;255;0m" << text << "\x1b[0m" << std::endl;
	}else{
		std::cout << text << std::endl;
	}
}
void logger::error(std::string text){
	if(is_tty){
		std::cout << "\x1b[38;2;255;0;0m" << text << "\x1b[0m" << std::endl;
	}else{
		std::cout << text << std::endl;
	}
}