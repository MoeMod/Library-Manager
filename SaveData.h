#pragma once

#include <fstream>

class IBaseDataLink
{
public:
	virtual void output(std::ostream &out) = 0;
	virtual void input(std::istream &in) = 0;

	friend std::ostream & operator<<(std::ostream &out, IBaseDataLink &info)
	{
		info.output(out);
		return out;
	}
	friend std::istream & operator>>(std::istream &in, IBaseDataLink &info)
	{
		info.input(in);
		return in;
	}
};

template<class L, class R>
class CDataLink : public IBaseDataLink
{
	virtual void output(std::ostream &out) override 
	{
		
	}
	virtual void input(std::istream &in) override
	{

	}
};
