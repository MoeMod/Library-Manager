#include <array>

#include "SaveData.h"

struct header_t
{
	int a = -1;
	char b[] = "图书信息管理系统文件";
} Header;


void SaveData_Write(std::ofstream &f)
{
	// header
	f << std::ofstream::binary;
	f.write(reinterpret_cast<const char *>(&Header), sizeof(Header));
	
	// Books
	f.write
}