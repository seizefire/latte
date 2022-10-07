#include <io.h>
#include <direct.h>
#include <windows.h>
#include <filesystem>

#include "latte.hpp"
#include "logger.hpp"
#include "commands.hpp"

bool is_tty = false;
std::string assembly_directory = "";
std::string working_directory = "";

void base_help_message() {
	logger::log(logger::dye("latte", 150, 75, 0) + logger::dye(" [arguments]", 255, 255, 255));
	logger::log(logger::dye("\nCommands:", 155, 155, 155));
	logger::help("[a]dd", "<name> <path>        ", "Adds an already-installed JVM");
	logger::help("[c]lean", "                   ", "Removes any inaccessible JVMs");
	logger::help("[cur]rent", "                 ", "Prints the JVM being used in PATH, along with the release version defaults");
	logger::help("[i]nstall", "[arguments]      ", "Installs a new JVM");
	logger::help("[l]ist", "                    ", "Lists the names of all available JVMs");
	logger::help("[m]ove", "<name> <new_path>   ", "Moves the selected JVM to a different location");
	logger::help("[p]rint", "<name>             ", "Prints information on a JVM");
	logger::help("[reg]ister", "<name>          ", "Registers a JVM as its version default");
	logger::help("[r]e[m]ove", "[--keep] <name> ", "Removes a JVM (use --keep to remove the JVM without deleting it)");
	logger::help("[ren]ame", "<old> <new>       ", "Renames a JVM");
	logger::help("[u]se", "<name>               ", "Uses the given Java installation in PATH");
	logger::log("");
}

int main(int argc, char** argv){
	is_tty = isatty(fileno(stdout)) > 0 && !std::getenv("NO_COLOR");
	logger::init();
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
	switch(argc){
		default:
			if(strcmp(argv[1], "add") == 0 || strcmp(argv[1], "a") == 0){
				commands::add(argv[2], argv[3]);
				return 0;
			}
			else if(strcmp(argv[1], "move") == 0 || strcmp(argv[1], "m") == 0){
				commands::move(argv[2], argv[3]);
				return 0;
			}
			else if(strcmp(argv[1], "rename") == 0 || strcmp(argv[1], "ren") == 0){
				commands::rename(argv[2], argv[3]);
				return 0;
			}
		case 3:
			if(strcmp(argv[1], "print") == 0 || strcmp(argv[1], "p") == 0){
				commands::print(argv[2]);
				return 0;
			}
			else if(strcmp(argv[1], "register") == 0 || strcmp(argv[1], "reg") == 0){
				commands::reg(argv[2]);
				return 0;
			}
			else if(strcmp(argv[1], "use") == 0 || strcmp(argv[1], "u") == 0){
				commands::use(argv[2]);
				return 0;
			}
			else if(strcmp(argv[1], "remove") == 0 || strcmp(argv[1], "rm") == 0){
				if(argc > 3 && strcmp(argv[2], "--keep") == 0){
					commands::remove(argv[3], true);
				}else{
					commands::remove(argv[2], false);
				}
				return 0;
			}
		case 2:
			if(strcmp(argv[1], "clean") == 0 || strcmp(argv[1], "c") == 0){
				commands::clean();
				return 0;
			}
			else if(strcmp(argv[1], "current") == 0 || strcmp(argv[1], "cur") == 0){
				commands::current();
				return 0;
			}
			else if(strcmp(argv[1], "list") == 0 || strcmp(argv[1], "l") == 0){
				commands::list();
				return 0;
			}
			else if(strcmp(argv[1], "install") == 0 || strcmp(argv[1], "i") == 0){
				commands::install(argv, argc);
				return 0;
			}
		case 1:
			base_help_message();
			return 0;
	}
}