#include <string.h>

#include "jvm.h"

void clean_jvm(struct jvm* entry){
	memset(entry->name, 0, 32);
	memset(entry->version, 0, 32);
	memset(entry->alt_version, 0, 32);
}
void flag_jvm_for_deletion(struct jvm* entry){
	entry->flags = entry->flags | 1;
}
void unflag_jvm_for_deletion(struct jvm* entry){
	entry->flags = entry->flags & 254;
}