#pragma once

#include <stdbool.h>

#include "jvm.h"

char* find_property_value(char* buffer, char* key);
bool scan_for_implementation(struct jvm* entry, char* buffer);
bool scan_for_build_date(struct jvm* entry, char* buffer);
bool scan_for_version(struct jvm* entry, char* version);
bool scan_for_alternate_version(struct jvm* entry, char* buffer);