#pragma once

#include "BaseBookInfo.hpp"
#include "BaseInfo.hpp"

class CPress : public CBaseBookInfo, public CBaseInfo
{
public:
	CPress(const std::string &name, int idx) : CBaseInfo(name, idx){}
};
