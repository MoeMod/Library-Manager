#pragma once

#include <set>
#include <memory>

#include "util.hpp"

class CBook;

class CBaseBookInfo
{
protected:
	std::set<std::weak_ptr<CBook>, weak_ptr_less<CBook>> m_RelatedBookList;

public:
	// 书本遍历器 for(auto &book : this->Books()) ...
	struct
	{
		CBaseBookInfo &obj;
		auto begin() -> std::set<std::weak_ptr<CBook>, weak_ptr_less<CBook>>::iterator
		{
			return obj.m_RelatedBookList.begin();
		}
		auto end() -> std::set<std::weak_ptr<CBook>, weak_ptr_less<CBook>>::iterator
		{
			return obj.m_RelatedBookList.end();
		}
	} Books()
	{
		return{ *this }; // 构造上面那个匿名POD对象然后返回
	}
	void AddRelatedBook(std::shared_ptr<CBook> p)
	{
		m_RelatedBookList.insert(p);
	}
public:
	void OnBookDeleting(std::weak_ptr<CBook> p)
	{
		m_RelatedBookList.erase(p);
	}

public:
	~CBaseBookInfo() = default; // 用智能指针不需要虚析构
};