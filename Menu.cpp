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

// ��������֮���Զ���ʾ��ͣ��ʾ
struct CPauser
{
	~CPauser() { system("pause"); }
};

void Menu_FuncNewBook()
{
	CPauser psr;

	cout << "������������";
	std::string BookName;
	cin >> BookName;

	auto p = Lib_FindBookByName(BookName);
	if (!p)
	{
		// û���ҵ���Ļ���ʼ���
		cout << "û���ڿ�����ҵ�" << BookName << "�����ڿ�ʼ������顣" << endl;
		p = Lib_NewBook(BookName);

		std::string buffer;
		int count = 0;
		cout << "�������뱾����ص����ߣ���Ctrl+Z������" << endl;
		while (cin >> buffer)
		{
			auto author = Lib_NewAuthor(buffer);
			Lib_LinkBookAndAuthor(p, author);
			cout << "�ɹ�������� " << buffer << " ����������롣" << endl;
			count++;
		}
		cout << "�����" << count << "�����ߡ�" << endl << endl;

		cin.clear();
		count = 0;
		cout << "�������뱾����صĳ����磬��Ctrl+Z������" << endl;
		while (cin >> buffer)
		{
			auto press = Lib_NewPress(buffer);
			Lib_LinkBookAndPress(p, press);
			cout << "�ɹ���ӳ����� " << buffer << " ����������롣" << endl;
			count++;
		}
		cout << "�����" << count << "�������硣" << endl << endl;
	}

	cout << "���������������";
	int num;
	cin.clear();
	cin >> num;

	p->Stock().inc(num);
	
	cout << "�ɹ��������[" << p->idx() << "]��" << p->name() << "��" << num << "�������ڹ��п��" << p->Stock().get() << "��" << endl;
	return;
}

void Menu_FuncClearStock()
{
	CPauser psr;

	cout << "������������";
	std::string BookName;
	cin >> BookName;

	auto p = Lib_FindBookByName(BookName);

	if (!p)
	{
		cout << "δ�ڿ�����ҵ�" << BookName << endl;
		return;
	}
	cout << "�Ƿ�Ҫɾ��[" << p->idx() << "]��" << p->name() << "��(���п��" << p->Stock().get() << "��)��[Y|N]";
	bool Input = GetInputYesNo();
	if (!Input)
	{
		cout << "������ȡ��" << endl;
		return;
	}

	Lib_DeleteBook(BookName);
	cout << "�����ɹ����" << endl;
	return;
}

void Menu_FuncBorrowBook()
{
	CPauser psr;

	cout << "������������";
	std::string BookName;
	cin >> BookName;

	auto p = Lib_FindBookByName(BookName);

	if (!p)
	{
		cout << "δ�ڿ�����ҵ�" << BookName << endl;
		return;
	}

	int borrowerid;
	cout << "�����������ID��";
	cin >> borrowerid;

	int days;
	cout << "���������������";
	cin >> days;
	try
	{
		Lib_BorrowBook(p, borrowerid, borrow_days_t(days));
	}
	catch (NoBookStock_e &)
	{
		cout << "Ŀ����Ŀû�п�棡" << endl;
	}
	catch (AlreadyBorrowed_e &)
	{
		cout << "�ý������ѽ��ı��飡" << endl;
	}
	cout << "�����ɹ���ɡ�" << endl;
}

void Menu_FuncReturnBook()
{
	CPauser psr;

	cout << "������������";
	std::string BookName;
	cin >> BookName;

	auto p = Lib_FindBookByName(BookName);

	if (!p)
	{
		cout << "δ�ڿ�����ҵ�" << BookName << endl;
		return;
	}

	int borrowerid;
	cout << "�����������ID��";
	cin >> borrowerid;

	try
	{
		Lib_ReturnBook(p, borrowerid);
	}
	catch (NoInformation_e &)
	{
		cout << "û���ҵ�Ŀ���¼��" << endl;
	}
	cout << "�����ɹ���ɡ�" << endl;
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
	cout << "[" << ref.idx() << "] ��" << ref.name() << "��" << endl;
	cout << "���ߣ�";
	for (auto author : ref.Authors())
	{
		cout << author->name() << " ";
	}
	cout << endl;
	cout << "�����磺";
	for (auto press : ref.Presses())
	{
		cout << press->name() << " ";
	}
	cout << endl;
}

void Menu_FuncSearchBookByName()
{
	CPauser psr;

	cout << "������������";
	std::string buffer;
	cin >> buffer;
	auto p = Lib_FindBookByName(buffer);
	if (!p)
	{
		cout << "δ�ҵ������" << endl;
		return;
	}
	return PrintBookInfo(*p);
}

template<class T>
void Menu_FuncSearchBookByRelationship()
{
	CPauser psr;

	cout << "�������ѯ���ݣ�";
	std::string buffer;
	cin >> buffer;
	auto pItem = GetItem<T>(buffer);
	if (!pItem)
	{
		cout << "δ�ҵ���" << buffer << "��" << endl;
		return;
	}
	int count = 0;
	for (auto pBook : pItem->Books()) // weak_ptr
	{
		PrintBookInfo(*pBook.lock());
		count++;
	}
	cout << "���ҵ�" << count << "�������" << endl;
	
}

void Menu_FuncSearchBookUnion()
{
	CPauser psr;

	cout << "������ؼ��ʣ�ctrl+z��������";
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
	cout << "���ҵ�" << count << "�������" << endl;
	return;
}

void Menu_SearchMethod()
{
	while (1)
	{
		ClearScreen();

		for (auto &str : {
			"a) ��������ѯ",
			"b) ������������ѯ",
			"c) ������������ѯ",
			"d) ��ϲ�ѯ",
			"0.�˳�"
		}) cout << str << endl;

		cout << "��ѡ��";
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
			"1.�ɱ����", 
			"2.������", 
			"3.����", 
			"4.�黹", 
			"5.��ѯͼ����Ϣ", 
			"0.�˳�" 
		}) cout << str << endl;
		
		cout << "��ѡ��";
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