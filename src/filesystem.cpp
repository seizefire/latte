#include "filesystem.hpp"
#include <cstring>

std::string bfinput_stream::read_string(int size){
	char* data = new char[size];
	this->read(data, size);
	std::string str(data, data + size);
	delete[] data;
	return str;
}
unsigned char bfinput_stream::read_uint8(){
	char data[1];
	this->read(data, 1);
	return reinterpret_cast<unsigned char&>(data[0]);
}
unsigned short bfinput_stream::read_uint16_le(){
	char data[2];
	this->read(data, 2);
	unsigned char* fdata = reinterpret_cast<unsigned char*>(data);
	return fdata[0] + (fdata[1] << 8);
}
unsigned int bfinput_stream::read_uint32_le(){
	char data[4];
	this->read(data, 4);
	unsigned char* fdata = reinterpret_cast<unsigned char*>(data);
	return fdata[0] + (fdata[1] << 8) + (fdata[2] << 16) + (fdata[3] << 24);
}
void bfoutput_stream::write_string(std::string data){
	this->write(data.c_str(), data.length());
}
void bfoutput_stream::write_uint8(unsigned char data){
	char bytes[] = {reinterpret_cast<char&>(data)};
	this->write(bytes, 1);
}
void bfoutput_stream::write_uint16_le(unsigned short data){
	unsigned char bytes[2];
	bytes[0] = data & 255;
	bytes[1] = data >> 8;
	this->write(reinterpret_cast<char*>(bytes), 2);
}
void bfoutput_stream::write_uint32_le(unsigned int data){
	unsigned char bytes[4];
	bytes[0] = data & 255;
	bytes[1] = (data >> 8) & 255;
	bytes[2] = (data >> 16) & 255;
	bytes[3] = (data >> 24) & 255;
	this->write(reinterpret_cast<char*>(bytes), 4);
}