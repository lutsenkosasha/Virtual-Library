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

	// Функции для работы с пользователями
	void AddReader(std::istream& stream);																	// Добавление читателя
	void InputListOfReadersFromFile();																		// Ввод списка читателей из файла
	deque<Reader>::iterator FindReader(const std::function <bool(Reader)> predicate);						// Поиск читателя
	void PrintReaders();																					// Печать читателей

	// Функции для списка не взятых книг
	void AddBookNotIssued(std::istream& stream);															// Добавление не взятой книги
	void InputBooksNotIssuedFromFile();																		// Ввод списка не взятых книг из файла
	void PrintBooksNotIssued();																				// Печать не взятых книг

	// Функции для списка взятых книг
	void AddBookIssued(std::istream& stream);																// Добавление взятой книги
	void InputBooksIssuedFromFile();																		// Ввод списка взятых книг из файла
	void PrintBooksIssued();																				// Печать взятых книг

	void SaveFiles();																						// Сохранение файлов

	void PrintHistoryOfBookIssuance(std::ostream& stream);													// Печать истории выдачи книг в файл
};