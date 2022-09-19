#include <map>
#include <string>
#include <stdint.h>

class jvm {
	public:
		std::string name;
		std::string path;
		uint16_t major_version;
		uint16_t minor_version;
		uint16_t patch_version;
		uint16_t build_number;
		uint8_t vendor;
		uint8_t implementation;
		uint8_t architecture;
		bool jre;
};

extern uint32_t next_id;
extern uint32_t current_id;

extern std::map<uint32_t, jvm> jvms;
extern std::map<uint16_t, uint32_t> jdk_defaults;
extern std::map<uint16_t, uint32_t> jre_defaults;

bool load_config();
bool save_config();

uint32_t get_id_from_name(std::string name);