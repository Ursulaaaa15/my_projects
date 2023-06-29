#pragma once
#include <vector>
#include <functional>
#include <list>
#include <fstream>

using std::vector;
using std::ostream_iterator;
using std::istream_iterator;
using std::function;
using std::list;


template<class T>
class DataManager
{
	std::vector<T> data;

public:
	using iterator = typename std::vector<T>::iterator;
	using const_iterator = typename std::vector<T>::const_iterator;
	//Ввод элементов через поток
	void Input(istream_iterator<T> in);
	//Замена элемента
	bool Edit(const T& books_old, const T& books_new);
	//Удаление элемента
	bool Del(const T& element);
	//Очистка вектора
	void Clear();
	//Проверка на пустоту
	bool Empty() const;
	//Линейный поиск
	const_iterator Linear(const T& element, function<bool(const T&, const T&)> comparator) const;
	//Сортировка элементов
	void Sort(function<bool(const T&, const T&)> condition);
	//Бинарный поиск
	const_iterator Binary(const T& element, function<bool(const T&, const T&)> comparator) const;
	//Поиск всех элементов, удовлетворяющих критерию
	std::list<T> FindAll(function<bool(const T&)> condition) const;
	//Поиск и вывод всех элементов, удовлетворяющих критерию
	void Subset(function<bool(const T&)> condition, ostream_iterator<T> out) const;
	//Поиск и вывод всех элементов, удовлетворяющих критерию, empty — результат поиска элементов
	void Subset(function<bool(const T&)> condition, ostream_iterator<T> out, bool& empty) const;
	//Вывод элементов через потоковый итератор
	void Output(ostream_iterator<T> out) const;
	//Добавление элементов по значению
	void Add(const T& books);
	//Размер
	size_t Size() const;

	const_iterator begin() const;
	const_iterator end() const;
	iterator begin();
	iterator end();
};



template<class T>
inline void DataManager<T>::Add(const T& books)
{

	data.push_back(books);
}

template<class T>
inline size_t DataManager<T>::Size() const
{
	return data.size();
}

template<class T>
inline void DataManager<T>::Input(std::istream_iterator<T> in)
{
	std::copy(in, std::istream_iterator<T>(), std::back_inserter<std::vector<T>>(data));
}

template<class T>
inline bool DataManager<T>::Edit(const T& books_old, const T& books_new)
{
	auto it = find(data.begin(), data.end(), books_old);
	if (it == data.end())
		return false;
	*it = books_new;
	return true;
}

template<class T>
inline bool DataManager<T>::Del(const T& books)
{
	auto it = find(data.begin(), data.end(), books);
	if (it == data.end())
		return false;
	data.erase(it);
	return true;
}

template<class T>
inline void DataManager<T>::Clear()
{
	data.clear();
}

template<class T>
inline bool DataManager<T>::Empty() const
{
	return data.empty();
}

template<class T>
inline void DataManager<T>::Sort(function<bool(const T&, const T&)> condition)
{
	sort(data.begin(), data.end(), condition);
}

template<class T>
void DataManager<T>::Output(ostream_iterator<T> out) const
{
	for_each(data.begin(), data.end(), [&out](const T& current) {out = current; });
}

template<class T>
inline std::list<T> DataManager<T>::FindAll(function<bool(const T&)> condition) const
{
	std::list<T> result;
	for (auto& item : data)
		if (condition(item))
			result.push_back(item);
	return result;
}

template<class T>
inline void DataManager<T>::Subset(function<bool(const T&)> condition, ostream_iterator<T> out) const
{
	bool p = true;
	Subset(condition, out, p);
}

template<class T>
inline void DataManager<T>::Subset(function<bool(const T&)> condition, ostream_iterator<T> out, bool& empty) const
{
	auto lst = FindAll(condition);
	empty = lst.empty();
	for_each(lst.begin(), lst.end(), [&out](const T& element) {out = element; });
}

template<class T>
inline typename DataManager<T>::const_iterator DataManager<T>::Binary(const T& element, function<bool(const T&, const T&)> condition) const
{
	auto left = data.begin();
	auto right = data.end();
	auto count = right - left;
	auto it = left;
	size_t step = 0;
	while (count > 0)
	{
		it = left;
		step = count / 2;
		advance(it, step);
		if (condition(*it, element))
		{
			left = ++it;
			count -= step + 1;
		}
		else
			count = step;
	}
	return (left != right && !condition(element, *left)) ? left : right;
}

template<class T>
inline typename DataManager<T>::const_iterator DataManager<T>::Linear(const T& element, function<bool(const T&, const T&)> condition) const
{
	const_iterator c = data.begin();
	const_iterator end = data.end();
	while (c != end && (condition(*c, element) || condition(element, *c))) { ++c; }
	return c;
}

template<class T>
inline typename DataManager<T>::const_iterator DataManager<T>::begin() const
{
	return data.begin();
}

template<class T>
inline typename DataManager<T>::const_iterator DataManager<T>::end() const
{
	return data.end();
}

template<class T>
inline typename DataManager<T>::iterator DataManager<T>::begin()
{
	return data.begin();
}

template<class T>
inline typename DataManager<T>::iterator DataManager<T>::end()
{
	return data.end();
}