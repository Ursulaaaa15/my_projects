//������ � 4
//�������������� ������ � �����, �������� �� ���� �������� ������������
//����� ��������� : ����� ������������� ������, ������� ��������, ���� ������,
//���� ��������, �����, ��������, ��� �������, ������������, ����.����� ��
//����� ����� ������������� ������, �����, ������������ � ���� ��������
//(������������).
// 
//����� ����������: ����� �� ��������� �� ������ ���������� ���������. ��� �����
//���������� ������������� �������� ���������� �� ������� �����(� ��������� ��
//���� �������������).����������� ������������ ��������� ������ � ��������� �� ������ ������!
//4. ����������� ����������� ����������. ��� ������ ������ � �������� �
//������������(���������� � ����).
// 
//��������(���������� �� ������ ���������) ����� ����������� ������ ����� �� ��������� ��������(����� ���� ���
//����� �����), ����������� ����� ����� �� ���� �� ������������ ����(����
//����� �� �� �����), ������� �����(���� ��� � ���� �� �����).
// 
//������������ ����� ����������� �������������� ������ ���������(�����������, ��������,
//��������������) � ������ ����; ����������� ��������� ������� ������ ���� ��
//����, ��������� ������ ���� � ������� �������� ������(�������������
//���������� � ���������� ������ �� ���������� ����)
#include "Func.h"
#include "Library.h"
std::string file_def_name = { "file.txt" };

bool Menu(Library& library)
{
	int action = Input<int>("\n�������� ����� ������:\n[1] ������������\n[2] ��������\n[������ ������] ��������� ������ ���������\n-> ");
	switch (action)
	{
	case 1:
		while (LibrarianMenu(library)) {}
		break;
	case 2:
		SubscriberMenu(library);
		break;
	default:
		return false;
	}
	return true;
}

bool FileMenu(Library& library)
{
	std::string f;
	std::ifstream file_n;
	file_n.open(file_def_name, std::ios::in);
	file_n >> f;
	if (!library.AddFile(f))
	{
		std::ofstream file_n;
		file_n.open(file_def_name, std::ios_base::out);
		std::string new_file;
		auto choice = Input<int>("�������� ��������\n[1] ������� ����\n[2] ������� ����\n[������ ������] �����\n");
		switch (choice)
		{
		case 1:
			new_file = Input<std::string>("������� ��� �����\n->");
			if (library.AddFile(new_file))
			{
				file_n << new_file;
				break;
			}
			std::cout << "�� ������� ������� ����\n";
			return true;
		case 2:
		{
			std::string file_name = Input<std::string>("������� ��� �����\n->");
			std::ofstream file(file_name);
			if (!file.is_open())
			{
				std::cout << "�� ������� ������� ����\n";
				return true;
			}
			library.Clear();
			file.close();
			library.Save(file_name);
			library.AddFile(file_name);
			file_n << file_name;
			break;
		}
		default:
			return false;
		}
	}
	while (Menu(library)) {}
	return false;
}

//������������ ����� ����������� �������������� ������ ���������(�����������, ��������,
//��������������) � ������ ����; ����������� ��������� ������� ������ ���� ��
//����, ��������� ������ ���� � ������� �������� ������(�������������
//���������� � ���������� ������ �� ���������� ����)
//���� ������ �������� ��� ������������
bool LibrarianMenu(Library& library)
{
	int action = Input<int>("\n�������� ��������\n[1] ������������� ������ ���������\n[2] ������������� ������ ����\n\
[3] ���������� ������ ��������\n[4] �������� ������ ����(������, �� ������ + ���������� � ���������� ������ �� ���������� ����)\n[������ ������] �����\n-> ");
	switch (action)
	{
	case 1:
		EditingReaderListSubscribers(library);
		break;
	case 2:
		EditingReaderListBook(library);
		break;
	case 3:
		library.PrintHistory();
		break;
	case 4:
		ViewingBookList(library);
		break;
	default:
		return false;
	}
	return true;
}

//4. �������� ������ ����
bool ViewingBookList(Library& library)
{
	int action = Input<int>("\n�������� ��������\n[1] ���������� � �����������\n[2] ���������� � �����������\n");
	switch (action)
	{
	case 1:
		WithFiltering(library);
		break;
	case 2:
		ViewingData(library);
		break;
	default:
		return false;
	}
	return true;
}
//���������� � �����������
bool WithFiltering(Library& library)
{
	int action = Input<int>("\n����� ������ �� ������ �������������?\n[1] ������ ����\n[2] �� ������ ����\n[������ ������] �����\n-> ");
	switch (action)
	{
	case 1:
		SearchTaken(library);
		break;
	case 2:
		SearchNotTaken(library);
		break;
	default:
		return false;
	}
	return true;
}

//�� ������
void SearchNotTaken(Library& library)
{
	function<bool(const Book&)> com = ConditionNotTaken();
	if (!com)
		return;
	auto books = library.GetBooksNotTaken().FindAll(com);
	if (books.empty())
		std::cout << "����� �� �������\n\a";
	else
		for (auto const& i : books)
			PrintNotTakenBook(i);
}

std::function<bool(const Book&)> ConditionNotTaken()
{
	std::string string_field;
	int int_field;
	year y_filed;
	int action = Input<int>("�������� ������ ���������:\n[1] ��������\n[2] �����\n[3] ������������\n[4] ��� �������\n[5] ���������\n[�����] ������\n-> ");
	switch (action)
	{
	case 1:
		string_field = Input<std::string>("������� �������� �����: ", &Book::CheckTitle);
		return [string_field](const Book& book) {return book.GetTitle().compare(string_field) == 0; };
		break;
	case 2:
		string_field = Input<std::string>("������� ������: ", &Book::CheckAuthor);
		return [string_field](const Book& book) {return book.GetAuthor().compare(string_field) == 0; };
		break;
	case 3:
		string_field = Input<std::string>("������� �������� ������������: ", &Book::CheckPublisher);
		return [string_field](const Book& book) {return book.GetPublisher().compare(string_field) == 0; };
		break;
	case 4:
		y_filed = year{ Input<int>("������� ��� �������: ") };
		return [y_filed](const Book& book) {return book.GetPublicationYear() == y_filed; };
		break;
	case 5:
		int_field = Input<int>("������� ��������� �����: ", &Book::CheckPrice);
		return [int_field](const Book& book) {return book.GetPrice() == int_field; };
		break;
	default:
		return function<bool(const Book&)>();
	}
}

//������
void SearchTaken(Library& library)
{
	function<bool(const Book&)> com = ConditionTaken();
	if (!com)
		return;
	ostream_iterator<Book> it(std::cout, "\n>-------------------------\n");
	bool empty = false;
	library.GetBooksTaken().Subset(com, it, empty);
	if (empty)
		std::cout << "����� �� �������\n\a";
}

std::function<bool(const Book&)> ConditionTaken()
{
	std::string string_field;
	int int_field;
	year_month_day ymd_filed;
	year y_filed;
	int action = Input<int>("�������� ������ ���������:\n[1] ��������\n[2] �����\n[3] ������������\n[4] ��� �������\n[5] ���������\n\
[6] ���� ��������\n[7] ����� ������������� ������\n[�����] ������\n-> ");
	switch (action)
	{
	case 1:
		string_field = Input<std::string>("������� �������� �����: ", &Book::CheckTitle);
		return [string_field](const Book& book) {return book.GetTitle().compare(string_field) == 0; };
		break;
	case 2:
		string_field = Input<std::string>("������� ������: ", &Book::CheckAuthor);
		return [string_field](const Book& book) {return book.GetAuthor().compare(string_field) == 0; };
		break;
	case 3:
		string_field = Input<std::string>("������� �������� ������������: ", &Book::CheckPublisher);
		return [string_field](const Book& book) {return book.GetPublisher().compare(string_field) == 0; };
		break;
	case 4:
		y_filed = year{ Input<int>("������� ��� �������: ") };
		return [y_filed](const Book& book) {return book.GetPublicationYear() == y_filed; };
		break;
	case 5:
		int_field = Input<int>("������� ��������� �����: ", &Book::CheckPrice);
		return [int_field](const Book& book) {return book.GetPrice() == int_field; };
		break;
	case 6:
		ymd_filed = { floor<days>(system_clock::now()) };
		return [ymd_filed](const Book& book) {return book.GetReturnDate() < ymd_filed; };
		break;
	case 7:
		int_field = Input<int>("������� ����� ������������� ������: ", &Book::CheckLibraryCard);
		return [int_field](const Book& book) {return book.GetLibraryCard() == int_field; };
		break;
	default:
		return function<bool(const Book&)>();
	}
}

//���������� � �����������
bool ViewingData(Library& library)
{
	int action = Input<int>("\n����� ������ �� ������ �������������?\n[1] ������ ����\n[2] �� ������ ����\n[������ ������] �����\n-> ");
	switch (action)
	{
	case 1:
		SortListTaken(library);
		break;
	case 2:
		SortListNotTaken(library);
		break;
	default:
		return false;
	}
	return true;
}
//�� ������
void SortListNotTaken(Library& library)
{
	auto com = ComparatorNotTaken();
	if (!com)
		return;
	library.GetBooksNotTaken().Sort(com);
	library.PrintNotTaken();
	library.Save();
}

std::function<bool(const Book&, const Book&)> ComparatorNotTaken()
{
	int action = Input<int>("�������� ������ ���������:\n[1] ��������\n[2] �����\n[3] ������������\n[4] ��� �������\n[5] ���������\n[�����] ������\n-> ");
	switch (action)
	{
	case 1:
		return[](const Book& Abook, const Book& Rbook) {return Abook.GetTitle() < Rbook.GetTitle(); };
	case 2:
		return[](const Book& Abook, const Book& Rbook) {return Abook.GetAuthor() < Rbook.GetAuthor(); };
	case 3:
		return[](const Book& Abook, const Book& Rbook) {return Abook.GetPublisher() < Rbook.GetPublisher(); };
	case 4:
		return[](const Book& Abook, const Book& Rbook) {return Abook.GetPublicationYear() < Rbook.GetPublicationYear(); };
	case 5:
		return[](const Book& Abook, const Book& Rbook) {return Abook.GetPrice() < Rbook.GetPrice(); };
	}
	return std::function<bool(const Book&, const Book&)>();
}

//������
void SortListTaken(Library& library)
{
	auto com = ComparatorTaken();
	if (!com)
		return;
	library.GetBooksTaken().Sort(com);
	library.PrintTaken();
	library.Save();
}

std::function<bool(const Book&, const Book&)> ComparatorTaken()
{
	int action = Input<int>("�������� ������ ���������:\n[1] ��������\n[2] �����\n[3] ������������\n[4] ��� �������\n[5] ���������\n\
[6] ���� ��������\n[7] ����� ������������� ������\n[�����] ������\n-> ");
	switch (action)
	{
	case 1:
		return[](const Book& Abook, const Book& Rbook) {return Abook.GetTitle() < Rbook.GetTitle(); };
	case 2:
		return[](const Book& Abook, const Book& Rbook) {return Abook.GetAuthor() < Rbook.GetAuthor(); };
	case 3:
		return[](const Book& Abook, const Book& Rbook) {return Abook.GetPublisher() < Rbook.GetPublisher(); };
	case 4:
		return[](const Book& Abook, const Book& Rbook) {return Abook.GetPublicationYear() < Rbook.GetPublicationYear(); };
	case 5:
		return[](const Book& Abook, const Book& Rbook) {return Abook.GetPrice() < Rbook.GetPrice(); };
	case 6:
		return[](const Book& Abook, const Book& Rbook) {return Abook.GetReturnDate() < Rbook.GetReturnDate(); };
	case 7:
		return[](const Book& Abook, const Book& Rbook) {return Abook.GetLibraryCard() < Rbook.GetLibraryCard(); };
	}
	return std::function<bool(const Book&, const Book&)>();
}

//1
//���� �������������� ������ ���������
bool EditingReaderListSubscribers(Library& library)
{
	int action = Input<int>("\n�������� ��������\n[1] ���������������� ��������\n[2] ������� ��������\n\
[3] ������������� ��������\n[4] �������� ������ ���������\n[������ ������] �����\n-> ");
	switch (action)
	{
	case 1:
		library.AddingSubscriber();
		break;
	case 2:
		SubscriberRemoval(library);
		break;
	case 3:
		SubscriberRedacting(library);
		break;
	case 4:
		library.PrintSubscribers();
		break;
	default:
		return false;
	}
	return true;
}

//2.������� �������� ��������
void SubscriberRemoval(Library& library)
{
	std::list<Book>::iterator it = IterSubscriber("\n������� ����� ������������� ������ ��������, �������� ������ �������:\n", library);
	std::vector<Book>::iterator res;
	auto i = library.GetBooksTaken().Size();
	while (--i)
	{
		if (library.TakenSub(*it, res))
			library.GetBooksTaken().Del(*res);
	}
	library.DeleteSubscriber(it);
	std::cout << "������� ������!\n";
}

//3.������� �������������� ��������
void SubscriberRedacting(Library& library)
{
	std::list<Book>::iterator it = IterSubscriber("\n������� ����� ������������� ������ ��������, �������� ������ �������������:\n", library);
	library.RedactingSubscriber(*it);
	std::cout << "������� �������!\n";
}

//2
//���� �������������� ������ ����
bool EditingReaderListBook(Library& library)
{
	int action = Input<int>("\n�������� ��������\n[1] �������� �����\n[2] ������� �����\n\
[3] ������������� �����\n[������ ������] �����\n-> ");
	switch (action)
	{
	case 1:
		AddData(library);
		break;
	case 2:
		DeleteData(library);
		break;
	case 3:
		EditData(library);
		break;
	default:
		return false;
	}
	return true;
}

//1.���������� �����
bool AddData(Library& library)
{
	int action = Input<int>("\n����� ����� �� ������ ��������?\n[1] ������� ������� ��� ����\n[2] ������� ������� ��� �� ����\n[������ ������] �����\n-> ");
	switch (action)
	{
	case 1:
		AdingBookTaken(library);
		break;
	case 2:
		AdingBookNotTaken(library);
		break;
	default:
		return false;
	}
	return true;
}

//��������� ������
void AdingBookTaken(Library& library)
{
	std::cout << "������� ������ � �����:\n";
	Book book = EditingInputBooks(true, true);
	library.GetBooksTaken().Add(book);
	Book sub{ book.GetFIO(), book.GetLibraryCard()};
	if (std::find(library.GetSubscribers().begin(), library.GetSubscribers().end(), sub) == library.GetSubscribers().end())
		library.GetSubscribers().push_back(sub);
	std::cout << "������ � ����� ���������!\n";
	library.OperationsHistory("������� ���� ����� � ��������� ", book);
}
//��������� �� ������
void AdingBookNotTaken(Library& library)
{
	std::cout << "������� ������ � �����:\n";
	Book book = EditingInputBooks(false, false);
	library.GetBooksNotTaken().Add(book);
	std::cout << "������ � ����� ���������!\n";
	library.Save();
}

//2.�������� ������
bool DeleteData(Library& library)
{
	int action = Input<int>("\n����� ����� �� ������ �������?\n[1] ������� ������� ��� ����\n[2] ������� ������� ��� �� ����\n[������ ������] �����\n-> ");
	switch (action)
	{
	case 1:
		DelBookTaken(library);
		break;
	case 2:
		DelBookNotTaken(library);
		break;
	default:
		return false;
	}
	return true;
}
//������� ������
void DelBookTaken(Library& library)
{
	std::cout << "������� ������ � �����:\n";
	Book book = EditingInputBooks(false, true);
	library.GetBooksTaken().Del(book);
	std::cout << "������ � ����� �������!\n";
	library.Save();
}

//������� �� ������
void DelBookNotTaken(Library& library)
{
	std::cout << "������� ������ � �����:\n";
	Book book = EditingInputBooks(false, false);
	library.GetBooksNotTaken().Del(book);
	std::cout << "������ � ����� �������!\n";
	library.Save();
}

//3.������� ������
bool EditData(Library& library)
{
	int action = Input<int>("\n����� ����� �� ������ ��������?\n[1] ������� ������� ��� ����\n[2] ������� ������� ��� �� ����\n[������ ������] �����\n-> ");
	switch (action)
	{
	case 1:
		EditBookTaken(library);
		break;
	case 2:
		EditBookNotTaken(library);
		break;
	default:
		return false;
	}
	return true;
}

//�������� ������
void EditBookTaken(Library& library)
{
	std::cout << "������� ������ � �����, ������� ������ ��������:\n";
	Book books_old = EditingInputBooks(false, true);
	std::cout << "������� ����� ������ � �����:\n";
	Book books_new = EditingInputBooks(false, true);
	if (library.GetBooksTaken().Edit(books_old, books_new))
	{
		std::cout << "������ ������� �������!\n";
		library.Save();
	}
	else
		std::cout << "������ ����� �� �������:-(\n'\a'";
}

//�������� �� ������
void EditBookNotTaken(Library& library)
{
	std::cout << "������� ������ � �����, ������� ������ ��������:\n";
	Book books_old = EditingInputBooks(false, false);
	std::cout << "������� ����� ������ � �����:\n";
	Book books_new = EditingInputBooks(false, false);
	if (library.GetBooksNotTaken().Edit(books_old, books_new))
	{
		std::cout << "������ ������� �������!\n";
		library.Save();
	}
	else
		std::cout << "������ ����� �� �������:-(\n'\a'";
}

//���������� ������ � �����
Book EditingInputBooks(bool is_curent_date, bool sub)
{
	Book book;
	if (sub)
	{
		book.GetFIO() = Input<std::string>("������� ������� ��������: ", &Book::CheckFIO);
		book.GetLibraryCard() = Input<int>("������� ����� ������������� ������: ", &Book::CheckLibraryCard);
		book.GetDate() = is_curent_date ? year_month_day(floor<days>(system_clock::now())) : Input<year_month_day>("������� ���� ������: ", [&book](const year_month_day& ymd) {return ymd > book.GetDate(); });
		book.GetReturnDate() = Input<year_month_day>("������� ���� ��������: ", [&book](const year_month_day& ymd) {return ymd > book.GetDate(); });
	}
	book.GetTitle() = Input<std::string>("������� �������� �����: ", &Book::CheckTitle);
	book.GetAuthor() = Input<std::string>("������� ������: ", &Book::CheckAuthor);
	book.GetPublisher() = Input<std::string>("������� ������������: ", &Book::CheckPublisher);
	book.GetPublicationYear() = year{ Input<int>("������� ��� �������: ") };
	book.GetPrice() = Input<int>("������� ��������� �����: ", &Book::CheckPrice);
	return book;
}

//��������(���������� �� ������ ���������) ����� ����������� ������ ����� �� ��������� ��������(����� ���� ���
//����� �����), ����������� ����� ����� �� ���� �� ������������ ����(����
//����� �� �� �����), ������� �����(���� ��� � ���� �� �����).
//���� ��������
void SubscriberMenu(Library& library)
{
	if (library.GetSubscribers().empty())
	{
		std::cout << "��� ������������������ ���������:-(\n";
		return;
	}
	std::list<Book>::iterator it = IterSubscriber("\n������� ����� ������ ������������� ������ �� ������:\n", library);
	while (SubscriberAction(library, *it)) {}
}
//���� ������ �������� ��� ��������
bool SubscriberAction(Library& library, Book& it)
{
	int action = Input<int>("\n�������� ��������\n[1] ����� �����\n[2] ������� �����\n\
[3] ����� ����� �� ��������� �������� (����� ���� ��� ����� �����)\n[4] ������ �� ������\n[������ ������] �����\n-> ");
	switch (action)
	{
	case 1:
		BorrowBook(library, it);
		break;
	case 2:
		ReturnBook(library, it);
		break;
	case 3:
		Search(library);
		break;
	case 4:
		library.PrintNotTaken();
		break;
	default:
		return false;
	}
	return true;
}
//1.����� �����
void BorrowBook(Library& library, Book& it)
{
	std::vector<Book>::iterator res;
	std::cout << "������� ������ � �����\n";
	Book book = InputBooks(it, true);
	if (library.NotTaken(book, res)) //���������, ��� ����� �� �����
	{
		library.GetBooksTaken().Add(book);
		library.GetBooksNotTaken().Del(*res);
		std::cout << "����� ������!\n";
		library.OperationsHistory("The subscriber picked up a book with a title: ", book);
	}
	else
		std::cout << "����� �� �������!\n";
}
//2.������� �����
void ReturnBook(Library& library, Book& it)
{
	std::vector<Book>::iterator res;
	std::cout << "������� ������ � �����:\n";
	Book book = InputBooks(it, false);
	if (library.Taken(book, res)) //���������, ��� ����� �����
	{
		library.GetBooksNotTaken().Add(book);
		library.GetBooksTaken().Del(*res);
		std::cout << "����� ����������!\n";
		library.OperationsHistory("The subscriber returned the book with the title: ", book);
	}
	else
		std::cout << "� ��� ����� ����� ���!\n";
}

//���������� ������ � ����� ���� ��� � ����� �������������
Book InputBooks(Book it, bool is_curent_date)
{
	Book book;
	book.GetFIO() = it.GetFIO();
	book.GetTitle() = Input<std::string>("������� �������� �����: ", &Book::CheckTitle);
	book.GetAuthor() = Input<std::string>("������� ������: ", &Book::CheckAuthor);
	book.GetPublisher() = Input<std::string>("������� ������������: ", &Book::CheckPublisher);
	book.GetLibraryCard() = it.GetLibraryCard();
	book.GetPublicationYear() = year{ Input<int>("������� ��� �������: ") };
	book.GetPrice() = Input<int>("������� ��������� �����: ", &Book::CheckPrice);
	book.GetDate() = is_curent_date ? year_month_day(floor<days>(system_clock::now())) : Input<year_month_day>("������� ���� ������: ", [&book](const year_month_day& ymd) {return ymd > book.GetDate(); });
	book.GetReturnDate() = Input<year_month_day>("������� ���� ��������: ", [&book](const year_month_day& ymd) {return ymd > book.GetDate(); });
	return book;
}
//3.����� ����� �� ��������� �������� (����� ���� ��� ����� �����)
void Search(Library& library)
{
	for (auto const& i : library.SearchList(BookSearch()))
		PrintNotTakenBook(i);
}

//������� ������ � ��������� ���� ����������
list<function<bool(const Book&)>> BookSearch()
{
	int ac, desired_book_price;
	std::string desired_book_title, desired_book_author, desired_book_publisher;
	year desired_book_publication_year;
	list<function<bool(const Book&)>> search;
	ac = Input<int>("\n������ ������ �� ��������?\n[1] ��\n[2] ���\n", [](auto c) {return c == 1 || c == 2; });
	if (ac == 1)
	{
		desired_book_title = Input<std::string>("������� �������� �����: ", &Book::CheckTitle);
		//� ������ ��������� � ����� ������
		//����������� ��� ����������
		//� ������ ������ ��������� ��������� ���� ��������� ������ � ���� ����������
		search.push_back([desired_book_title](const Book& book) {return book.GetTitle() == desired_book_title; });
	}
	ac = Input<int>("\n������ ������ �� ������?\n[1] ��\n[2] ���\n", [](auto c) {return c == 1 || c == 2; });
	if (ac == 1)
	{
		desired_book_author = Input<std::string>("������� ������: ", &Book::CheckAuthor);
		search.push_back([desired_book_author](const Book& book) {return book.GetAuthor() == desired_book_author; });
	}
	ac = Input<int>("\n������ ������ ������������?\n[1] ��\n[2] ���\n", [](auto c) {return c == 1 || c == 2; });
	if (ac == 1)
	{
		desired_book_publisher = Input<std::string>("������� ������������: ", &Book::CheckPublisher);
		search.push_back([desired_book_publisher](const Book& book) {return book.GetPublisher() == desired_book_publisher; });
	}
	ac = Input<int>("\n������ ������ �� ���� ����������?\n[1] ��\n[2] ���\n", [](auto c) {return c == 1 || c == 2; });
	if (ac == 1)
	{
		desired_book_publication_year = year{ Input<int>("������� ��� ����������: ") };
		search.push_back([desired_book_publication_year](const Book& book) {return book.GetPublicationYear() == desired_book_publication_year; });
	}
	ac = Input < int>("\n������ ������ �� ��������� �����?\n[1] ��\n[2] ���\n", [](auto c) {return c == 1 || c == 2; });
	if (ac == 1)
	{
		desired_book_price = Input<int>("������� ��������� �����: ", &Book::CheckPrice);
		search.push_back([desired_book_price](const Book& book) {return book.GetPrice() == desired_book_price; });
	}
	return search;
}

//��� �������
// 
//������� ���������� ��������� �� ��������
std::list<Book>::iterator IterSubscriber(std::string messag, Library& library)
{
	std::list<Book>::iterator it;
	std::cout << messag;
	library.PrintSubscribers();
	do
	{
		int num = Input<int>("-> ");
		it = std::find_if(library.GetSubscribers().begin(), library.GetSubscribers().end(), [num](auto sub) {return sub.GetLibraryCard() == num; });
	} while (it == library.GetSubscribers().end());
	return it;
}
//������� ��� �����
bool File(Library& library)
{
	return library.AddFile(Input<std::string>("������� ��� �����\n-> "));
}

std::fstream InputFile(std::ios_base::openmode mode)
{
	std::fstream file;
	std::cout << "\n������� ��� ����� ��� ������\n->";
	std::string file_name;
	getline(std::cin, file_name);
	file.open(file_name, mode);
	while (!file)
	{
		getline(std::cin, file_name);
		file.open(file_name, mode);
	}
	return file;
}