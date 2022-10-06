#pragma once

#include <stdbool.h>

#include "jvm.h"

char* find_property_value(char* buffer, char* key);
bool scan_for_implementation(jvm* entry, char* buffer);
bool scan_for_build_date(jvm* entry, char* buffer);
bool scan_for_version(jvm* entry, char* version);
bool scan_for_alternate_version(jvm* entry, char* buffer);