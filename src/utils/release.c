#include <string.h>
#include <stdlib.h>

char* find_property_value(char* buffer, char* key){
	int keylen = strlen(key);
	char* prefix = malloc(keylen + 3);
	prefix[keylen + 2] = 0;
	memcpy(prefix, key, keylen);
	strcat(prefix, "=\"");
	char* val = strstr(buffer, prefix);
	if(val == NULL){
		free(prefix);
		return NULL;
	}
	val = val + strlen(prefix) + 1;
	free(prefix);
	char* end = strstr(val, "\"");
	if(end == NULL){
		return NULL;
	}
	char* value = malloc(end - val + 1);
	value[end - val] = 0;
	memcpy(value, val, end - val);
	return value;
}