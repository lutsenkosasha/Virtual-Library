#pragma once
#include <deque>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>


using std::deque;
using std::istream_iterator;
using std::ostream_iterator;
using std::cout;
using std::cin;


template<typename T>
class DataList
{
private:
	deque<T> deq;
	deque<T> helper;

public:
	DataList() : deq(), helper() {}

	void Add(const T& value);
	bool Remove(const T& value);
	void AddData();
	void ChangeData(typename deque<T>::iterator x);
	void DeleteData(typename deque<T>::iterator x);
	void Clear();
	void PrintData(typename deque<T>::iterator x);
	void PrintData();
	void PrintSomeData();
	void Sort(std::function<bool(T, T)> comp);
	void Copy(const std::function<bool(T)> predicate);
	deque<T> FindAll(const std::function<bool(T)> predicate) const;
	bool PrintAll(const std::function<bool(T)> predicate, std::ostream_iterator<T> it) const;
	void PrintPartData(std::ostream& out, typename deque<T>::iterator x);
	void WriteFullToFile(std::ofstream& file);
	void ReadFullFromFile(std::ifstream& file);
	void Input(std::istream_iterator<T> iterator);
	void Output(std::ostream_iterator<T> iterator);

	deque<T>::iterator Find(const std::function <bool(T)> predicate);
	deque<T>::iterator LowerBound(T book, std::function<bool(const T&, const T&)> predicate);
	deque<T>::iterator UpperBound(T book, std::function<bool(const T&, const T&)> predicate);
	deque<T>::iterator Begin();
	deque<T>::iterator End();
	deque<T>::const_iterator Begin() const;
	deque<T>::const_iterator End() const;
};


template<typename T>
inline void DataList<T>::Add(const T& value)
{
	deq.push_back(value);
}


template<typename T>
inline bool DataList<T>::Remove(const T& value)
{
	auto it = find(deq.begin(), deq.end(), value);
	if (it == deq.end())
		return false;
	deq.erase(it);
	return true;
}


template<typename T>
inline void DataList<T>::AddData()
{
	istream_iterator<T> is;
	is = cin;
	deq.push_back(*is);
}


template<typename T>
inline void DataList<T>::ChangeData(typename deque<T>::iterator x)
{
	istream_iterator<T> is(cin);
	*x = *is;
}


template<typename T>
inline void DataList<T>::DeleteData(typename deque<T>::iterator x)
{
	deq.erase(x);
}


template<typename T>
inline void DataList<T>::Clear()
{
	helper.clear();
	deq.clear();
}


template<typename T>
inline void DataList<T>::PrintData(typename deque<T>::iterator x)
{
	cout << *x << '\n';
}


template<typename T>
inline void DataList<T>::PrintData()
{
	for (auto i = deq.begin(); i < deq.end(); i++)
		PrintData(i);
}


template<typename T>
inline void DataList<T>::PrintSomeData()
{
	copy(helper.begin(), helper.end(), ostream_iterator<T>(cout, "\n"));
}


template<typename T>
inline void DataList<T>::Sort(std::function<bool(T, T)> comp)
{
	std::sort(deq.begin(), deq.end(), comp);
}


template<typename T>
inline void DataList<T>::Copy(const std::function<bool(T)> predicate)
{
	helper.clear();
	std::copy_if(deq.begin(), deq.end(), std::back_inserter(helper), predicate);
}


template<typename T>
inline deque<T> DataList<T>::FindAll(const std::function<bool(T)> predicate) const
{
	deque<T> result;
	std::copy_if(deq.begin(), deq.end(), std::back_inserter(result), predicate);
	return result;
}


template<typename T>
inline bool DataList<T>::PrintAll(const std::function<bool(T)> predicate, std::ostream_iterator<T> it) const
{
	if (deq.empty())
		return false;
	auto entries = FindAll(predicate);
	std::copy(entries.begin(), entries.end(), it);
	return true;
}


template<typename T>
inline void DataList<T>::PrintPartData(std::ostream& out, typename deque<T>::iterator x)
{
	ostream_iterator<T> is(out, "\n");
	*is = *x;
}


template<typename T>
inline void DataList<T>::WriteFullToFile(std::ofstream& file)
{
	file.clear();
	copy(deq.begin(), deq.end(), ostream_iterator<T>(file, "\n"));
}


template<typename T>
inline void DataList<T>::ReadFullFromFile(std::ifstream& file)
{
	copy(istream_iterator<T>(file), istream_iterator<T>(), std::back_inserter(deq));
}


template<typename T>
inline void DataList<T>::Input(std::istream_iterator<T> iterator)
{
	copy(iterator, istream_iterator<T>(), std::back_inserter(deq));
}


template<typename T>
inline void DataList<T>::Output(std::ostream_iterator<T> iterator)
{
	copy(deq.begin(), deq.end(), iterator);
}


template<typename T>
inline deque<T>::iterator DataList<T>::Find(const std::function<bool(T)> predicate)
{
	return std::find_if(deq.begin(), deq.end(), predicate);
}


template<typename T>
inline deque<T>::iterator DataList<T>::LowerBound(T book, std::function<bool(const T&, const T&)> predicate)
{
	return std::lower_bound(deq.begin(), deq.end(), book, predicate);
}


template<typename T>
inline deque<T>::iterator DataList<T>::UpperBound(T book, std::function<bool(const T&, const T&)> predicate)
{
	return std::upper_bound(deq.begin(), deq.end(), book, predicate);
}

template<typename T>
inline deque<T>::iterator DataList<T>::Begin()
{
	return deq.begin();
}


template<typename T>
inline deque<T>::iterator DataList<T>::End()
{
	return deq.end();
}


template<typename T>
inline deque<T>::const_iterator DataList<T>::Begin() const
{
	return deq.begin();
}


template<typename T>
inline deque<T>::const_iterator DataList<T>::End() const
{
	return deq.end();
}
