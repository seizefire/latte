#include "logger.h"

#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

bool logger_color_enabled = true;

const char* headers[] = {"latte [", "\x1b[38;2;150;75;0mlatte\x1b[0m \x1b[38;2;0;255;255m["};
const char* scope_headers[] = {"info ", "\x1b[38;2;0;135;255minfo\x1b[0m \x1b[38;2;255;255;255m", "warn ", "\x1b[38;2;205;205;0mwarn\x1b[0m \x1b[38;2;255;255;55m", "error ", "\x1b[38;2;255;0;0merror\x1b[0m \x1b[38;2;255;105;105m", "fatal ", "\x1b[38;2;255;85;0mfatal\x1b[0m \x1b[38;2;255;155;70m"};
const char* endings[] = {"\r\n", "\x1b[0m\r\n"};

char* timestamp(){
	time_t secs;
	time(&secs);
	char* buffer = calloc(20, 1);
	strftime(buffer, 20, "%Y-%m-%dT%H:%M:%S", localtime(&secs));
	return buffer;
}
const char* logger_dye(const char* text, unsigned char r, unsigned char g, unsigned char b){
	if(logger_color_enabled == false){
		return text;
	}
	char buffer[12];
	memset(buffer, 0, 12);
	itoa(r, buffer, 10);
	itoa(g, buffer + 4, 10);
	itoa(b, buffer + 8, 10);
	int length = strlen(text) + strlen(buffer) + strlen(buffer + 4) + strlen(buffer + 8) + 15;
	char* string = calloc(length, 1);
	strcpy(string, "\x1b[38;2;");
	strcat(string, buffer);
	strcat(string, ";");
	strcat(string, buffer + 4);
	strcat(string, ";");
	strcat(string, buffer + 8);
	strcat(string, "m");
	strcat(string, text);
	strcat(string, "\x1b[0m");
	return (const char*) string;
}
void logger_log(char scope, const char* text){
	const char* header = headers[logger_color_enabled];
	const char* scope_header = scope_headers[scope | logger_color_enabled];
	const char* ending = endings[logger_color_enabled];
	char* stamp = timestamp();
	int length = strlen(header) + strlen(stamp) + strlen(scope_header) + strlen(text) + strlen(ending) + 3;
	char* full_buffer = calloc(length, 1);
	strcpy(full_buffer, header);
	strcat(full_buffer, stamp);
	strcat(full_buffer, "] ");
	strcat(full_buffer, scope_header);
	strcat(full_buffer, text);
	strcat(full_buffer, ending);
	fwrite(full_buffer, 1, length, stdout);
	free(stamp);
	free(full_buffer);
}
void logger_logm(char scope, ...){
	va_list length_list;
	va_list arg_list;
	va_start(length_list, scope);
	va_copy(arg_list, length_list);
	const char* header = headers[logger_color_enabled];
	const char* scope_header = scope_headers[scope | logger_color_enabled];
	const char* ending = endings[logger_color_enabled];
	char* stamp = timestamp();
	int length = strlen(header) + strlen(stamp) + strlen(scope_header) + strlen(ending) + 3;
	int arg_count = 0;
	while(true){
		const char* arg = va_arg(length_list, const char*);
		if(arg == NULL){
			break;
		}
		length += strlen(arg);
		++arg_count;
	}
	va_end(length_list);
	if(arg_count == 0){
		free(stamp);
		return;
	}
	char* full_buffer = calloc(length, 1);
	strcpy(full_buffer, header);
	strcat(full_buffer, stamp);
	strcat(full_buffer, "] ");
	strcat(full_buffer, scope_header);
	for(int i = 0; i < arg_count; ++i){
		strcat(full_buffer, va_arg(arg_list, const char*));
	}
	strcat(full_buffer, ending);
	fwrite(full_buffer, 1, length, stdout);
	free(stamp);
	free(full_buffer);
	va_end(arg_list);
}