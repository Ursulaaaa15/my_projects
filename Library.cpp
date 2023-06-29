#include "Library.h"
#include "Func.h"

void Library::SubscribersFile(std::fstream& file)
{
    Book sub;
    int count_subscribers;
    file >> count_subscribers;
    while (count_subscribers > 0)
    {
        file >> sub.GetFIO();
        file >> sub.GetLibraryCard();
        subscribers.push_back(sub);
        count_subscribers--;
    }
}

void Library::BooksTakenFile(std::fstream& file)
{
    Book taken;
    int count_books_taken;
    file >> count_books_taken;
    file.ignore(1024, '\n');
    while (count_books_taken > 0)
    {
        file >> taken;
        books_taken.Add(taken);
        count_books_taken--;
    }
}

void Library::BooksNotTakenFile(std::fstream& file)
{
    int y;
    Book not_taken;
    int count_books_not_taken;
    file >> count_books_not_taken;
    file.ignore(1024, '\n');
    while (count_books_not_taken > 0)
    {
        getline(file, not_taken.GetTitle());
        getline(file, not_taken.GetAuthor());
        getline(file, not_taken.GetPublisher());
        file >> y;
        not_taken.GetPublicationYear() = year{y};
        file >> not_taken.GetPrice();
        file.ignore(1024, '\n');
        books_not_taken.Add(not_taken);
        count_books_not_taken--;
    }
}

void Library::Clear()
{
    books_taken.Clear();
    books_not_taken.Clear();
    history.clear();
    subscribers.clear();
}

bool Library::AddFile(const std::string& file_name)
{
    std::string str;
    std::fstream file(file_name);
    if (!file.is_open())
        return false;
    Clear();
    SubscribersFile(file);
    BooksTakenFile(file);
    BooksNotTakenFile(file);
    if (!file)
        return false;
    while (getline(file, str))
        history.push_back(str);
    this->file_name = file_name;
    return true;
}

void Library::PrintSubscribers()
{
    int k = 1;
    for (auto const& i : subscribers)
    {
        std::cout << '[' << k << "] " << i.GetFIO() << ' ' << i.GetLibraryCard() << '\n';
        k++;
    }
    if (k == 1)
        std::cout << "Пусто!\n";
}

void PrintNotTakenBook(const Book& book, std::ostream& stream, std::string delim)
{
    stream << book.GetTitle() << '\n' << book.GetAuthor() << '\n' << book.GetPublisher() << '\n' << (int)book.GetPublicationYear() << '\n' << book.GetPrice() << delim;
}

void Library::PrintNotTaken(std::ostream& stream, std::string delim)
{
    for (auto const& i : books_not_taken)
        PrintNotTakenBook(i, stream, delim);
}

void PrintTakenBook(const Book& book, std::ostream& stream, std::string delim)
{
    stream << book.GetFIO() << '\n' << book.GetLibraryCard() << '\n' << book.GetTitle() << '\n' << book.GetAuthor() << '\n'
        << book.GetPublisher() << '\n' << (int)book.GetPublicationYear() << '\n' << book.GetPrice() << '\n' << (unsigned int)book.GetDate().day() << " "
        << (unsigned int)book.GetDate().month() << " " << (int)book.GetDate().year() << '\n' << (unsigned int)book.GetReturnDate().day() << " "
        << (unsigned int)book.GetReturnDate().month() << " " << (int)book.GetReturnDate().year() << delim;
}

void Library::PrintTaken(std::ostream& stream, std::string delim)
{
    for (auto const& i : books_taken)
        PrintTakenBook(i, stream, delim);
}

bool Library::NotTaken(Book& book, std::vector<Book>::iterator& res)
{
    for (auto i = books_not_taken.begin(); i != books_not_taken.end(); ++i)
        if (i->GetTitle() == book.GetTitle() && i->GetAuthor() == book.GetAuthor() && i->GetPrice() == book.GetPrice() && i->GetPublicationYear() == book.GetPublicationYear() && i->GetPublisher() == book.GetPublisher())
        {
            res = i;
            return true;
        }
    return false;
}

bool Library::Taken(Book& book, std::vector<Book>::iterator& res)
{
    for (auto i = books_taken.begin(); i != books_taken.end(); ++i)
        if (i->GetTitle() == book.GetTitle() && i->GetAuthor() == book.GetAuthor() && i->GetPrice() == book.GetPrice() && i->GetPublicationYear() == book.GetPublicationYear() && i->GetPublisher() == book.GetPublisher())
        {
            res = i;
            return true;
        }
    return false;
}

bool Library::TakenSub(Book it, std::vector<Book>::iterator& res)
{
    for (auto i = books_taken.begin(); i != books_taken.end(); ++i)
        if (i->GetLibraryCard() == it.GetLibraryCard())
        {
            res = i;
            return true;
        }
    return false;
}

void Library::OperationsHistory(const std::string& message, Book& book)
{
    std::string operation = message + book.GetTitle();
    history.push_back(operation);
    Save();
}

list<Book> Library::SearchList(const list<function<bool(const Book&)>>& search)
{
    return books_not_taken.FindAll([&search](auto& Book)
        {
            for (auto x : search)
                if (!x(Book))
                    return false;
            return true;
        });
}

bool Library::Save()
{
    return Save(file_name);
}

bool Library::Save(std::string str)
{
    std::ofstream file(str);
    if (!file.is_open())
        return false;
    file << subscribers.size() << '\n';
    for (auto const& i : subscribers)
        file << i.GetFIO() << '\n' << i.GetLibraryCard() << '\n';
    file << books_taken.Size() << '\n';
    books_taken.Output(std::ostream_iterator<Book>(file, "\n"));
    file << books_not_taken.Size() << '\n';
    PrintNotTaken(file, "\n");
    for (auto const& i : history)
        file << i << '\n';
    return true;
}

void Library::AddingSubscriber()
{
    Book sub;
    sub.GetFIO() = Input<std::string>("Введите фамилию абонента: ", &Book::CheckFIO);
    sub.GetLibraryCard() = Input<int>("Введите номер читательского билета: ", &Book::CheckLibraryCard);
    subscribers.push_back(sub);
    std::cout << "Абонент добавлен!\n";
    Save();
}

void Library::DeleteSubscriber(std::list<Book>::iterator& it)
{
    subscribers.erase(it);
    
    Save();
}

void Library::RedactingSubscriber(Book& it)
{
    auto sur = Input<int>("Хотите радактировать фамилию абонента?\n[1] Да\n[2] Нет\n");
    if (sur == 1)
        it.GetFIO() = Input<std::string>("Введите новую фамилию абонента: ", &Book::CheckFIO);
    auto cart = Input<int>("Хотите радактировать номер читательского билета абонента?\n[1] Да\n[2] Нет\n");
    if (cart == 1)
        it.GetLibraryCard() = Input<int>("Введите новый номер читательского билета: ", &Book::CheckLibraryCard);
    Save();
}

void Library::PrintHistory()
{
    for (auto const& i : history)
        std::cout << i << '\n';
    if (history.empty())
        std::cout << "У вас нет истории!\n";
}