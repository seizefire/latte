#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "release.h"
#include "../filesystem/fileapi.h"

char* find_property_value(char* buffer, const char* key){
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

bool scan_for_implementation(char* buffer, u8* vendor, u8* implementation){
	char* implementor = find_property_value(buffer, "IMPLEMENTOR");
	*implementation = 0;
	if(implementor == NULL){
		char* build_type = find_property_value(buffer, "BUILD_TYPE");
		if(build_type == NULL || strcmp(build_type, "commercial") == 0){
			*vendor = 0;
			return true;
		}
	}else if(strcmp(implementor, "Oracle Corporation") == 0){
		*vendor = 0;
		return true;
	}else if(strcmp(implementor, "Eclipse Adoptium") == 0){
		*vendor = 2;
		char* variant = find_property_value(buffer, "JVM_VARIANT");
		if(variant != NULL && strcmp(variant, "Openj9") == 0){
			*implementation = 1;
		}
		return true;
	}else if(strcmp(implementor, "BellSoft") == 0){
		*vendor = 3;
		return true;
	}else if(strcmp(implementor, "Azul Systems, Inc.") == 0){
		*vendor = 4;
		return true;
	}
	return false;
}
bool scan_for_build_date(char* buffer, u8* date, u8* month, u16* year){
	char* build_date = find_property_value(buffer, "JAVA_VERSION_DATE");
	if(build_date == NULL){
		*date = 255;
		*month = 255;
		*year = 65535;
		return false;
	}else{
		int values[3] = {0, 0, 0};
		sscanf(build_date, "%d-%d-%d", &values[0], &values[1], &values[2]);
		*year = values[0];
		*month = values[1];
		*date = values[2];
		return true;
	}
}
bool scan_for_version(const char* version, u16* major_version, u16* minor_version, u16* patch_version, u16* build_number, u64* build_identifier){
	int length = strlen(version);
	*major_version = 65535;
	*minor_version = 65535;
	*patch_version = 65535;
	*build_number = 65535;
	// X.X.X.X
	sscanf(version, "%hu.%hu.%hu.%hu", major_version, minor_version, patch_version, build_number);
	if(*build_number != -1){
		*build_identifier = (((u64) *major_version) << 48) | (((u64) *minor_version) << 32) | (((u64) *patch_version) << 16) | (u64) *build_number;
		return true;
	}
	// X.X.X_X
	sscanf(version, "%hu.%hu.%hu_%hu", major_version, minor_version, patch_version, build_number);
	if(*build_number != -1){
		*build_identifier = (((u64) *major_version) << 48) | (((u64) *minor_version) << 32) | (((u64) *patch_version) << 16) | (u64) *build_number;
		return true;
	}
	// X.X.X+X
	sscanf(version, "%hu.%hu.%hu+%hu", major_version, minor_version, patch_version, build_number);
	if(*patch_version != -1){
		*build_identifier = (((u64) *major_version) << 48) | (((u64) *minor_version) << 32) | (((u64) *patch_version) << 16) | (u64) *build_number;
		return true;
	}
	// XuX-bX
	sscanf(version, "%huu%hu-b%hu", major_version, patch_version, build_number);
	if(*build_number != -1){
		*minor_version = 0;
		*build_identifier = (((u64) *major_version) << 48) | (((u64) *minor_version) << 32) | (((u64) *patch_version) << 16) | (u64) *build_number;
		return true;
	}
	*major_version = 65535;
	*minor_version = 65535;
	*patch_version = 65535;
	*build_number = 65535;
	return false;
}