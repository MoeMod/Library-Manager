#include <array>

#include "SaveData.h"

struct header_t
{
	int a = -1;
	char b[] = "ͼ����Ϣ����ϵͳ�ļ�";
} Header;


void SaveData_Write(std::ofstream &f)
{
	// header
	f << std::ofstream::binary;
	f.write(reinterpret_cast<const char *>(&Header), sizeof(Header));
	
	// Books
	f.write
}