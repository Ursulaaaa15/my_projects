#pragma once
#include "Book.h"
#include "DataManager.h"

//struct Subscribers
//{
//	std::string surname; //фамииля
//	int library_card; //номер чиательского билета
//	auto operator<=>(const Subscribers& other) const = default;
//};

class Library
{
private:
	DataManager<Book> books_taken;//взятые книги
	DataManager<Book> books_not_taken; //не взятые книги
	list<Book> subscribers; //список абонентов
	list<std::string> history; //история операций

public:
	DataManager<Book>& GetBooksTaken()
	{
		return books_taken;
	}
	DataManager<Book>& GetBooksNotTaken()
	{
		return books_not_taken;
	}
	list<Book>& GetSubscribers()
	{
		return subscribers;
	}
	list<std::string>& GetHistory()
	{
		return history;
	}
	
	const DataManager<Book>& GetBooksTaken() const
	{
		return books_taken;
	}
	const DataManager<Book>& GetBooksNotTaken() const
	{
		return books_not_taken;
	}
	const list<Book>& GetSubscribers() const
	{
		return subscribers;
	}
	const list<std::string>& GetHistory() const
	{
		return history;
	}


	std::string file_name;
	//Чтение списка абонентов (Фамилия + Номер читательского)
	void SubscribersFile(std::fstream& file);
	//Чтение списка взятых книг
	void BooksTakenFile(std::fstream& file);
	//Чтение списка не взятых книг
	void BooksNotTakenFile(std::fstream& file);
	void Clear();
	//Общая функция для чтения всего файла
	bool AddFile(const std::string& file_name);
	//Печать списка абонентов
	void PrintSubscribers();
	//Проверка, что книгу книгу еще не взяли
	bool NotTaken(Book& book, std::vector<Book>::iterator& res);
	//Проверка, что книгу взяли
	bool Taken(Book& book, std::vector<Book>::iterator& res);
	//Список истории операций
	void OperationsHistory(const std::string& message, Book& book);
	//Поиск книг по критериям
	list<Book> SearchList(const list<function<bool(const Book&)>>& search);
	//Печать не взятых книг
	void PrintNotTaken(std::ostream& stream = std::cout, std::string delim = "\n---------------------------------------\n");
	//Печать взятых книг
	void PrintTaken(std::ostream& stream = std::cout, std::string delim = "\n---------------------------------------\n");
	//Функция сохранения файла
	bool Save();
	bool Save(std::string str);
	//Добавление абонента
	void AddingSubscriber();
	//Удаление абонента
	void DeleteSubscriber(std::list<Book>::iterator& it);
	//Редактирование абонента
	void RedactingSubscriber(Book& it);
	//Печать истории операций
	void PrintHistory();
	bool TakenSub(Book it, std::vector<Book>::iterator& res);

};
//Метод для печати
void PrintNotTakenBook(const Book& book, std::ostream& stream = std::cout, std::string delim = "\n---------------------------------------\n");