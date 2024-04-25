#pragma once
#include <deque>
#include <algorithm>
#include <functional>
#include <list>
#include "DataList.h"
#include "Book.h"


class VirtualLibrary
{
public:
	DataList<Reader> list_of_readers;
	DataList<BookIssued> list_of_books_issued;
	DataList<BookNotIssued> list_of_books_not_issued;
	std::list<string> history_of_book_issuance;

public:

	// ������� ��� ������ � ��������������
	void AddReader(std::istream& stream);																	// ���������� ��������
	void InputListOfReadersFromFile();																		// ���� ������ ��������� �� �����
	deque<Reader>::iterator FindReader(const std::function <bool(Reader)> predicate);						// ����� ��������
	void PrintReaders();																					// ������ ���������

	// ������� ��� ������ �� ������ ����
	void AddBookNotIssued(std::istream& stream);															// ���������� �� ������ �����
	void InputBooksNotIssuedFromFile();																		// ���� ������ �� ������ ���� �� �����
	void PrintBooksNotIssued();																				// ������ �� ������ ����

	// ������� ��� ������ ������ ����
	void AddBookIssued(std::istream& stream);																// ���������� ������ �����
	void InputBooksIssuedFromFile();																		// ���� ������ ������ ���� �� �����
	void PrintBooksIssued();																				// ������ ������ ����

	void SaveFiles();																						// ���������� ������

	void PrintHistoryOfBookIssuance(std::ostream& stream);													// ������ ������� ������ ���� � ����
};