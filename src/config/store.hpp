#pragma once

#include <vector>
#include "../jvm.hpp"

namespace config {
	extern u32 next_jvm_id;
	extern u32 current_jvm_id;

	extern std::vector<jvm> jvms;
	extern std::map<u16, jvm&> jdk_major_default_map;
	extern std::map<u16, jvm&> jre_major_default_map;
	extern std::map<u64, jvm&> jdk_build_default_map;
	extern std::map<u64, jvm&> jre_build_default_map;

	extern bool auto_set_build_default;
	extern bool auto_set_path_jvm;
	extern bool auto_set_version_default;
	extern bool console_color_enabled;
	extern bool registry_enabled;
	extern bool tracking_enabled;
	extern bool verbose_tracking_enabled;

	u32 get_id_from_name(std::string name);
	void load_latte_config(std::string path);
	void save_latte_config(std::string path);
}