#include <string>

namespace logger {
    void init();
    std::string dye(std::string text, int r, int g, int b);
    void fatal(std::string text);
    void info(std::string text);
    void log(std::string text);
    void warn(std::string text);
    void error(std::string text);
	void help(std::string command, std::string arguments, std::string purpose);
    bool start_progress(std::string task, std::string name, std::string initialStatus);
    void update_progress(std::string status);
    void close_progress(std::string end_status);
}