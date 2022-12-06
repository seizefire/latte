#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"

const char* MAIN_HELP_MESSAGES[33] = {
	"[a]dd", "<name> <path>        ", "Adds an already-installed JVM",
	"[c]lean", "                   ", "Removes any inaccessible JVMs",
	"[cur]rent", "                 ", "Prints the JVM being used in PATH, along with the release version defaults",
	"[i]nstall", "[arguments]      ", "Installs a new JVM",
	"[l]ist", "                    ", "Lists the names of all available JVMs",
	"[m]ove", "<name> <new_path>   ", "Moves the selected JVM to a different location",
	"[p]rint", "<name>             ", "Prints information on a JVM",
	"[reg]ister", "<name>          ", "Registers a JVM as its version default",
	"[r]e[m]ove", "[--keep] <name> ", "Removes a JVM (use --keep to remove the JVM without deleting it)",
	"[ren]ame", "<old> <new>       ", "Renames a JVM",
	"[u]se", "<name>               ", "Uses the given Java installation in PATH"
};
const unsigned int MAIN_HELP_COMMAND_COUNT = 11;

void logger_display_main_help(){
	if(logger_color_enabled){
		fwrite("\x1b[38;2;150;75;0mlatte\x1b[0m \x1b[38;2;255;255;255m[arguments]\x1b[0m\r\n\n\x1b[38;2;155;155;155mCommands:\x1b[0m\r\n", 1, 98, stdout);
		for(int i = 0; i < MAIN_HELP_COMMAND_COUNT; ++i){
			int length = strlen(MAIN_HELP_MESSAGES[i * 3]) + strlen(MAIN_HELP_MESSAGES[i * 3 + 1]) + strlen(MAIN_HELP_MESSAGES[i * 3 + 2]) + 73;
			char* buffer = calloc(length, 1);
			memcpy(buffer, "  \x1b[38;2;150;75;0mlatte \x1b[38;2;255;215;145m", 44);
			strcat(buffer, MAIN_HELP_MESSAGES[i * 3]);
			strcat(buffer, " \x1b[38;2;255;255;255m");
			strcat(buffer, MAIN_HELP_MESSAGES[i * 3 + 1]);
			strcat(buffer, "\x1b[0m - ");
			strcat(buffer, MAIN_HELP_MESSAGES[i * 3 + 2]);
			strcat(buffer, "\r\n");
			fwrite(buffer, 1, length, stdout);
			free(buffer);
		}
	}else{
		fwrite("latte [arguments]\r\n\nCommands:\r\n", 1, 32, stdout);
		for(int i = 0; i < MAIN_HELP_COMMAND_COUNT; ++i){
			int length = strlen(MAIN_HELP_MESSAGES[i * 3]) + strlen(MAIN_HELP_MESSAGES[i * 3 + 1]) + strlen(MAIN_HELP_MESSAGES[i * 3 + 2]) + 15;
			char* buffer = calloc(length, 1);
			memcpy(buffer, "  latte ", 9);
			strcat(buffer, MAIN_HELP_MESSAGES[i * 3]);
			strcat(buffer, " ");
			strcat(buffer, MAIN_HELP_MESSAGES[i * 3 + 1]);
			strcat(buffer, " - ");
			strcat(buffer, MAIN_HELP_MESSAGES[i * 3 + 2]);
			strcat(buffer, "\r\n");
			fwrite(buffer, 1, length, stdout);
			free(buffer);
		}
	}
}