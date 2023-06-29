//Задача № 4
//Информационная запись о книге, выданной на руки абоненту представляет
//собой структуру : номер читательского билета, фамилия абонента, дата выдачи,
//срок возврата, автор, название, год издания, издательство, цена.Поиск по
//полям номер читательского билета, автор, издательство и дата возврата
//(просроченные).

#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>
//#include "Func.h"

using namespace std::chrono;
using std::istream;
using std::ostream;

struct Book
{
public:
	static bool CheckFIO(const std::string& str)
	{
		return !str.empty() && std::all_of(str.begin(), str.end(), [](int x) { return (x >= 'а' && x <= 'я') || (x >= 'А' && x <= 'Я') || isalpha(x); });
	}

	static bool CheckTitle(const std::string& str)
	{
		return !str.empty();
	}
	static bool CheckAuthor(const std::string& str)
	{
		return !str.empty();
	}
	static bool CheckPublisher(const std::string& str)
	{
		return !str.empty();
	}
	static bool CheckLibraryCard(int library_card)
	{
		return library_card > 0;
	}
	static bool CheckPrice(double price)
	{
		return price > 0;
	}

private:
	std::string FIO; //фамилия абонента
	std::string title; //название книги
	std::string author; //автор
	std::string publisher;//издательство
	int library_card; //номер читательского билета
	year publication_year;//год издания
	double price;//цена
	year_month_day date; //дата выдачи
	year_month_day return_date;//срок возврата

public:
	Book() : FIO(), title(), author(), publisher(), library_card(), publication_year(), price(), date(), return_date(){}

	Book(const std::string& surname, int number) : FIO(surname), library_card(number), title(), author(), publisher(), publication_year(), price(), date(), return_date() {}

	Book(const std::string& surname, const std::string& ti, const std::string& au, const std::string& pu, int lib, const year& y, double p, const year_month_day& d, const year_month_day& rd) :
		FIO(surname), library_card(lib),
		title(ti), author(au), publisher(pu),
		publication_year(y), price(p), date(d), return_date(rd) {}

	Book(const std::string& ti, const std::string& au, const std::string& pu, const year& y, double p) :
		FIO(), library_card(),
		title(ti), author(au), publisher(pu),
		publication_year(y), price(p), date(), return_date() {}

	const std::string& SetFIO(const std::string& str)
	{
		FIO = str;
	}
	const std::string& SetTitle(const std::string& str)
	{
		FIO = str;
	}
	const std::string& SetAuthor(const std::string& str)
	{
		author = str;
	}
	const std::string& SetPublisher(const std::string& str)
	{
		publisher = str;
	}
	int SetLibraryCard(int x)
	{
		library_card = x;
	}
	const year& SetPublicationYear(const year& y)
	{
		publication_year = y;
	}
	double SetPrice(double x)
	{
		price = x;
	}
	const year_month_day& SetDate(const year_month_day& ymd)
	{
		date = ymd;
	}
	const year_month_day& SetReturnDate(const year_month_day& ymd)
	{
		return_date = ymd;
	}

	std::string& GetFIO()
	{
		return FIO;
	}
	std::string& GetTitle()
	{
		return title;
	}
	std::string& GetAuthor()
	{
		return author;
	}
	std::string& GetPublisher()
	{
		return publisher;
	}
	int& GetLibraryCard()
	{
		return library_card;
	}
	year& GetPublicationYear()
	{
		return publication_year;
	}
	double& GetPrice()
	{
		return price;
	}
	year_month_day& GetDate()
	{
		return date;
	}
	year_month_day& GetReturnDate()
	{
		return return_date;
	}

	const std::string& GetFIO() const
	{
		return FIO;
	}
	const std::string& GetTitle() const
	{
		return title;
	}
	const std::string& GetAuthor() const
	{
		return author;
	}
	const std::string& GetPublisher() const
	{
		return publisher;
	}
	const int& GetLibraryCard() const
	{
		return library_card;
	}
	const year& GetPublicationYear() const
	{
		return publication_year;
	}
	const double& GetPrice() const
	{
		return price;
	}
	const year_month_day& GetDate() const
	{
		return date;
	}
	const year_month_day& GetReturnDate() const
	{
		return return_date;
	}

	auto operator<=>(const Book& other) const = default; //это автоматически генерируемый оператор сравнения. это и больше и меньше и равно

	bool IsValid() const
	{
		return CheckFIO(FIO) && CheckTitle(title) && CheckAuthor(author) && CheckPublisher(publisher) &&
			CheckLibraryCard(library_card) && publication_year.ok() && CheckPrice(price) && date.ok() && return_date.ok() && return_date > date;
	}
};

istream& operator>>(istream& stream, year_month_day& date);

istream& operator>>(istream& stream, Book& book);

ostream& operator<<(ostream& stream, const Book& book);

std::stringstream BookConsoleInput();