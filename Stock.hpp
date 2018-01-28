#pragma once

class CStock
{
protected:
	int m_amount;
public:
	CStock() : m_amount(0)
	{

	}
	int get()
	{
		return m_amount;
	}
	void inc(int n = 1)
	{
		m_amount += n;
	}
	void dec(int n = 1)
	{
		m_amount -= 1;
	}

};