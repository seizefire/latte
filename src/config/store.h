#include "jvm.h"

extern jvm* jvms;
extern uint32_t jvm_count;
extern uint32_t next_jvm_id;
extern jvm* current_jvms;

extern uint16_t jdk_default_count;
extern uint16_t jre_default_count;
extern jvm** jdk_defualts;
extern jvm** jre_defualts;
extern jvm** normal_jvms;

void load_latte_config(char* path);
void save_latte_config(char* path);