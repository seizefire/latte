#include <direct.h>
#include <windows.h>
#include <filesystem>

#include "jvm.h"
#include "config.h"
#include "logger.h"
#include "commands.h"

bool is_tty = false;
std::string assembly_directory = "";
std::string working_directory = "";

void base_help_message() {
	logger::log(logger::dye("jvm", 0, 255, 255) + logger::dye(" [arguments]", 155, 155, 155));
	logger::log(logger::dye("\nCommands:", 155, 155, 155));
	logger::help("[a]dd", "<name> <path>        ", "Adds an already-installed JVM");
	logger::help("[c]lean", "                   ", "Removes any inaccessible JVMs");
	logger::help("[cur]rent", "                 ", "Prints the JVM being used in PATH, along with the release version defaults");
	logger::help("[i]nstall", "[arguments]      ", "Installs a new JVM");
	logger::help("[l]ist", "all                 ", "Lists the names of all available JVMs");
	logger::help("[m]ove", "<name> <new_path>   ", "Moves the selected JVM to a different location");
	logger::help("[p]rint", "<name>             ", "Prints information on a JVM");
	logger::help("[reg]ister", "<name>          ", "Registers a JVM as its version default");
	logger::help("[r]e[m]ove", "[--keep] <name> ", "Removes a JVM (use --keep to remove the JVM without deleting it)");
	logger::help("[ren]ame", "<old> <new>       ", "Renames a JVM");
	logger::help("[u]se", "<name>               ", "Uses the given Java installation in PATH");
}

int main(int argc, char** argv){
	is_tty = _isatty(fileno(stdout)) > 0 && !std::getenv("NO_COLOR");
	char buffer[MAX_PATH];
	if(GetModuleFileNameA(NULL, buffer, MAX_PATH) == 0){
		logger::error("Failed to retrieve assembly directory!");
		return 0;
	}
	assembly_directory = std::string(buffer);
	assembly_directory = assembly_directory.substr(0, assembly_directory.find_last_of("/\\"));
	if(GetCurrentDirectoryA(MAX_PATH, buffer) == 0){
		logger::error("Failed to retrieve working directory!");
		return 0;
	}
	working_directory = std::string(buffer);
	HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD console_mode;
	GetConsoleMode(out_handle, &console_mode);
	SetConsoleMode(out_handle, console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);
	if(argc == 1){
		base_help_message();
		return 0;
	}
	if(strcmp(argv[1], "debug") == 0 && argc > 2){
		commands::debug(argv[2]);
	}
}