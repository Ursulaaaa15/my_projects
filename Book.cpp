#include "Book.h"
#include "Func.h"
#include "Library.h"

std::istream& operator>>(istream& stream, Book& book)
{
	getline(stream, book.GetFIO());
	getline(stream, book.GetTitle());
	getline(stream, book.GetAuthor());
	getline(stream, book.GetPublisher());
	int y;
	stream >> book.GetLibraryCard() >> y >> book.GetPrice() >> book.GetDate() >> book.GetReturnDate();
	book.GetPublicationYear() = year{y};
	std::string str;
	if (!stream.eof())
		getline(stream, str);
	if (!book.IsValid())
		stream.setstate(std::ios::failbit);
	return stream;
}

std::ostream& operator<<(ostream& stream, const Book& book)
{
	stream << book.GetFIO() << '\n';
	stream << book.GetTitle() << '\n';
	stream << book.GetAuthor() << '\n';
	stream << book.GetPublisher() << '\n';
	stream << book.GetLibraryCard() << '\n';
	stream << (int)book.GetPublicationYear() << '\n';
	stream << book.GetPrice() << '\n';
	stream << (unsigned int)book.GetDate().day() << " " << (unsigned int)book.GetDate().month() << " " << (int)book.GetDate().year() << '\n';
	stream << (unsigned int)book.GetReturnDate().day() << " " << (unsigned int)book.GetReturnDate().month() << " " << (int)book.GetReturnDate().year();
	return stream;
}

//std::stringstream BookConsoleInput()
//{
//	Book book;
//	book.SetFIO() = Input<std::string>("������� ������� ��������: ", &Book::CheckFIO);
//	book.title = Input<std::string>("������� �������� �����: ", &Book::CheckTitle);
//	book.author = Input<std::string>("������� ������: ", &Book::CheckAuthor);
//	book.publisher = Input<std::string>("������� ������������: ", &Book::CheckPublisher);
//	book.library_card = Input<int>("������� ����� ������������� ������: ", &Book::CheckLibraryCard);
//	book.publication_year = year{ Input<int>("������� ��� �������: ") };
//	book.price = Input<int>("������� ��������� �����: ", &Book::CheckPrice);
//	book.date = Input<year_month_day>("������� ���� ������: ");
//	book.return_date = Input<year_month_day>("������� ���� ��������: ", [&book](const year_month_day& ymd) {return ymd > book.date; });
//	std::stringstream result;
//	result << book;
//	return result;
//}


istream& operator>>(istream& stream, year_month_day& date)
{
	unsigned int m, d;
	int y;
	stream >> d >> m >> y;
	date = { year{y},month{m},day{d} };
	if (!date.ok())
		stream.setstate(std::ios::failbit);
	return stream;
}