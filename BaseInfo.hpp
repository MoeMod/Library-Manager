#pragma once

#include <iostream>
#include <string>

class CBaseInfo
{
protected:
	struct
	{
		int idx;
		std::string name;
	} m_info;
public:
	CBaseInfo(const std::string &name, int idx) : m_info({ idx, name }){}
	CBaseInfo(std::istream &in) { in >> *this; }
	const std::string &name() const
	{
		return m_info.name;
	}
	int idx() const
	{
		return m_info.idx;
	}
	friend std::istream & operator>>(std::istream &in, CBaseInfo &info)
	{
		int idx;
		char ch;
		in >> idx;
		in >> ch;
		std::string name;
		in >> name;
		info.m_info = { idx, name };
		return in;
	}
	friend std::ostream & operator<<(std::ostream &out, CBaseInfo &info)
	{
		out << info.idx();
		out << '\t';
		out << info.name();
		return out;
	}
};