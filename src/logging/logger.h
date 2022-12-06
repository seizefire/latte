#pragma once

#include <stdbool.h>

#define SC_INFO 0
#define SC_WARN 2
#define SC_ERROR 4
#define SC_FATAL 6

extern bool logger_color_enabled;

const char* logger_dye(const char* text, unsigned char r, unsigned char g, unsigned char b);
#define logger_error(text) logger_log(SC_ERROR, text);
#define logger_fatal(text) logger_log(SC_FATAL, text);
#define logger_info(text) logger_log(SC_INFO, text);
void logger_log(char scope, const char* text);
void logger_logm(char scope, ...);
#define logger_warn(text) logger_log(SC_WARN, text);