#pragma once
#include<Windows.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include "Book.h"
#include "DataManager.h"
#include "Library.h"
#include <functional>
struct Book;

template<class T>
class DataManager;


template<class T>
inline T Input(const std::string& message, std::function<bool(const T&)> condition = [](const T&) {return true; }, const std::string& error = "Ошибка ввода")
{
	T result;
	std::cout << message;
	while (!(std::cin >> result) || !condition(result))
	{
		std::cin.clear();
		std::cin.ignore(1024, '\n');
		std::cout << error << '\n';
		std::cout << message;
	}
	std::cin.ignore(1024, '\n');
	return result;
}

template<>
inline std::string Input(const std::string& message, std::function<bool(const std::string&)> condition, const std::string& error)
{
	std::string result;
	std::cout << message;
	while (!(std::getline(std::cin, result)) || !condition(result))
	{
		std::cin.clear();
		std::cin.ignore(1024, '\n');
		std::cout << error << '\n';
		std::cout << message;
	}

	return result;
}

bool Menu(Library& library);

bool FileMenu(Library& library);

bool LibrarianMenu(Library& library);

bool ViewingBookList(Library& library);

bool WithFiltering(Library& library);

void SearchNotTaken(Library& library);

std::function<bool(const Book&)> ConditionNotTaken();

void SearchTaken(Library& library);

std::function<bool(const Book&)> ConditionTaken();

bool ViewingData(Library& library);

void SortListNotTaken(Library& library);

std::function<bool(const Book&, const Book&)> ComparatorNotTaken();

void SortListTaken(Library& library);

std::function<bool(const Book&, const Book&)> ComparatorTaken();

bool EditingReaderListSubscribers(Library& library);

void SubscriberRemoval(Library& library);

void SubscriberRedacting(Library& library);

bool EditingReaderListBook(Library& library);

bool AddData(Library& library);

void AdingBookTaken(Library& library);

void AdingBookNotTaken(Library& library);

bool DeleteData(Library& library);

void DelBookTaken(Library& library);

void DelBookNotTaken(Library& library);

bool EditData(Library& library);

void EditBookTaken(Library& library);

void EditBookNotTaken(Library& library);

Book EditingInputBooks(bool is_curent_date, bool sub);

void SubscriberMenu(Library& library);

bool SubscriberAction(Library& library, Book& it);

void BorrowBook(Library& library, Book& it);

void ReturnBook(Library& library, Book& it);

Book InputBooks(Book it, bool is_curent_date);

void Search(Library& library);

list<function<bool(const Book&)>> BookSearch();

std::list<Book>::iterator IterSubscriber(std::string messag, Library& library);

bool File(Library& library);

std::fstream InputFile(std::ios_base::openmode mode);