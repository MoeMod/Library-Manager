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
	// �鱾������ for(auto &book : this->Books()) ...
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
		return{ *this }; // ���������Ǹ�����POD����Ȼ�󷵻�
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
	~CBaseBookInfo() = default; // ������ָ�벻��Ҫ������
};