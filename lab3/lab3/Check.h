#pragma once
#include <string>
#include <functional>
#include <iostream>
using namespace std;


// ������ ������� ����� �������� ��������� ����, ���������������� �������, � �������
template<class T>
inline T Input(const string& message, function<bool(T)> condition = [](T) {return true; })
{
	T result;
	cout << message;
	while (!(cin >> result) || !condition(result))
	{
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "������ �����\n";
		cout << message;
	}
	cin.ignore(1024, '\n');
	return result;
}


// ������������ ��� �����
template<>
inline string Input(const string& message, function<bool(string)> condition)
{
	string result;
	cout << message;
	while (!getline(cin, result) || !condition(result))
	{
		cin.clear();
		cout << "������ �����\n";
		cout << message;
	}
	return result;
}


// �������� �����
inline bool CheckFile(std::ifstream& file)
{
	bool result = true;
	if (!file)
	{
		cout << "������ �����!\n";
		result = false;
	}
	else
	{
		if (file.peek() == EOF)
		{
			cout << "���� ����!\n";
			result = false;
		}
	}
	return result;
}