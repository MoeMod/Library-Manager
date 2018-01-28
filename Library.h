#pragma once

#include <string>
#include <memory>

#include "Book.hpp"
#include "Borrower.hpp"

std::shared_ptr<CBook> Lib_FindBookByName(const std::string &name);
std::shared_ptr<CAuthor> Lib_FindAuthorByName(const std::string &name);
std::shared_ptr<CPress> Lib_FindPressByName(const std::string &name);

void Lib_DeleteBook(const std::string &name);

std::shared_ptr<CBook> Lib_NewBook(const std::string &name);
std::shared_ptr<CAuthor> Lib_NewAuthor(const std::string &name);
std::shared_ptr<CPress> Lib_NewPress(const std::string &name);

void Lib_LinkBookAndAuthor(std::shared_ptr<CBook> book, std::shared_ptr<CAuthor> author);
void Lib_LinkBookAndPress(std::shared_ptr<CBook> book, std::shared_ptr<CPress> press);

std::set<std::shared_ptr<CBook>> Lib_CombinedSearchUnion(std::set<std::string> keywords);

class NoBookStock_e{};
class AlreadyBorrowed_e{};
class NoInformation_e{};
void Lib_BorrowBook(std::shared_ptr<CBook> book, int borrower, borrow_days_t days) throw(NoBookStock_e, AlreadyBorrowed_e);
void Lib_ReturnBook(std::shared_ptr<CBook> book, int borrower) throw(NoInformation_e);