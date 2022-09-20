#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

char* find_property_value(char* buffer, char* key){
	int keylen = strlen(key);
	char* prefix = malloc(keylen + 3);
	memset(prefix, 0, keylen + 3);
	memcpy(prefix, key, keylen);
	strcat(prefix, "=\"");
	char* val = strstr(buffer, prefix);
	if(val == NULL){
		free(prefix);
		return NULL;
	}
	val = val + keylen + 2;
	char* end = strstr(val, "\"");
	if(end == NULL){
		free(prefix);
		return NULL;
	}
	char* value = malloc(end - val + 1);
	value[end - val] = 0;
	memcpy(value, val, end - val);
	free(prefix);
	return value;
}
int* scan_version_string(char* version){
	static int values[4] = {USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX};
	int length = strlen(version);
	// X.X.X.X
	sscanf(version, "%d.%d.%d.%d", &values[0], &values[1], &values[2], &values[3]);
	if(values[3] != -1){
		return values;
	}
	// X.X.X+X
	sscanf(version, "%d.%d.%d+%d", &values[0], &values[1], &values[2], &values[3]);
	if(values[2] != -1){
		return values;
	}
	// XuX-bX
	sscanf(version, "%du%d-b%d", &values[0], &values[2], &values[3]);
	if(values[3] != -1){
		values[1] = 0;
		return values;
	}
	return values;
}