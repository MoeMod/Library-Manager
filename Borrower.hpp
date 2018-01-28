#pragma once
#include <chrono>


using borrow_days_t = std::chrono::duration<int, std::ratio<3600 * 24> >;
struct CBorrowInfo
{
	std::shared_ptr<CBook> Book;
	int Borrower;
	std::chrono::system_clock::time_point ReturnTime;
};