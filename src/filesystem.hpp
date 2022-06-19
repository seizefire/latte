#include <fstream>

class bfinput_stream : public std::ifstream {
	public:
		using std::ifstream::basic_ifstream;
		std::string read_string(int size);
		unsigned char read_uint8();
		unsigned short read_uint16_le();
		unsigned int read_uint32_le();
};
class bfoutput_stream : public std::ofstream {
	public:
		using std::ofstream::basic_ofstream;
		void write_string(std::string data);
		void write_uint8(unsigned char data);
		void write_uint16_le(unsigned short data);
		void write_uint32_le(unsigned int data);
};