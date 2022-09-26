#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#include "jvm.h"
#include "release.h"

char* find_property_value(char* buffer, char* key){
	int keylen = strlen(key);
	char* prefix = malloc(keylen + 3);
	memset(prefix, 0, keylen + 3);
	memset(prefix, '\n', 1);
	strcat(prefix, key);
	strcat(prefix, "=");
	char* val = strstr(buffer, prefix);
	if(val == NULL){
		free(prefix);
		return NULL;
	}
	val = val + keylen + 2;
	if(val[0] == '"'){
		val = val + 1;
	}
	char* end = strstr(val, "\n");
	if(end == NULL){
		end = buffer + strlen(buffer);
	}
	if(*(end - 1) == '\r'){
		end = end - 1;
	}
	if(*(end - 1) == '"'){
		end = end - 1;
	}
	char* value = malloc(end - val + 1);
	value[end - val] = 0;
	memcpy(value, val, end - val);
	free(prefix);
	return value;
}

// Oracle Corporation, ????, Eclipse Adoptium, BellSoft, Azul Systems, Inc.
// HotSpot, OpenJ9

bool scan_for_implementation(struct jvm* entry, char* buffer){
	char* implementor = find_property_value(buffer, "IMPLEMENTOR");
	entry->implementation = 0;
	if(implementor == NULL){
		char* build_type = find_property_value(buffer, "BUILD_TYPE");
		if(strcmp(build_type, "commercial") == 0){
			entry->vendor = 0;
			return true;
		}
	}else if(strcmp(implementor, "Oracle Corporation") == 0){
		entry->vendor = 0;
		return true;
	}else if(strcmp(implementor, "Eclipse Adoptium") == 0){
		entry->vendor = 2;
		char* variant = find_property_value(buffer, "JVM_VARIANT");
		if(variant != NULL && strcmp(variant, "Openj9") == 0){
			entry->implementation = 1;
		}
		return true;
	}else if(strcmp(implementor, "BellSoft") == 0){
		entry->vendor = 3;
	}else if(strcmp(implementor, "Azul Systems, Inc.") == 0){
		entry->vendor = 4;
	}
	return false;
}
bool scan_for_build_date(struct jvm* entry, char* buffer){
	char* build_date = find_property_value(buffer, "JAVA_VERSION_DATE");
	if(build_date == NULL){
		entry->build_date = 255;
		entry->build_month = 255;
		entry->build_year = UINT16_MAX;
		return false;
	}else{
		int values[3] = {0, 0, 0};
		sscanf(build_date, "%d-%d-%d", &values[0], &values[1], &values[2]);
		entry->build_year = values[0];
		entry->build_month = values[1];
		entry->build_date = values[2];
		return true;
	}
}
bool scan_for_version(struct jvm* entry, char* version){
	int length = strlen(version);
	// X.X.X.X
	sscanf(version, "%hu.%hu.%hu.%hu", &entry->major_version, &entry->minor_version, &entry->patch_version, &entry->build_number);
	if(entry->build_number != -1){
		return true;
	}
	// X.X.X_X
	sscanf(version, "%hu.%hu.%hu_%hu", &entry->major_version, &entry->minor_version, &entry->patch_version, &entry->build_number);
	if(entry->build_number != -1){
		return true;
	}
	// X.X.X+X
	sscanf(version, "%hu.%hu.%hu+%hu", &entry->major_version, &entry->minor_version, &entry->patch_version, &entry->build_number);
	if(entry->patch_version != -1){
		return true;
	}
	// XuX-bX
	sscanf(version, "%huu%hu-b%hu", &entry->major_version, &entry->patch_version, &entry->build_number);
	if(entry->build_number != -1){
		entry->minor_version = 0;
		return true;
	}
	entry->major_version = UINT16_MAX;
	entry->minor_version = UINT16_MAX;
	entry->patch_version = UINT16_MAX;
	entry->build_number = UINT16_MAX;
	return true;
}
bool scan_for_alternate_version(struct jvm* entry, char* buffer){
	memset(entry->alt_version, 0, 32);
	if(entry->implementation > 0){
		char* jvm_version = find_property_value(buffer, "JVM_VERSION");
		if(jvm_version != NULL){
			strcpy(entry->alt_version, jvm_version);
			return true;
		}
	}
	return false;
}