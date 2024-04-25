#pragma once
#include "VirtualLibrary.h"
#include <Windows.h>


void MainMenu(VirtualLibrary& library);																// Главное меню
void ReaderMode(VirtualLibrary& library, Reader& reader);											// Режим пользователя
void BookSearchMenu(VirtualLibrary& library);														// Меню поиска книги
void TakeBook(VirtualLibrary& library, Reader& reader);												// Взять книгу
void ReturnBook(VirtualLibrary& library, Reader& reader);											// Вернуть книгу
void LibrarianMode(VirtualLibrary& library);														// Режим библиотекаря
void EditListReaders(VirtualLibrary& library);														// Редактировать список читателей
void RegisterNewReader(VirtualLibrary& library);													// Зарегистрировать нового читателя
void DeleteReader(VirtualLibrary& library);															// Удалить данные читателя
void EditReader(VirtualLibrary& library);															// Редактировать данные читателя
void EditListBooks(VirtualLibrary& library);														// Редактировать список книг
void AddBookInLibrary(VirtualLibrary& library);														// Добавить книгу в библиотеку
void DeleteBookFromLibrary(VirtualLibrary& library);												// Удалить не взятую книгу
void EditBookFromLibrary(VirtualLibrary& library);													// Редактировать не взятую книгу
void GiveBookToReader(VirtualLibrary& library);														// Выдать книгу читателю
void TakeBookFromReader(VirtualLibrary& library);													// Забрать книгу у читателя
void EditReaderBook(VirtualLibrary& library);														// Редактировать книгу читателя
void WatchHistoryOfBookIssuance(VirtualLibrary& library);											// Просмотреть историю выдачи книг
void ViewListBooksWithCurrentStatusOfEach(VirtualLibrary& library);									// Просмотреть список книг с текущим статусом каждой
void SortingBooksNotIssued(VirtualLibrary& library);												// Сортировка не взятых книг
void SortingBooksIssued(VirtualLibrary& library);													// Сортировка взятых книг
void FilteringBooksNotIssued(VirtualLibrary& library);												// Фильтрация не взятых книг
void FilteringBooksIssued(VirtualLibrary& library);													// Фильтрация взятых книг