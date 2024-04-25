#pragma once
#include "VirtualLibrary.h"
#include <Windows.h>


void MainMenu(VirtualLibrary& library);																// ������� ����
void ReaderMode(VirtualLibrary& library, Reader& reader);											// ����� ������������
void BookSearchMenu(VirtualLibrary& library);														// ���� ������ �����
void TakeBook(VirtualLibrary& library, Reader& reader);												// ����� �����
void ReturnBook(VirtualLibrary& library, Reader& reader);											// ������� �����
void LibrarianMode(VirtualLibrary& library);														// ����� ������������
void EditListReaders(VirtualLibrary& library);														// ������������� ������ ���������
void RegisterNewReader(VirtualLibrary& library);													// ���������������� ������ ��������
void DeleteReader(VirtualLibrary& library);															// ������� ������ ��������
void EditReader(VirtualLibrary& library);															// ������������� ������ ��������
void EditListBooks(VirtualLibrary& library);														// ������������� ������ ����
void AddBookInLibrary(VirtualLibrary& library);														// �������� ����� � ����������
void DeleteBookFromLibrary(VirtualLibrary& library);												// ������� �� ������ �����
void EditBookFromLibrary(VirtualLibrary& library);													// ������������� �� ������ �����
void GiveBookToReader(VirtualLibrary& library);														// ������ ����� ��������
void TakeBookFromReader(VirtualLibrary& library);													// ������� ����� � ��������
void EditReaderBook(VirtualLibrary& library);														// ������������� ����� ��������
void WatchHistoryOfBookIssuance(VirtualLibrary& library);											// ����������� ������� ������ ����
void ViewListBooksWithCurrentStatusOfEach(VirtualLibrary& library);									// ����������� ������ ���� � ������� �������� ������
void SortingBooksNotIssued(VirtualLibrary& library);												// ���������� �� ������ ����
void SortingBooksIssued(VirtualLibrary& library);													// ���������� ������ ����
void FilteringBooksNotIssued(VirtualLibrary& library);												// ���������� �� ������ ����
void FilteringBooksIssued(VirtualLibrary& library);													// ���������� ������ ����