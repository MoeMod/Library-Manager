#pragma once

#include "BaseBookInfo.hpp"
#include "BaseInfo.hpp"

class CAuthor : public CBaseBookInfo, public CBaseInfo
{
public:
	CAuthor(const std::string &name, int idx) : CBaseInfo(name, idx){}
};