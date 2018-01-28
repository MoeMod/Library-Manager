#include <iostream>
#include <string>
#include <conio.h>

#include "Library.h"

using namespace std;

inline void ClearScreen()
{
	system("cls");
}

inline char GetCharFromKeyboard()
{
	return getch();
}

inline bool IsInRange(int x, int iMin, int iMax)
{
	return x >= iMin && x <= iMax;
}

inline bool IsCharNumber(char ch)
{
	return IsInRange(ch, '0', '9');
}

inline int Char2Number(char ch)
{
	return ch - '0';
}

int __fastcall GetInputNumber(int iMin, int iMax)
{
	int num = -1;
	do
	{
		char ch = '\0';
		while (!IsCharNumber(ch = GetCharFromKeyboard())){}
		num = Char2Number(ch);
	} while (!IsInRange(num, iMin, iMax));
	cin.clear();
	
	return num;
}

template<class T, class...Args>
inline bool IsInSequence(T what, T first, Args...args)
{
	if (what == first)
		return true;
	return IsInSequence(what, args...);
}
template<class T>
inline bool IsInSequence(T what)
{
	return false;
}

template<class...chars>
inline char GetRestrictedInput(char first, chars...args)
{
	char ch = '\0';
	char ret;
	while (ch = GetCharFromKeyboard())
	{
		if (IsInSequence(ch, first, args...))
		{
			ret = ch;
			break;
		}
	}
	cin.clear();
	return ret;
}

bool GetInputYesNo()
{
	char ch = GetRestrictedInput('y', 'Y', 'n', 'N');
	return ch == 'y' || ch == 'Y';
}

// 函数结束之后自动显示暂停提示
struct CPauser
{
	~CPauser() { system("pause"); }
};

void Menu_FuncNewBook()
{
	CPauser psr;

	cout << "请输入书名：";
	std::string BookName;
	cin >> BookName;

	auto p = Lib_FindBookByName(BookName);
	if (!p)
	{
		// 没有找到书的话开始添加
		cout << "没有在库存中找到" << BookName << "，现在开始添加新书。" << endl;
		p = Lib_NewBook(BookName);

		std::string buffer;
		int count = 0;
		cout << "请输入与本书相关的作者，按Ctrl+Z结束。" << endl;
		while (cin >> buffer)
		{
			auto author = Lib_NewAuthor(buffer);
			Lib_LinkBookAndAuthor(p, author);
			cout << "成功添加作者 " << buffer << " ，请继续输入。" << endl;
			count++;
		}
		cout << "已添加" << count << "名作者。" << endl << endl;

		cin.clear();
		count = 0;
		cout << "请输入与本书相关的出版社，按Ctrl+Z结束。" << endl;
		while (cin >> buffer)
		{
			auto press = Lib_NewPress(buffer);
			Lib_LinkBookAndPress(p, press);
			cout << "成功添加出版社 " << buffer << " ，请继续输入。" << endl;
			count++;
		}
		cout << "已添加" << count << "个出版社。" << endl << endl;
	}

	cout << "请输入添加数量：";
	int num;
	cin.clear();
	cin >> num;

	p->Stock().inc(num);
	
	cout << "成功向库存添加[" << p->idx() << "]《" << p->name() << "》" << num << "本，现在共有库存" << p->Stock().get() << "本" << endl;
	return;
}

void Menu_FuncClearStock()
{
	CPauser psr;

	cout << "请输入书名：";
	std::string BookName;
	cin >> BookName;

	auto p = Lib_FindBookByName(BookName);

	if (!p)
	{
		cout << "未在库存中找到" << BookName << endl;
		return;
	}
	cout << "是否要删除[" << p->idx() << "]《" << p->name() << "》(共有库存" << p->Stock().get() << "本)？[Y|N]";
	bool Input = GetInputYesNo();
	if (!Input)
	{
		cout << "操作已取消" << endl;
		return;
	}

	Lib_DeleteBook(BookName);
	cout << "操作成功完成" << endl;
	return;
}

void Menu_FuncBorrowBook()
{
	CPauser psr;

	cout << "请输入书名：";
	std::string BookName;
	cin >> BookName;

	auto p = Lib_FindBookByName(BookName);

	if (!p)
	{
		cout << "未在库存中找到" << BookName << endl;
		return;
	}

	int borrowerid;
	cout << "请输入借阅者ID：";
	cin >> borrowerid;

	int days;
	cout << "请输入借阅天数：";
	cin >> days;
	try
	{
		Lib_BorrowBook(p, borrowerid, borrow_days_t(days));
	}
	catch (NoBookStock_e &)
	{
		cout << "目标书目没有库存！" << endl;
	}
	catch (AlreadyBorrowed_e &)
	{
		cout << "该借阅者已借阅本书！" << endl;
	}
	cout << "操作成功完成。" << endl;
}

void Menu_FuncReturnBook()
{
	CPauser psr;

	cout << "请输入书名：";
	std::string BookName;
	cin >> BookName;

	auto p = Lib_FindBookByName(BookName);

	if (!p)
	{
		cout << "未在库存中找到" << BookName << endl;
		return;
	}

	int borrowerid;
	cout << "请输入借阅者ID：";
	cin >> borrowerid;

	try
	{
		Lib_ReturnBook(p, borrowerid);
	}
	catch (NoInformation_e &)
	{
		cout << "没有找到目标记录！" << endl;
	}
	cout << "操作成功完成。" << endl;
}

template<class T>
inline std::shared_ptr<T> GetItem(const std::string &name);
template<>
inline std::shared_ptr<CBook> GetItem<CBook>(const std::string &name)
{
	return Lib_FindBookByName(name);
}
template<>
inline std::shared_ptr<CAuthor> GetItem<CAuthor>(const std::string &name)
{
	return Lib_FindAuthorByName(name);
}
template<>
inline std::shared_ptr<CPress> GetItem<CPress>(const std::string &name)
{
	return Lib_FindPressByName(name);
}

void PrintBookInfo(const CBook &ref)
{
	cout << "[" << ref.idx() << "] 《" << ref.name() << "》" << endl;
	cout << "作者：";
	for (auto author : ref.Authors())
	{
		cout << author->name() << " ";
	}
	cout << endl;
	cout << "出版社：";
	for (auto press : ref.Presses())
	{
		cout << press->name() << " ";
	}
	cout << endl;
}

void Menu_FuncSearchBookByName()
{
	CPauser psr;

	cout << "请输入书名：";
	std::string buffer;
	cin >> buffer;
	auto p = Lib_FindBookByName(buffer);
	if (!p)
	{
		cout << "未找到结果。" << endl;
		return;
	}
	return PrintBookInfo(*p);
}

template<class T>
void Menu_FuncSearchBookByRelationship()
{
	CPauser psr;

	cout << "请输入查询内容：";
	std::string buffer;
	cin >> buffer;
	auto pItem = GetItem<T>(buffer);
	if (!pItem)
	{
		cout << "未找到《" << buffer << "》" << endl;
		return;
	}
	int count = 0;
	for (auto pBook : pItem->Books()) // weak_ptr
	{
		PrintBookInfo(*pBook.lock());
		count++;
	}
	cout << "共找到" << count << "条结果。" << endl;
	
}

void Menu_FuncSearchBookUnion()
{
	CPauser psr;

	cout << "请输入关键词（ctrl+z结束）：";
	std::string buffer;
	std::set<std::string> keywords;
	while (cin >> buffer)
	{
		keywords.insert(buffer);
	}
	auto result = Lib_CombinedSearchUnion(keywords);
	int count = 0;
	for (auto p : result)
	{
		PrintBookInfo(*p);
		count++;
	}
	cout << "共找到" << count << "条结果。" << endl;
	return;
}

void Menu_SearchMethod()
{
	while (1)
	{
		ClearScreen();

		for (auto &str : {
			"a) 按书名查询",
			"b) 按作者姓名查询",
			"c) 按出版社名查询",
			"d) 组合查询",
			"0.退出"
		}) cout << str << endl;

		cout << "请选择：";
		char chInput = GetRestrictedInput('a', 'A', 'b', 'B', 'c', 'C', 'd', 'D', '0');
		cout << chInput << endl;
		switch (chInput)
		{
		case 'a':
		case 'A':
			Menu_FuncSearchBookByName();
			break;
		case 'b':
		case 'B':
			Menu_FuncSearchBookByRelationship<CAuthor>();
			break;
		case 'c':
		case 'C':
			Menu_FuncSearchBookByRelationship<CPress>();
			break;
		case 'd':
		case 'D':
			Menu_FuncSearchBookUnion();
			break;
		case '0':
			return;
		}
	}
}

void Menu_MainMenu()
{
	while (1)
	{
		ClearScreen();

		for (auto &str : { 
			"1.采编入库", 
			"2.清除库存", 
			"3.借阅", 
			"4.归还", 
			"5.查询图书信息", 
			"0.退出" 
		}) cout << str << endl;
		
		cout << "请选择：";
		int iInput = GetInputNumber(0, 5);
		cout << iInput << endl;
		switch (iInput)
		{
		case 1:
			Menu_FuncNewBook();
			break;
		case 2:
			Menu_FuncClearStock();
			break;
		case 3:
			Menu_FuncBorrowBook();
			break;
		case 4:
			Menu_FuncReturnBook();
			break;
		case 5:
			Menu_SearchMethod();
			break;
		case 0:
			return;
		}
	}
}