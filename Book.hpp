#pragma once

#include <set>
#include <memory>
#include <array>

#include "BaseInfo.hpp"

#include "Author.hpp"
#include "Press.hpp"
#include "Stock.hpp"

class CBook : public CBaseInfo
{
private:
	std::set<std::shared_ptr<CAuthor>> m_AuthorList;
	std::set<std::shared_ptr<CPress>> m_PressList;
	CStock m_BookStock;
public:
	CBook(const std::string &name, int idx) : CBaseInfo(name, idx){}
	~CBook(){}

	struct
	{
		const CBook &obj;
		auto begin() -> std::set<std::shared_ptr<CAuthor>>::iterator
		{
			return obj.m_AuthorList.begin();
		}
		auto end() -> std::set<std::shared_ptr<CAuthor>>::iterator
		{
			return obj.m_AuthorList.end();
		}
	} Authors() const
	{
		return{ *this };
	}
	struct
	{
		const CBook &obj;
		auto begin() -> std::set<std::shared_ptr<CPress>>::iterator
		{
			return obj.m_PressList.begin();
		}
		auto end() -> std::set<std::shared_ptr<CPress>>::iterator
		{
			return obj.m_PressList.end();
		}
	} Presses() const 
	{
		return{ *this };
	}

	CStock &Stock()
	{
		return m_BookStock;
	}

	void AddAuthor(std::shared_ptr<CAuthor> p)
	{
		m_AuthorList.insert(p);
	}
	void AddPress(std::shared_ptr<CPress> p)
	{
		m_PressList.insert(p);
	}
};