#include <ctime>
#include <cstring>
#include <iostream>

#include "jvm.hpp"
#include "logger.hpp"

std::string prefix = "jvm ";
std::string beginTimestamp = "[";
std::string endTimestamp = "] ";
std::string fatalPrefix = "fatal ";
std::string infoPrefix = "info ";
std::string warnPrefix = "warn ";
std::string errorPrefix = "error ";

int previous_progress_length = 0;
bool progress_started = false;
std::string progress_task = "";

std::string timestamp(){
	time_t secs;
	time(&secs);
	char buffer[80];
	strftime(buffer, 79, "%Y-%m-%dT%H:%M:%S", localtime(&secs));
	return beginTimestamp + std::string(buffer) + endTimestamp;
}
void logger::init(){
	if(is_tty){
		prefix = "\x1b[38;2;0;255;255mjvm\x1b[0m ";
		beginTimestamp = "\x1b[38;2;155;125;255m[";
		endTimestamp = "]\x1b[0m ";
		fatalPrefix = "\x1b[38;2;255;85;0mfatal\x1b[0m \x1b[38;2;255;155;70m";
		infoPrefix = "\x1b[38;2;0;135;255minfo\x1b[0m \x1b[38;2;255;255;255m";
		warnPrefix = "\x1b[38;2;205;205;0mwarn\x1b[0m \x1b[38;2;255;255;55m";
		errorPrefix = "\x1b[38;2;255;0;0merror\x1b[0m \x1b[38;2;255;105;105m";
	}
}
std::string logger::dye(std::string text, int r, int g, int b){
	if(is_tty){
		return "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + text + "\x1b[0m";
	}
	return text;
}
void logger::fatal(std::string text){
	std::cout << std::endl << prefix + timestamp() + fatalPrefix + text;
}
void logger::info(std::string text){
	std::cout << std::endl << prefix + timestamp() + infoPrefix + text;
}
void logger::log(std::string text){
	std::cout << std::endl << text + (is_tty ? "\x1b[0m" : "");
}
void logger::warn(std::string text){
	std::cout << std::endl << prefix + timestamp() + warnPrefix + text;
}
void logger::error(std::string text){
	std::cout << std::endl << prefix + timestamp() + errorPrefix + text;
}
bool logger::start_progress(std::string task, std::string name, std::string initialStatus){
	if(progress_started){
		return false;
	}
	progress_task = task;
	progress_started = true;
	if(is_tty){
		std::cout << std::endl << prefix + timestamp() + logger::dye(task, 0, 255, 0) + " " + logger::dye(name, 255, 255, 255) + " " + logger::dye(initialStatus, 255, 255, 255);
		previous_progress_length = initialStatus.length();
	}else{
		std::cout << std::endl << prefix + timestamp() + task + " " + name + "...";
	}
	return true;
}
void logger::update_progress(std::string status){
	if(!progress_started){
		return;
	}
	if(is_tty){
		std::cout << (previous_progress_length == 0 ? "" : "\x1b[" + std::to_string(previous_progress_length) + "D") + "\x1b[0K" + logger::dye(status, 255, 255, 255);
		previous_progress_length = status.length();
	}
}
void logger::close_progress(std::string end_status){
	if(!progress_started){
		return;
	}
	if(is_tty){
		std::cout << "\r\x1b[0K" + prefix + timestamp() + logger::dye(progress_task, 0, 255, 0) + " " + logger::dye(end_status, 255, 255, 255);
	}else{
		std::cout << std::endl << prefix + timestamp() + progress_task + " " + end_status;
	}
	progress_started = false;
}
void logger::help(std::string command, std::string arguments, std::string purpose){
	if(is_tty){
		logger::log("  \x1b[38;2;0;255;255mjvm \x1b[38;2;0;195;145m" + command + " \x1b[38;2;255;255;0m" + arguments + "\x1b[0m - " + purpose);
	}else{
		logger::log("  jvm " + command + " " + arguments + " - " + purpose);
	}
}