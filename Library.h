#pragma once
#include "Book.h"
#include "DataManager.h"

//struct Subscribers
//{
//	std::string surname; //�������
//	int library_card; //����� ������������ ������
//	auto operator<=>(const Subscribers& other) const = default;
//};

class Library
{
private:
	DataManager<Book> books_taken;//������ �����
	DataManager<Book> books_not_taken; //�� ������ �����
	list<Book> subscribers; //������ ���������
	list<std::string> history; //������� ��������

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
	//������ ������ ��������� (������� + ����� �������������)
	void SubscribersFile(std::fstream& file);
	//������ ������ ������ ����
	void BooksTakenFile(std::fstream& file);
	//������ ������ �� ������ ����
	void BooksNotTakenFile(std::fstream& file);
	void Clear();
	//����� ������� ��� ������ ����� �����
	bool AddFile(const std::string& file_name);
	//������ ������ ���������
	void PrintSubscribers();
	//��������, ��� ����� ����� ��� �� �����
	bool NotTaken(Book& book, std::vector<Book>::iterator& res);
	//��������, ��� ����� �����
	bool Taken(Book& book, std::vector<Book>::iterator& res);
	//������ ������� ��������
	void OperationsHistory(const std::string& message, Book& book);
	//����� ���� �� ���������
	list<Book> SearchList(const list<function<bool(const Book&)>>& search);
	//������ �� ������ ����
	void PrintNotTaken(std::ostream& stream = std::cout, std::string delim = "\n---------------------------------------\n");
	//������ ������ ����
	void PrintTaken(std::ostream& stream = std::cout, std::string delim = "\n---------------------------------------\n");
	//������� ���������� �����
	bool Save();
	bool Save(std::string str);
	//���������� ��������
	void AddingSubscriber();
	//�������� ��������
	void DeleteSubscriber(std::list<Book>::iterator& it);
	//�������������� ��������
	void RedactingSubscriber(Book& it);
	//������ ������� ��������
	void PrintHistory();
	bool TakenSub(Book it, std::vector<Book>::iterator& res);

};
//����� ��� ������
void PrintNotTakenBook(const Book& book, std::ostream& stream = std::cout, std::string delim = "\n---------------------------------------\n");