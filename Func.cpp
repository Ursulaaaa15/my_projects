//Задача № 4
//Информационная запись о книге, выданной на руки абоненту представляет
//собой структуру : номер читательского билета, фамилия абонента, дата выдачи,
//срок возврата, автор, название, год издания, издательство, цена.Поиск по
//полям номер читательского билета, автор, издательство и дата возврата
//(просроченные).
// 
//Общее требование: выход из программы не должен сбрасывать состояние. Для этого
//необходимо предусмотреть хранение информации на жестком диске(с загрузкой по
//мере необходимости).Обязательна внимательная обработка ошибок — программа не должна падать!
//4. Реализовать виртуальную библиотеку. Два режима работы — читатель и
//библиотекарь(выбирается в меню).
// 
//Читатель(выбирается из списка доступных) имеет возможность поиска книги по заданному критерию(любое поле или
//набор полей), возможность взять книгу на руки на определенный срок(если
//книга не на руках), вернуть книгу(если она у него на руках).
// 
//Библиотекарь имеет возможность редактирования списка читателей(регистрация, удаление,
//редактирование) и списка книг; возможность просмотра истории выдачи книг на
//руки, просмотра списка книг с текущим статусом каждой(предусмотреть
//фильтрацию и сортировку списка по выбранному полю)
#include "Func.h"
#include "Library.h"
std::string file_def_name = { "file.txt" };

bool Menu(Library& library)
{
	int action = Input<int>("\nВыберите режим работы:\n[1] Библиотекарь\n[2] Читатель\n[Другой символ] Завершить работу программы\n-> ");
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
		auto choice = Input<int>("Выберите действие\n[1] Выбрать файл\n[2] Создать файл\n[Другой символ] Выход\n");
		switch (choice)
		{
		case 1:
			new_file = Input<std::string>("Введите имя файла\n->");
			if (library.AddFile(new_file))
			{
				file_n << new_file;
				break;
			}
			std::cout << "Не удалось открыть файл\n";
			return true;
		case 2:
		{
			std::string file_name = Input<std::string>("Введите имя файла\n->");
			std::ofstream file(file_name);
			if (!file.is_open())
			{
				std::cout << "Не удалось создать файл\n";
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

//Библиотекарь имеет возможность редактирования списка читателей(регистрация, удаление,
//редактирование) и списка книг; возможность просмотра истории выдачи книг на
//руки, просмотра списка книг с текущим статусом каждой(предусмотреть
//фильтрацию и сортировку списка по выбранному полю)
//Меню выбора действий для библиотекаря
bool LibrarianMenu(Library& library)
{
	int action = Input<int>("\nВыберите действие\n[1] Редактировать список читателей\n[2] Редактировать список книг\n\
[3] Посмотреть исторю операций\n[4] Просмотр списка книг(взятые, не взятые + фильтрацию и сортировку списка по выбранному полю)\n[Другой символ] Выход\n-> ");
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

//4. Просмотр списка книг
bool ViewingBookList(Library& library)
{
	int action = Input<int>("\nВыберите действие\n[1] Посмотреть с фильтрацией\n[2] Посмотреть с сортировкой\n");
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
//Посмотреть с фильтрацией
bool WithFiltering(Library& library)
{
	int action = Input<int>("\nКакой список вы хотите отфильтровать?\n[1] Взятых книг\n[2] Не взятых книг\n[Другой символ] Выход\n-> ");
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

//не взятые
void SearchNotTaken(Library& library)
{
	function<bool(const Book&)> com = ConditionNotTaken();
	if (!com)
		return;
	auto books = library.GetBooksNotTaken().FindAll(com);
	if (books.empty())
		std::cout << "Книги не найдены\n\a";
	else
		for (auto const& i : books)
			PrintNotTakenBook(i);
}

std::function<bool(const Book&)> ConditionNotTaken()
{
	std::string string_field;
	int int_field;
	year y_filed;
	int action = Input<int>("Выберете способ сравнения:\n[1] Название\n[2] Автор\n[3] Издательство\n[4] Год издания\n[5] Стоимость\n[Любой] Отмена\n-> ");
	switch (action)
	{
	case 1:
		string_field = Input<std::string>("Введите название книги: ", &Book::CheckTitle);
		return [string_field](const Book& book) {return book.GetTitle().compare(string_field) == 0; };
		break;
	case 2:
		string_field = Input<std::string>("Введите автора: ", &Book::CheckAuthor);
		return [string_field](const Book& book) {return book.GetAuthor().compare(string_field) == 0; };
		break;
	case 3:
		string_field = Input<std::string>("Введите название издательства: ", &Book::CheckPublisher);
		return [string_field](const Book& book) {return book.GetPublisher().compare(string_field) == 0; };
		break;
	case 4:
		y_filed = year{ Input<int>("Введите год издания: ") };
		return [y_filed](const Book& book) {return book.GetPublicationYear() == y_filed; };
		break;
	case 5:
		int_field = Input<int>("Введите стоимость книги: ", &Book::CheckPrice);
		return [int_field](const Book& book) {return book.GetPrice() == int_field; };
		break;
	default:
		return function<bool(const Book&)>();
	}
}

//Взятые
void SearchTaken(Library& library)
{
	function<bool(const Book&)> com = ConditionTaken();
	if (!com)
		return;
	ostream_iterator<Book> it(std::cout, "\n>-------------------------\n");
	bool empty = false;
	library.GetBooksTaken().Subset(com, it, empty);
	if (empty)
		std::cout << "Книги не найдены\n\a";
}

std::function<bool(const Book&)> ConditionTaken()
{
	std::string string_field;
	int int_field;
	year_month_day ymd_filed;
	year y_filed;
	int action = Input<int>("Выберете способ сравнения:\n[1] Название\n[2] Автор\n[3] Издательство\n[4] Год издания\n[5] Стоимость\n\
[6] Дата возврата\n[7] Номер читательского билета\n[Любой] Отмена\n-> ");
	switch (action)
	{
	case 1:
		string_field = Input<std::string>("Введите название книги: ", &Book::CheckTitle);
		return [string_field](const Book& book) {return book.GetTitle().compare(string_field) == 0; };
		break;
	case 2:
		string_field = Input<std::string>("Введите автора: ", &Book::CheckAuthor);
		return [string_field](const Book& book) {return book.GetAuthor().compare(string_field) == 0; };
		break;
	case 3:
		string_field = Input<std::string>("Введите название издательства: ", &Book::CheckPublisher);
		return [string_field](const Book& book) {return book.GetPublisher().compare(string_field) == 0; };
		break;
	case 4:
		y_filed = year{ Input<int>("Введите год издания: ") };
		return [y_filed](const Book& book) {return book.GetPublicationYear() == y_filed; };
		break;
	case 5:
		int_field = Input<int>("Введите стоимость книги: ", &Book::CheckPrice);
		return [int_field](const Book& book) {return book.GetPrice() == int_field; };
		break;
	case 6:
		ymd_filed = { floor<days>(system_clock::now()) };
		return [ymd_filed](const Book& book) {return book.GetReturnDate() < ymd_filed; };
		break;
	case 7:
		int_field = Input<int>("Введите номер читательского билета: ", &Book::CheckLibraryCard);
		return [int_field](const Book& book) {return book.GetLibraryCard() == int_field; };
		break;
	default:
		return function<bool(const Book&)>();
	}
}

//Посмотреть с сортировкой
bool ViewingData(Library& library)
{
	int action = Input<int>("\nКакой список вы хотите отсортировать?\n[1] Взятых книг\n[2] Не взятых книг\n[Другой символ] Выход\n-> ");
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
//не взятые
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
	int action = Input<int>("Выберете способ сравнения:\n[1] Название\n[2] Автор\n[3] Издательство\n[4] Год издания\n[5] Стоимость\n[Любой] Отмена\n-> ");
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

//Взятые
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
	int action = Input<int>("Выберете способ сравнения:\n[1] Название\n[2] Автор\n[3] Издательство\n[4] Год издания\n[5] Стоимость\n\
[6] Дата возврата\n[7] Номер читательского билета\n[Любой] Отмена\n-> ");
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
//Меню редактирования списка читателей
bool EditingReaderListSubscribers(Library& library)
{
	int action = Input<int>("\nВыберите действие\n[1] Зарегестрировать абонента\n[2] Удалить абонента\n\
[3] Редактировать абонента\n[4] Просмотр списка абонентов\n[Другой символ] Выход\n-> ");
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

//2.Функция удаления абонента
void SubscriberRemoval(Library& library)
{
	std::list<Book>::iterator it = IterSubscriber("\nВведите номер читательского билета абонента, которого хотите удалить:\n", library);
	std::vector<Book>::iterator res;
	auto i = library.GetBooksTaken().Size();
	while (--i)
	{
		if (library.TakenSub(*it, res))
			library.GetBooksTaken().Del(*res);
	}
	library.DeleteSubscriber(it);
	std::cout << "Абонент удален!\n";
}

//3.Функция редактирования абонента
void SubscriberRedacting(Library& library)
{
	std::list<Book>::iterator it = IterSubscriber("\nВведите номер читательского билета абонента, которого хотите редактировать:\n", library);
	library.RedactingSubscriber(*it);
	std::cout << "Абонент изменет!\n";
}

//2
//Меню редактирования списка книг
bool EditingReaderListBook(Library& library)
{
	int action = Input<int>("\nВыберите действие\n[1] Добавить книгу\n[2] Удалить книгу\n\
[3] Редактировать книгу\n[Другой символ] Выход\n-> ");
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

//1.Добавление даных
bool AddData(Library& library)
{
	int action = Input<int>("\nКакую книгу вы хотите добавить?\n[1] Которую абонент уже взял\n[2] Которую абонент еще не взял\n[Другой символ] Выход\n-> ");
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

//Добавляем взятую
void AdingBookTaken(Library& library)
{
	std::cout << "Введите данные о книге:\n";
	Book book = EditingInputBooks(true, true);
	library.GetBooksTaken().Add(book);
	Book sub{ book.GetFIO(), book.GetLibraryCard()};
	if (std::find(library.GetSubscribers().begin(), library.GetSubscribers().end(), sub) == library.GetSubscribers().end())
		library.GetSubscribers().push_back(sub);
	std::cout << "Запись о книге добавлена!\n";
	library.OperationsHistory("Абонент взял книгу с названием ", book);
}
//Добавляем не взятую
void AdingBookNotTaken(Library& library)
{
	std::cout << "Введите данные о книге:\n";
	Book book = EditingInputBooks(false, false);
	library.GetBooksNotTaken().Add(book);
	std::cout << "Запись о книге добавлена!\n";
	library.Save();
}

//2.Удаление данных
bool DeleteData(Library& library)
{
	int action = Input<int>("\nКакую книгу вы хотите удалить?\n[1] Которую абонент уже взял\n[2] Которую абонент еще не взял\n[Другой символ] Выход\n-> ");
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
//Удаляем взятую
void DelBookTaken(Library& library)
{
	std::cout << "Введите данные о книге:\n";
	Book book = EditingInputBooks(false, true);
	library.GetBooksTaken().Del(book);
	std::cout << "Запись о книге удалена!\n";
	library.Save();
}

//Удаляем не взятую
void DelBookNotTaken(Library& library)
{
	std::cout << "Введите данные о книге:\n";
	Book book = EditingInputBooks(false, false);
	library.GetBooksNotTaken().Del(book);
	std::cout << "Запись о книге удалена!\n";
	library.Save();
}

//3.Измение данных
bool EditData(Library& library)
{
	int action = Input<int>("\nКакую книгу вы хотите изменить?\n[1] Которую абонент уже взял\n[2] Которую абонент еще не взял\n[Другой символ] Выход\n-> ");
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

//изменяем взятую
void EditBookTaken(Library& library)
{
	std::cout << "Введите данные о книге, которую хотите изменить:\n";
	Book books_old = EditingInputBooks(false, true);
	std::cout << "Введите новые данные о книге:\n";
	Book books_new = EditingInputBooks(false, true);
	if (library.GetBooksTaken().Edit(books_old, books_new))
	{
		std::cout << "Данные успешно измнены!\n";
		library.Save();
	}
	else
		std::cout << "Данная книга не найдена:-(\n'\a'";
}

//изменяем не взятую
void EditBookNotTaken(Library& library)
{
	std::cout << "Введите данные о книге, которую хотите изменить:\n";
	Book books_old = EditingInputBooks(false, false);
	std::cout << "Введите новые данные о книге:\n";
	Book books_new = EditingInputBooks(false, false);
	if (library.GetBooksNotTaken().Edit(books_old, books_new))
	{
		std::cout << "Данные успешно измнены!\n";
		library.Save();
	}
	else
		std::cout << "Данная книга не найдена:-(\n'\a'";
}

//Заполнение данных о книге
Book EditingInputBooks(bool is_curent_date, bool sub)
{
	Book book;
	if (sub)
	{
		book.GetFIO() = Input<std::string>("Введите фамилию абонента: ", &Book::CheckFIO);
		book.GetLibraryCard() = Input<int>("Введите номер читательского билета: ", &Book::CheckLibraryCard);
		book.GetDate() = is_curent_date ? year_month_day(floor<days>(system_clock::now())) : Input<year_month_day>("Введите дату выдачи: ", [&book](const year_month_day& ymd) {return ymd > book.GetDate(); });
		book.GetReturnDate() = Input<year_month_day>("Введите дату возврата: ", [&book](const year_month_day& ymd) {return ymd > book.GetDate(); });
	}
	book.GetTitle() = Input<std::string>("Введите название книги: ", &Book::CheckTitle);
	book.GetAuthor() = Input<std::string>("Введите автора: ", &Book::CheckAuthor);
	book.GetPublisher() = Input<std::string>("Введите издательство: ", &Book::CheckPublisher);
	book.GetPublicationYear() = year{ Input<int>("Введите год издания: ") };
	book.GetPrice() = Input<int>("Введите стоимость книги: ", &Book::CheckPrice);
	return book;
}

//Читатель(выбирается из списка доступных) имеет возможность поиска книги по заданному критерию(любое поле или
//набор полей), возможность взять книгу на руки на определенный срок(если
//книга не на руках), вернуть книгу(если она у него на руках).
//Меню абонента
void SubscriberMenu(Library& library)
{
	if (library.GetSubscribers().empty())
	{
		std::cout << "Нет зарегестрированных абонентов:-(\n";
		return;
	}
	std::list<Book>::iterator it = IterSubscriber("\nВведите номер своего читательского билета из списка:\n", library);
	while (SubscriberAction(library, *it)) {}
}
//Меню выбора действия для абонента
bool SubscriberAction(Library& library, Book& it)
{
	int action = Input<int>("\nВыберите действие\n[1] Взять книгу\n[2] Вернуть книгу\n\
[3] Поиск книги по заданному критерию (любое поле или набор полей)\n[4] Печать не взятых\n[Другой символ] Выход\n-> ");
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
//1.Взять книгу
void BorrowBook(Library& library, Book& it)
{
	std::vector<Book>::iterator res;
	std::cout << "Введите данные о книге\n";
	Book book = InputBooks(it, true);
	if (library.NotTaken(book, res)) //проверяем, что книга не взята
	{
		library.GetBooksTaken().Add(book);
		library.GetBooksNotTaken().Del(*res);
		std::cout << "Книга выдана!\n";
		library.OperationsHistory("The subscriber picked up a book with a title: ", book);
	}
	else
		std::cout << "Книге не найдена!\n";
}
//2.Вернуть книгу
void ReturnBook(Library& library, Book& it)
{
	std::vector<Book>::iterator res;
	std::cout << "Введите данные о книге:\n";
	Book book = InputBooks(it, false);
	if (library.Taken(book, res)) //проверяем, что книга взята
	{
		library.GetBooksNotTaken().Add(book);
		library.GetBooksTaken().Del(*res);
		std::cout << "Книга возвращена!\n";
		library.OperationsHistory("The subscriber returned the book with the title: ", book);
	}
	else
		std::cout << "У вас такой книги нет!\n";
}

//Заполнение данных о книге зная имя и номер читательского
Book InputBooks(Book it, bool is_curent_date)
{
	Book book;
	book.GetFIO() = it.GetFIO();
	book.GetTitle() = Input<std::string>("Введите название книги: ", &Book::CheckTitle);
	book.GetAuthor() = Input<std::string>("Введите автора: ", &Book::CheckAuthor);
	book.GetPublisher() = Input<std::string>("Введите издательство: ", &Book::CheckPublisher);
	book.GetLibraryCard() = it.GetLibraryCard();
	book.GetPublicationYear() = year{ Input<int>("Введите год издания: ") };
	book.GetPrice() = Input<int>("Введите стоимость книги: ", &Book::CheckPrice);
	book.GetDate() = is_curent_date ? year_month_day(floor<days>(system_clock::now())) : Input<year_month_day>("Введите дату выдачи: ", [&book](const year_month_day& ymd) {return ymd > book.GetDate(); });
	book.GetReturnDate() = Input<year_month_day>("Введите дату возврата: ", [&book](const year_month_day& ymd) {return ymd > book.GetDate(); });
	return book;
}
//3.Поиск книги по заданному критерию (любое поле или набор полей)
void Search(Library& library)
{
	for (auto const& i : library.SearchList(BookSearch()))
		PrintNotTakenBook(i);
}

//Функция выбора и сравнения всех параметров
list<function<bool(const Book&)>> BookSearch()
{
	int ac, desired_book_price;
	std::string desired_book_title, desired_book_author, desired_book_publisher;
	year desired_book_publication_year;
	list<function<bool(const Book&)>> search;
	ac = Input<int>("\nХотите искать по названию?\n[1] Да\n[2] Нет\n", [](auto c) {return c == 1 || c == 2; });
	if (ac == 1)
	{
		desired_book_title = Input<std::string>("Введите название книги: ", &Book::CheckTitle);
		//в список добавляем в конец лямбду
		//захватываем эту переменную
		//и внутри лямбды проверяем равенство поля аргумента лямбды и этой переменной
		search.push_back([desired_book_title](const Book& book) {return book.GetTitle() == desired_book_title; });
	}
	ac = Input<int>("\nХотите искать по автору?\n[1] Да\n[2] Нет\n", [](auto c) {return c == 1 || c == 2; });
	if (ac == 1)
	{
		desired_book_author = Input<std::string>("Введите автора: ", &Book::CheckAuthor);
		search.push_back([desired_book_author](const Book& book) {return book.GetAuthor() == desired_book_author; });
	}
	ac = Input<int>("\nХотите искать издательству?\n[1] Да\n[2] Нет\n", [](auto c) {return c == 1 || c == 2; });
	if (ac == 1)
	{
		desired_book_publisher = Input<std::string>("Введите издательство: ", &Book::CheckPublisher);
		search.push_back([desired_book_publisher](const Book& book) {return book.GetPublisher() == desired_book_publisher; });
	}
	ac = Input<int>("\nХотите искать по году публикации?\n[1] Да\n[2] Нет\n", [](auto c) {return c == 1 || c == 2; });
	if (ac == 1)
	{
		desired_book_publication_year = year{ Input<int>("Введите год публикации: ") };
		search.push_back([desired_book_publication_year](const Book& book) {return book.GetPublicationYear() == desired_book_publication_year; });
	}
	ac = Input < int>("\nХотите искать по стоимости книги?\n[1] Да\n[2] Нет\n", [](auto c) {return c == 1 || c == 2; });
	if (ac == 1)
	{
		desired_book_price = Input<int>("Введите стоимость книги: ", &Book::CheckPrice);
		search.push_back([desired_book_price](const Book& book) {return book.GetPrice() == desired_book_price; });
	}
	return search;
}

//Доп функции
// 
//Функция возвращает интератор на абонента
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
//Функции для файла
bool File(Library& library)
{
	return library.AddFile(Input<std::string>("Введите имя файла\n-> "));
}

std::fstream InputFile(std::ios_base::openmode mode)
{
	std::fstream file;
	std::cout << "\nВведите имя файла для работы\n->";
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