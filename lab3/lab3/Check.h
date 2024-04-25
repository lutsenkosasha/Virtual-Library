#pragma once
#include <string>
#include <functional>
#include <iostream>
using namespace std;


// Шаблон функции ввода значения заданного типа, удовлетворяющего условию, с консоли
template<class T>
inline T Input(const string& message, function<bool(T)> condition = [](T) {return true; })
{
	T result;
	cout << message;
	while (!(cin >> result) || !condition(result))
	{
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "Ошибка ввода\n";
		cout << message;
	}
	cin.ignore(1024, '\n');
	return result;
}


// Спецификация для строк
template<>
inline string Input(const string& message, function<bool(string)> condition)
{
	string result;
	cout << message;
	while (!getline(cin, result) || !condition(result))
	{
		cin.clear();
		cout << "Ошибка ввода\n";
		cout << message;
	}
	return result;
}


// Проверка файла
inline bool CheckFile(std::ifstream& file)
{
	bool result = true;
	if (!file)
	{
		cout << "Ошибка файла!\n";
		result = false;
	}
	else
	{
		if (file.peek() == EOF)
		{
			cout << "Файл пуст!\n";
			result = false;
		}
	}
	return result;
}