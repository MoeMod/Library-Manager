#include <map>
#include "Book.hpp"
#include "Borrower.hpp"
#include "util.hpp"

#include "Library.h"

template<class T>
SearchTable<T> &SearchTableInstance()
{
	static SearchTable<T> x;
	return x;
}

using CBorrowInfoList = std::map<std::pair<std::shared_ptr<CBook>, int>, CBorrowInfo>;
CBorrowInfoList &BorrowInfoList()
{
	static CBorrowInfoList x;
	return x;
}

template<class T>
int IndexAllocator()
{
	static int index = 0;
	return index++;
}

template<class T>
std::shared_ptr<T> ItemFactory(const std::string &name)
{
	auto p = SearchTableInstance<T>().get(name);
	if (p)
		return p;
	int idx = IndexAllocator<T>();
	auto new_ptr = std::make_shared<T>(name, idx);
	SearchTableInstance<T>().set(name, new_ptr);
	return new_ptr;
}

std::shared_ptr<CBook> Lib_FindBookByName(const std::string &name)
{
	return SearchTableInstance<CBook>().get(name);
}

std::shared_ptr<CAuthor> Lib_FindAuthorByName(const std::string &name)
{
	return SearchTableInstance<CAuthor>().get(name);
}

std::shared_ptr<CPress> Lib_FindPressByName(const std::string &name)
{
	return SearchTableInstance<CPress>().get(name);
}

std::shared_ptr<CBook> Lib_NewBook(const std::string &name)
{
	return ItemFactory<CBook>(name);
}

std::shared_ptr<CAuthor> Lib_NewAuthor(const std::string &name)
{
	return ItemFactory<CAuthor>(name);
}

std::shared_ptr<CPress> Lib_NewPress(const std::string &name)
{
	return ItemFactory<CPress>(name);
}

void Lib_DeleteBook(const std::string &name)
{
	auto p = SearchTableInstance<CBook>().get(name);
	if (!p)
		return;
	for (auto &author : p->Authors())
	{
		author->OnBookDeleting(p);
	}
	for (auto &press : p->Presses())
	{
		press->OnBookDeleting(p);
	}
	return SearchTableInstance<CBook>().erase(name);
}

void Lib_LinkBookAndAuthor(std::shared_ptr<CBook> book, std::shared_ptr<CAuthor> author)
{
	book->AddAuthor(author);
	author->AddRelatedBook(book);
}

void Lib_LinkBookAndPress(std::shared_ptr<CBook> book, std::shared_ptr<CPress> press)
{
	book->AddPress(press);
	press->AddRelatedBook(book);
}

std::set<std::shared_ptr<CBook>> Lib_CombinedSearchUnion(std::set<std::string> keywords)
{
	std::set<std::shared_ptr<CBook>> result;
	for (const std::string &kw : keywords)
	{
		auto book = Lib_FindBookByName(kw);
		if (book)
			result.insert(book);
		auto author = Lib_FindAuthorByName(kw);
			if (author)
			for (auto wp : author->Books())
			{
				result.insert(wp.lock());
			}
		
		auto press = Lib_FindPressByName(kw);
		if (press)
			for (auto wp : press->Books())
			{
				result.insert(wp.lock());
			}
	}
	return result;
}

void Lib_BorrowBook(std::shared_ptr<CBook> book, int borrower, borrow_days_t days)
{
	if (book->Stock().get() <= 0)
		throw NoBookStock_e();
	
	CBorrowInfo info{ book, borrower, std::chrono::system_clock::now() + days };
	auto bookandborrower = std::make_pair(book, borrower);

	auto iter = BorrowInfoList().find(bookandborrower);
	if (iter != BorrowInfoList().end())
		throw AlreadyBorrowed_e();

	BorrowInfoList()[bookandborrower] = info;
	book->Stock().dec();
	return;
}

void Lib_ReturnBook(std::shared_ptr<CBook> book, int borrower)
{
	auto bookandborrower = std::make_pair(book, borrower);

	auto iter = BorrowInfoList().find(bookandborrower);
	if (iter == BorrowInfoList().end())
		throw NoInformation_e();

	BorrowInfoList().erase(iter);
	book->Stock().inc();
	return;
}