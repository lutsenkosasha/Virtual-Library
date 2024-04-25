#include "VirtualLibrary.h"
#include "Check.h"


void VirtualLibrary::AddReader(std::istream& stream)
{
	Reader reader;
	stream >> reader;
	list_of_readers.Add(reader);
}


void VirtualLibrary::InputListOfReadersFromFile()
{
	std::ifstream file("list of readers.txt");
	if (CheckFile(file))
		list_of_readers.Input({ file });
}


deque<Reader>::iterator VirtualLibrary::FindReader(const std::function<bool(Reader)> predicate)
{
	return std::find_if(list_of_readers.Begin(), list_of_readers.End(), predicate);
}


void VirtualLibrary::PrintReaders()
{
	list_of_readers.Output({ cout, "\n----------------------------------------------\n" });
}


void VirtualLibrary::AddBookNotIssued(std::istream& stream)
{
	BookNotIssued book_not_issued;
	stream >> book_not_issued;
	list_of_books_not_issued.Add(book_not_issued);
}


void VirtualLibrary::InputBooksNotIssuedFromFile()
{
	std::ifstream file("list of books not issued.txt");
	if (CheckFile(file))
		list_of_books_not_issued.Input({ file });
}


void VirtualLibrary::PrintBooksNotIssued()
{
	list_of_books_not_issued.Output({ cout, "\n----------------------------------------------\n" });
}


void VirtualLibrary::AddBookIssued(std::istream& stream)
{
	BookIssued book_issued;
	stream >> book_issued;
	list_of_books_issued.Add(book_issued);
}


void VirtualLibrary::InputBooksIssuedFromFile()
{
	std::ifstream file("list of books issued.txt");
	if (CheckFile(file))
		list_of_books_issued.Input({ file });
}


void VirtualLibrary::PrintBooksIssued()
{
	list_of_books_issued.Output({ cout, "\n----------------------------------------------\n" });
}


void VirtualLibrary::PrintHistoryOfBookIssuance(std::ostream& stream)
{
	for (auto const& i : history_of_book_issuance)
		stream << i << '\n';
}


void VirtualLibrary::SaveFiles()
{
	std::ofstream file_with_readers("list of readers.txt");
	std::ofstream file_with_books_not_issued("list of books not issued.txt");
	std::ofstream file_with_books_issued("list of books issued.txt");
	std::ofstream file_history_of_book_issuance("history of book issuance.txt");

	if (file_with_readers.good())
		list_of_readers.Output({ file_with_readers, "\n" });
	if (file_with_books_not_issued.good())
		list_of_books_not_issued.Output({ file_with_books_not_issued, "\n" });
	if (file_with_books_issued.good())
		list_of_books_issued.Output({ file_with_books_issued, "\n" });
	if (file_history_of_book_issuance.good())
		PrintHistoryOfBookIssuance(file_history_of_book_issuance);
}