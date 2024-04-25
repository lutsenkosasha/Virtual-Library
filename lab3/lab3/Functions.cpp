#include "Functions.h"
#include <chrono>
#include "Check.h"

using namespace std::chrono;


// ������� ����
void MainMenu(VirtualLibrary& library)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	library.InputListOfReadersFromFile();		// ���� ������ ��������� �� �����
	library.InputBooksNotIssuedFromFile();		// ���� ������ �� ������ ���� �� �����
	library.InputBooksIssuedFromFile();			// ���� ������ ������ ���� �� �����

	short choice_1;
	choice_1 = Input<short>("[1] ������ ������ ���������\n"
		"[2] ��������� ������ ���������\n", [](auto x) {return x == 1 || x == 2; });
	while (choice_1 != 2 && choice_1 == 1)
	{
		short choice_2;
		choice_2 = Input<short>("�������� ����� ������:\n"
			"[1] ��������\n"
			"[2] ������������\n"
			"[0] �����\n", [](auto x) {return x == 1 || x == 2 || x == 0; });
		switch (choice_2)
		{
		case 1:
		{
			Reader reader;
			ReaderMode(library, reader);
			break;
		}
		case 2:
		{
			LibrarianMode(library);
			break;
		}
		case 0:
		{
			choice_1 = 2;
			break;
		}
		default:
		{
			cout << "������ �����!\n";
			break;
		}
		}
	}
}


// ����� ������������
void ReaderMode(VirtualLibrary& library, Reader& reader)
{
	int num;
	deque<Reader>::iterator iterator;
	cout << "\n������ ���������:\n";
	library.PrintReaders();
	num = Input<int>("������� ����� ������ ������������� ������: ", [](auto x) {return x > 0; });
	iterator = library.FindReader([&num](const Reader& reader) { return num == reader.ticket_number; });				// ����� �������� �� ������ ������
	if (iterator != library.list_of_readers.End())																		// ���� �������, ��:
	{
		reader = *iterator;
		short input;
		input = Input<short>("�������� ��������:\n"
			"[1] ���������� ������ ��������� ����\n"
			"[2] ����� ����� �� ��������� �������� (����� ���� ��� ����� �����)\n"
			"[3] ����� �����\n"
			"[4] ������� �����\n", [](auto x) {return x == 1 || x == 2 || x == 3 || x == 4; });
		switch (input)
		{
		case 1:
		{
			library.PrintBooksNotIssued();													// ������ ������ �� ������ ����
			break;
		}
		case 2:
		{
			BookSearchMenu(library);														// ���� ������ �����
			break;
		}
		case 3:
		{
			TakeBook(library, reader);														// ����� �����
			break;
		}
		case 4:
		{
			ReturnBook(library, reader);													// ������� �����
			break;
		}
		default:
		{
			cout << "������ �����!\n";
			break;
		}
		}
	}
	else
		cout << "\n������������ � ����� ������� ������������� ������ ��� � ����!\n"
		"���������� � ������������ � �������� ��� ����������������\n";
}


// ���� ������ �����
void BookSearchMenu(VirtualLibrary& library)
{
	bool array[5] = { false, false, false, false, false };
	deque<BookNotIssued>::iterator iterator;
	short input;
	string author, name_book, publishing_house;
	int year_of_publication, price;

	input = Input<short>("����� ����� �� ������?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
	if (input == 1)
	{
		author = Input<string>("������� ������ �����: ", [](auto x) {return !x.empty(); });
		array[0] = true;
	}

	input = Input<short>("����� ����� �� ��������?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
	if (input == 1)
	{
		name_book = Input<string>("������� �������� �����: ", [](auto x) {return !x.empty(); });
		array[1] = true;
	}

	input = Input<short>("����� ����� �� ���� �������?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
	if (input == 1)
	{
		year_of_publication = Input<int>("������� ��� ������� �����: ", [](auto x) {return x > 0; });
		array[2] = true;
	}

	input = Input<short>("����� ����� �� ������������?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
	if (input == 1)
	{
		publishing_house = Input<string>("������� ������������ �����: ", [](auto x) {return !x.empty(); });
		array[3] = true;
	}

	input = Input<short>("����� ����� �� ����?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
	if (input == 1)
	{
		price = Input<int>("������� ���� �����: ", [](auto x) {return x > 0; });
		array[4] = true;
	}

	iterator = library.list_of_books_not_issued.Find([author, name_book, year_of_publication, publishing_house, price, array](const BookNotIssued& p) {				// ����� �� ���� ��������� ���������
		return	(!array[0] || author == p.author)
			&& (!array[1] || name_book == p.name_book)
			&& (!array[2] || year_of_publication == p.year_of_publication)
			&& (!array[3] || publishing_house == p.publishing_house)
			&& (!array[4] || price == p.price);
	});

	if (iterator != library.list_of_books_not_issued.End())							// ���� ����� �������, �� �������� �� �����
		library.list_of_books_not_issued.PrintData(iterator);
	else
		cout << "\n�� ������ ��������� �� �� ����� ���������� ��� ��� �����\n";
}


// ����� �����
void TakeBook(VirtualLibrary& library, Reader& reader)
{
	deque<BookIssued>::iterator iterator;
	string name_book;
	name_book = Input<string>("������� �������� �����, ������� ������ �����: ", [](auto x) {return !x.empty(); });
	iterator = library.list_of_books_issued.Find([name_book](const BookIssued& p) { return name_book == p.book_not_issued.name_book; });			// ��� ����� � ����� ��������� ����� ������ ����
	if (iterator != library.list_of_books_issued.End())																								// ���� ����� ������� � ������ ������ ����, ��:
		cout << "\n��� ����� ��� ���-�� ����! �������� :(\n";
	else
	{
		deque<BookNotIssued>::iterator it;
		it = library.list_of_books_not_issued.Find([name_book](const BookNotIssued& p) { return name_book == p.name_book; });						// ��� ����� � ����� ��������� � ������ �� ������ ����
		if (it != library.list_of_books_not_issued.End())																							// ���� �����, ��:
		{
			Date return_date;
			Date ymd(floor<days>(system_clock::now()).time_since_epoch().count());																	// ���� ������������ ���
			BookIssued book_issued;
			book_issued.reader = reader;
			book_issued.book_not_issued = (*it);
			book_issued.date_of_issue = ymd;
			cout << "\n������� ���� �������� �����:\n";
			cin >> return_date;
			book_issued.return_date = return_date;

			library.list_of_books_issued.Add(book_issued);																							// �������� ������ ����� � ������ ������ ����

			library.list_of_books_not_issued.DeleteData(it);																						// �� ������ �� ������ ���� ������ �����, ������� ����
			cout << "\n�� ������� ����� �����\n";

			library.history_of_book_issuance.push_back("������������ " + reader.surname + " ���� ����� ��� ��������� " + book_issued.book_not_issued.name_book);		// �������� ������ � ������� ������ ����

			library.SaveFiles();																													// ��������� ��� ����� �����
		}
		else
			cout << "\n���� � ����� ��������� � ����� ���������� ���.\n";
	}
}


// ������� �����
void ReturnBook(VirtualLibrary& library, Reader& reader)
{
	deque<BookNotIssued>::iterator iterator;
	string name_book;
	name_book = Input<string>("������� �������� �����, ������� ������ �������: ", [](auto x) {return !x.empty(); });
	iterator = library.list_of_books_not_issued.Find([name_book](const BookNotIssued& p) { return name_book == p.name_book; });						// ��� ����� � ����� ��������� ����� �� ������ ����
	if (iterator != library.list_of_books_not_issued.End())																							// ���� ����� ������� � ������ �� ������ ����, ��:
		cout << "\n��� ����� ��� �������!\n";
	else
	{
		deque<BookIssued>::iterator it;
		it = library.list_of_books_issued.Find([name_book, &reader](const BookIssued& p)
			{ return (name_book == p.book_not_issued.name_book) && (reader.ticket_number == p.reader.ticket_number); });							// ��� ����� � ����� ��������� � ������ ������ ����
																																					// � ��������, ��� ����� ����������� �������� ������������

		if (it != library.list_of_books_issued.End())																								// ���� �����, ��:
		{
			BookNotIssued book_not_issued;
			book_not_issued = (*it).book_not_issued;

			library.list_of_books_not_issued.Add(book_not_issued);																					// �������� ����� � ������ �� ������ ����

			library.list_of_books_issued.DeleteData(it);																							// �� ������ ������ ���� ������ �����, ������� ���������
			cout << "\n�� ������� ������� �����\n";

			library.history_of_book_issuance.push_back("������������ " + reader.surname + " ������ ����� ��� ��������� " + book_not_issued.name_book);		// �������� ������ � ������� ������ ����

			library.SaveFiles();																													// ��������� ��� ����� �����
		}
		else
			cout << "\n� ��� �� �������� ����� � ����� ���������\n";
	}
}


// ����� ������������
void LibrarianMode(VirtualLibrary& library)
{
	short input;
	input = Input<short>("�������� ��������:\n"
		"[1] ������������� ������ ���������\n"
		"[2] ������������� ������ ����\n"
		"[3] ����������� ������� ������ ����\n"
		"[4] ����������� ������ ���� � ������� �������� ������\n", [](auto x) {return x == 1 || x == 2 || x == 3 || x == 4; });
	switch (input)
	{
	case 1:
	{
		EditListReaders(library);																// ������������� ������ ���������
		break;
	}
	case 2:
	{
		EditListBooks(library);																	// ������������� ������ ����
		break;
	}
	case 3:
	{
		WatchHistoryOfBookIssuance(library);													// ����������� ������� ������ ����
		break;
	}
	case 4:
	{
		ViewListBooksWithCurrentStatusOfEach(library);											// ����������� ������ ���� � ������� �������� ������
		break;
	}
	default:
	{
		cout << "������ �����!\n";
		break;
	}
	}
}


// ������������� ������ ���������
void EditListReaders(VirtualLibrary& library)
{
	short input;
	input = Input<short>("�������� ��������:\n"
		"[1] ���������������� ������ ��������\n"
		"[2] ������� ������ ��������\n"
		"[3] ������������� ������ ��������\n", [](auto x) {return x == 1 || x == 2 || x == 3; });
	switch (input)
	{
	case 1:
	{
		RegisterNewReader(library);									// ���������������� ������ ��������
		break;
	}
	case 2:
	{
		DeleteReader(library);										// ������� ������ ��������
		break;
	}
	case 3:
	{
		EditReader(library);										// ������������� ������ ��������
		break;
	}
	default:
	{
		cout << "������ �����!\n";
		break;
	}
	}
}


// ���������������� ������ ��������
void RegisterNewReader(VirtualLibrary& library)
{
	library.AddReader(cin);
	cout << "\n����� ������������ �������� �������!\n";
	library.SaveFiles();
}


// ������� ������ ��������
void DeleteReader(VirtualLibrary& library)
{
	int num;
	deque<Reader>::iterator iterator;
	cout << "\n������ ���������:\n";
	library.PrintReaders();
	num = Input<int>("������� ����� ������������� ������ ��� �������� ������������: ", [](auto x) {return x > 0; });

	iterator = library.FindReader([&num](const Reader& reader) { return num == reader.ticket_number; });												// ����� ��������, �������� ����� �������, �� ������ ������
	if (iterator != library.list_of_readers.End())																										// ���� �������, ��:
	{
		deque<BookIssued>::iterator it;
		it = library.list_of_books_issued.Find([iterator](const BookIssued& book_issued) { return (*iterator) == book_issued.reader; });				// ���������, ���� �� � ����� ������������ ������ �����
		if (it != library.list_of_books_issued.End())																									// ���� ����, ��
		{
			cout << "\n� ������� �������� ���� ����� �� �����!"
				"\n����� ��������� �������� ���������� �������������� ������� ���� ���� � ����������.\n";
			while (it != library.list_of_books_issued.End())
			{
				library.list_of_books_issued.DeleteData(it);																							// �� ������ ������ ���� ������ �����, ������� ���������

				BookNotIssued book_not_issued;
				book_not_issued = (*it).book_not_issued;

				library.list_of_books_not_issued.Add(book_not_issued);																					// �������� ����� � ������ �� ������ ����

				it = library.list_of_books_issued.Find([iterator](const BookIssued& book_issued) { return (*iterator) == book_issued.reader; });		// ��������, ���� �� � ���� ��� ������ �����
			}
			cout << "\n��� ����� ������� ������������ ���������� � ����������.\n";
		}

		library.list_of_readers.DeleteData(iterator);																									// ������� �������� �� ������ ���������
		cout << "\n������������ ������ �������\n";
		library.SaveFiles();																															// ��������� ��� �����
	}
	else
		cout << "\n������������ � ����� ������� ������������� ������ ��� � ����!\n"
		"������� ��������������� ���\n";
}


// ������������� ������ ��������
void EditReader(VirtualLibrary& library)
{
	int num, new_ticket_number;
	string new_surname;
	short input;
	deque<Reader>::iterator iterator;
	cout << "\n������ ���������:\n";
	library.PrintReaders();
	num = Input<int>("������� ����� ������������� ������ ��� ��������� ������������: ", [](auto x) {return x > 0; });

	iterator = library.FindReader([&num](const Reader& reader) { return num == reader.ticket_number; });										// ����� ��������, �������� ����� ��������, �� ������ ������
	if (iterator != library.list_of_readers.End())																								// ���� �������, ��:
	{
		input = Input<short>("������ �������� ����� ������������� ������ ������� ������������?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_ticket_number = Input<int>("������� ����� ����� ������������� ������: ", [](auto x) {return x > 0; });
			(*iterator).ticket_number = new_ticket_number;
		}

		input = Input<short>("������ �������� ������� ������� ������������?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_surname = Input<string>("������� ����� �������: ", [](auto x) {return !x.empty(); });
			(*iterator).surname = new_surname;
		}

		cout << "\n������ ������������ �������� �������\n";
		library.SaveFiles();																													// ��������� ��� �����
	}
	else
		cout << "\n������������ � ����� ������� ������������� ������ ��� � ����!\n"
		"������� ��������������� ���\n";
}


// ������������� ������ ����
void EditListBooks(VirtualLibrary& library)
{
	short input;
	input = Input<short>("�������� ��������:\n"
		"[1] �������� ������ �� ������ ����\n"
		"[2] �������� ������ ������ ����\n", [](auto x) {return x == 1 || x == 2; });
	switch (input)
	{
	case 1:
	{
		input = Input<short>("�������� ��������:\n"
			"[1] �������� ����� � ����������\n"
			"[2] ������� ����� �� ����������\n"
			"[3] ������������� ����� �� ����������\n", [](auto x) {return x == 1 || x == 2 || x == 3; });
		switch (input)
		{
		case 1:
		{
			AddBookInLibrary(library);																// �������� ����� � ����������
			break;
		}
		case 2:
		{
			DeleteBookFromLibrary(library);															// ������� ����� �� ����������
			break;
		}
		case 3:
		{
			EditBookFromLibrary(library);															// ������������� ����� �� ����������
			break;
		}
		default:
		{
			cout << "������ �����!\n";
			break;
		}
		}
		break;
	}
	case 2:
	{
		Reader reader;
		input = Input<short>("�������� ��������:\n"
			"[1] ������ ����� ��������\n"
			"[2] ������� ����� � ��������\n"
			"[3] ������������� ����� ��������\n", [](auto x) {return x == 1 || x == 2 || x == 3; });
		switch (input)
		{
		case 1:
		{
			GiveBookToReader(library);																// ������ ����� ��������
			break;
		}
		case 2:
		{	
			TakeBookFromReader(library);															// ������� ����� � ��������
			break;
		}
		case 3:
		{
			EditReaderBook(library);																// ������������� ����� ��������
			break;
		}
		default:
		{
			cout << "������ �����!\n";
			break;
		}
		}
		break;
	}
	default:
	{
		cout << "������ �����!\n";
		break;
	}
	}
}


// �������� ����� � ����������
void AddBookInLibrary(VirtualLibrary& library)
{
	library.AddBookNotIssued(cin);
	cout << "\n����� ������� ���������!\n";
	library.SaveFiles();
}


// ������� ����� �� ����������
void DeleteBookFromLibrary(VirtualLibrary& library)
{
	string name_book;
	deque<BookNotIssued>::iterator iterator;
	cout << "\n������ ����, ������� ���� � �������:\n";
	library.PrintBooksNotIssued();
	name_book = Input<string>("\n������� �������� �����, ������� ������ �������: ", [](auto x) {return !x.empty(); });

	iterator = library.list_of_books_not_issued.Find([name_book](const BookNotIssued& book) { return name_book == book.name_book; });					// ����� �����, ������� ����� �������, �� ��������
	if (iterator != library.list_of_books_not_issued.End())																								// ���� �������, ��:
	{
		library.list_of_books_not_issued.DeleteData(iterator);																							// �������
		library.SaveFiles();																															// ���������
		cout << "\n����� ������� �������\n";
	}
	else
		cout << "\n���� � ����� ��������� ��� � ����������!\n";
}


// ������������� ����� �� ����������
void EditBookFromLibrary(VirtualLibrary& library)
{
	short input;
	string name_book, new_name_book, new_author, new_publishing_house;
	int new_year_of_publication, new_price;
	deque<BookNotIssued>::iterator iterator;
	cout << "\n������ ����, ������� ���� � �������:\n";
	library.PrintBooksNotIssued();
	name_book = Input<string>("\n������� �������� �����, ������� ������ ��������: ", [](auto x) {return !x.empty(); });

	iterator = library.list_of_books_not_issued.Find([name_book](const BookNotIssued& book) { return name_book == book.name_book; });					// ����� �����, ������� ����� ��������, �� ��������
	if (iterator != library.list_of_books_not_issued.End())																								// ���� �������, ��:
	{
		input = Input<short>("������ �������� ������ �����?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_author = Input<string>("������� ������ ������ �����: ", [](auto x) {return !x.empty(); });
			(*iterator).author = new_author;
		}

		input = Input<short>("������ �������� �������� �����?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_name_book = Input<string>("������� ����� �������� �����: ", [](auto x) {return !x.empty(); });
			(*iterator).name_book = new_name_book;
		}

		input = Input<short>("������ �������� ��� ������� �����?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_year_of_publication = Input<int>("������� ����� ��� ������� �����: ", [](auto x) {return x > 0; });
			(*iterator).year_of_publication = new_year_of_publication;
		}

		input = Input<short>("������ �������� ������������ �����?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_publishing_house = Input<string>("������� ����� ������������ �����: ", [](auto x) {return !x.empty(); });
			(*iterator).publishing_house = new_publishing_house;
		}

		input = Input<short>("������ �������� ���� �����?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_price = Input<int>("������� ����� ���� �����: ", [](auto x) {return x > 0; });
			(*iterator).price = new_price;
		}
		cout << "\n����� ������� ��������\n";
		library.SaveFiles();																													// ��������� ��� �����
	}
	else
		cout << "\n���� � ����� ��������� ��� � ����������!\n";
}


// ������ ����� ��������
void GiveBookToReader(VirtualLibrary& library)
{
	int num;
	short input;
	deque<Reader>::iterator iterator;
	cout << "\n������ ���������:\n";
	library.PrintReaders();
	num = Input<int>("\n������� ����� ������������� ������ ������������, �������� ������ �����: ", [](auto x) {return x > 0; });

	iterator = library.FindReader([&num](const Reader& reader) { return num == reader.ticket_number; });										// ����� �������� �� ������ ������, �������� ������ �����
	if (iterator != library.list_of_readers.End())																								// ���� �������, ��:
		TakeBook(library, (*iterator));																											// �������� ������� "����� �����"
	else
		cout << "\n������������ � ����� ������� ������������� ������ ��� � ����!\n"
		"������� ��������������� ���\n";
}


// ������� ����� � ��������
void TakeBookFromReader(VirtualLibrary& library)
{
	int num;
	short input;
	deque<Reader>::iterator iterator;
	cout << "\n������ ���������:\n";
	library.PrintReaders();
	num = Input<int>("\n������� ����� ������������� ������ ������������, � �������� ��������� �����: ", [](auto x) {return x > 0; });

	iterator = library.FindReader([&num](const Reader& reader) { return num == reader.ticket_number; });										// ����� �������� �� ������ ������, � �������� �������� �����
	if (iterator != library.list_of_readers.End())																								// ���� �������, ��:
		ReturnBook(library, (*iterator));																											// �������� ������� "������� �����"
	else
		cout << "\n������������ � ����� ������� ������������� ������ ��� � ����!\n"
		"������� ��������������� ���\n";
}


// ������������� ����� ��������
void EditReaderBook(VirtualLibrary& library)
{
	short input;
	string name_book, new_name_book, new_author, new_publishing_house;
	int new_year_of_publication, new_price, num;
	deque<Reader>::iterator iterator;
	cout << "\n������ ���������:\n";
	library.PrintReaders();
	num = Input<int>("\n������� ����� ������������� ������ ������������, ����� �������� �� ������ ��������: ", [](auto x) {return x > 0; });

	iterator = library.FindReader([&num](const Reader& reader) { return num == reader.ticket_number; });										// ����� ��������, � �������� ����� �������� �����
	if (iterator != library.list_of_readers.End())																								// ���� �������, ��:
	{
		name_book = Input<string>("������� �������� �����, ������� ������ ��������: ", [](auto x) {return !x.empty(); });
		deque<BookIssued>::iterator it;
		it = library.list_of_books_issued.Find([name_book, iterator](const BookIssued& p)
			{ return (name_book == p.book_not_issued.name_book) && ((*iterator).ticket_number == p.reader.ticket_number); });						// ��� ����� � ����� ��������� � ������ ������ ����
																																				// � ��������, ��� ����� ����������� �������� ������������
		if (it != library.list_of_books_issued.End())																							// ���� �����, ��:
		{
			input = Input<short>("������ �������� ������ �����?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
			if (input == 1)
			{
				new_author = Input<string>("������� ������ ������ �����: ", [](auto x) {return !x.empty(); });
				(*it).book_not_issued.author = new_author;
			}

			input = Input<short>("������ �������� �������� �����?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
			if (input == 1)
			{
				new_name_book = Input<string>("������� ����� �������� �����: ", [](auto x) {return !x.empty(); });
				(*it).book_not_issued.name_book = new_name_book;
			}

			input = Input<short>("������ �������� ��� ������� �����?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
			if (input == 1)
			{
				new_year_of_publication = Input<int>("������� ����� ��� ������� �����: ", [](auto x) {return x > 0; });
				(*it).book_not_issued.year_of_publication = new_year_of_publication;
			}

			input = Input<short>("������ �������� ������������ �����?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
			if (input == 1)
			{
				new_publishing_house = Input<string>("������� ����� ������������ �����: ", [](auto x) {return !x.empty(); });
				(*it).book_not_issued.publishing_house = new_publishing_house;
			}

			input = Input<short>("������ �������� ���� �����?\n""[1] ��\n""[2] ���\n", [](auto x) {return x == 1 || x == 2; });
			if (input == 1)
			{
				new_price = Input<int>("������� ����� ���� �����: ", [](auto x) {return x > 0; });
				(*it).book_not_issued.price = new_price;
			}

			cout << "\n����� ������� ��������\n";
			library.SaveFiles();																					// ���������
		}
		else
			cout << "\n� ��� �� �������� ����� � ����� ���������\n";
	}
	else
		cout << "\n������������ � ����� ������� ������������� ������ ��� � ����!\n"
		"������� ��������������� ���\n";
}


// ����������� ������� ������ ����
void WatchHistoryOfBookIssuance(VirtualLibrary& library)
{
	cout << "\n������� ������/�������� ���� ����� ����������:\n";
	library.PrintHistoryOfBookIssuance(cout);
}


// ����������� ������ ���� � ������� �������� ������
void ViewListBooksWithCurrentStatusOfEach(VirtualLibrary& library)
{
	short input;
	input = Input<short>("�������� ��������:\n"
		"[1] ����������� ��������������� ������\n"
		"[2] ����������� ������ � �����������\n", [](auto x) {return x == 1 || x == 2; });
	switch (input)
	{
	case 1:
	{
		input = Input<short>("�������� ��������:\n"
			"[1] ������ �� ������ ����\n"
			"[2] ������ ������ ����\n", [](auto x) {return x == 1 || x == 2; });
		switch (input)
		{
		case 1:
		{
			SortingBooksNotIssued(library);																	// ���������� �� ������ ����
			break;
		}
		case 2:
		{
			SortingBooksIssued(library);																	// ���������� ������ ����
			break;
		}
		default:
		{
			cout << "������ �����!\n";
			break;
		}
		}
		break;
	}
	case 2:
	{
		input = Input<short>("�������� ��������:\n"
			"[1] ������ �� ������ ����\n"
			"[2] ������ ������ ����\n", [](auto x) {return x == 1 || x == 2; });
		switch (input)
		{
		case 1:
		{
			FilteringBooksNotIssued(library);																// ���������� �� ������ ����
			break;
		}
		case 2:
		{
			FilteringBooksIssued(library);																	// ���������� ������ ����
			break;
		}
		default:
		{
			cout << "������ �����!\n";
			break;
		}
		}
		break;
	}
	default:
	{
		cout << "������ �����!\n";
		break;
	}
	}
}


// ���������� �� ������ ����
void SortingBooksNotIssued(VirtualLibrary& library)
{
	short input;
	input = Input<short>("�������� ��������:\n"
		"[1] ������������� �� ������ �����\n"
		"[2] ������������� �� �������� �����\n"
		"[3] ������������� �� ���� ������� �����\n"
		"[4] ������������� �� ������������ �����\n"
		"[5] ������������� �� ���� �����\n", [](auto x) {return x == 1 || x == 2 || x == 3 || x == 4 || x == 5; });
	switch (input)
	{
	case 1:
	{
		library.list_of_books_not_issued.Sort([](const BookNotIssued& p1, const BookNotIssued& p2) {return p1.author < p2.author; });
		library.list_of_books_not_issued.PrintData();
		break;
	}
	case 2:
	{
		library.list_of_books_not_issued.Sort([](const BookNotIssued& p1, const BookNotIssued& p2) {return p1.name_book < p2.name_book; });
		library.list_of_books_not_issued.PrintData();
		break;
	}
	case 3:
	{
		library.list_of_books_not_issued.Sort([](const BookNotIssued& p1, const BookNotIssued& p2) {return p1.year_of_publication < p2.year_of_publication; });
		library.list_of_books_not_issued.PrintData();
		break;
	}
	case 4:
	{
		library.list_of_books_not_issued.Sort([](const BookNotIssued& p1, const BookNotIssued& p2) {return p1.publishing_house < p2.publishing_house; });
		library.list_of_books_not_issued.PrintData();
		break;
	}
	case 5:
	{
		library.list_of_books_not_issued.Sort([](const BookNotIssued& p1, const BookNotIssued& p2) {return p1.price < p2.price; });
		library.list_of_books_not_issued.PrintData();
		break;
	}
	default:
	{
		cout << "������ �����!\n";
		break;
	}
	}
}


// ���������� ������ ����
void SortingBooksIssued(VirtualLibrary& library)
{
	short input;
	input = Input<short>("�������� ��������:\n"
		"[1] ������������� �� ������ ������������� ������ ������������\n"
		"[2] ������������� �� ������� ������������\n"
		"[3] ������������� �� ������ �����\n"
		"[4] ������������� �� �������� �����\n"
		"[5] ������������� �� ���� ������� �����\n"
		"[6] ������������� �� ������������ �����\n"
		"[7] ������������� �� ���� �����\n"
		"[8] ������������� �� ���� ������\n"
		"[9] ������������� �� ���� ��������\n", [](auto x) {return x == 1 || x == 2 || x == 3 || x == 4 || x == 5 || x == 6 || x == 7 || x == 8 || x == 9; });
	switch (input)
	{
	case 1:
	{
		library.list_of_books_issued.Sort([](const BookIssued& p1, const BookIssued& p2) {return p1.reader.ticket_number < p2.reader.ticket_number; });
		library.list_of_books_issued.PrintData();
		break;
	}
	case 2:
	{
		library.list_of_books_issued.Sort([](const BookIssued& p1, const BookIssued& p2) {return p1.reader.surname < p2.reader.surname; });
		library.list_of_books_issued.PrintData();
		break;
	}
	case 3:
	{
		library.list_of_books_issued.Sort([](const BookIssued& p1, const BookIssued& p2) {return p1.book_not_issued.author < p2.book_not_issued.author; });
		library.list_of_books_issued.PrintData();
		break;
	}
	case 4:
	{
		library.list_of_books_issued.Sort([](const BookIssued& p1, const BookIssued& p2) {return p1.book_not_issued.name_book < p2.book_not_issued.name_book; });
		library.list_of_books_issued.PrintData();
		break;
	}
	case 5:
	{
		library.list_of_books_issued.Sort([](const BookIssued& p1, const BookIssued& p2) {return p1.book_not_issued.year_of_publication < p2.book_not_issued.year_of_publication; });
		library.list_of_books_issued.PrintData();
		break;
	}
	case 6:
	{
		library.list_of_books_issued.Sort([](const BookIssued& p1, const BookIssued& p2) {return p1.book_not_issued.publishing_house < p2.book_not_issued.publishing_house; });
		library.list_of_books_issued.PrintData();
		break;
	}
	case 7:
	{
		library.list_of_books_issued.Sort([](const BookIssued& p1, const BookIssued& p2) {return p1.book_not_issued.price < p2.book_not_issued.price; });
		library.list_of_books_issued.PrintData();
		break;
	}
	case 8:
	{
		library.list_of_books_issued.Sort([](const BookIssued& p1, const BookIssued& p2) {return p1.date_of_issue < p2.date_of_issue; });
		library.list_of_books_issued.PrintData();
		break;
	}
	case 9:
	{
		library.list_of_books_issued.Sort([](const BookIssued& p1, const BookIssued& p2) {return p1.return_date < p2.return_date; });
		library.list_of_books_issued.PrintData();
		break;
	}
	default:
	{
		cout << "������ �����!\n";
		break;
	}
	}
}


// ���������� �� ������ ����
void FilteringBooksNotIssued(VirtualLibrary& library)
{
	short input;
	input = Input<short>("�������� ��������:\n"
		"[1] ������������� �� ������ �����\n"
		"[2] ������������� �� �������� �����\n"
		"[3] ������������� �� ���� ������� �����\n"
		"[4] ������������� �� ������������ �����\n"
		"[5] ������������� �� ���� �����\n", [](auto x) {return x == 1 || x == 2 || x == 3 || x == 4 || x == 5; });
	switch (input)
	{
	case 1:
	{
		string author;
		author = Input<string>("\n������� ������ �����: ", [](auto x) {return !x.empty(); });
		library.list_of_books_not_issued.PrintAll([&author](const BookNotIssued& p) { return p.author == author; }, { cout, "\n" });
		break;
	}
	case 2:
	{
		string name_book;
		name_book = Input<string>("\n������� �������� �����: ", [](auto x) {return !x.empty(); });
		library.list_of_books_not_issued.PrintAll([&name_book](const BookNotIssued& p) { return p.name_book == name_book; }, { cout, "\n" });
		break;
	}
	case 3:
	{
		int year_of_publication;
		year_of_publication = Input<int>("\n������� ��� ������� �����: ", [](auto x) {return x > 0; });
		library.list_of_books_not_issued.PrintAll([&year_of_publication](const BookNotIssued& p) { return p.year_of_publication == year_of_publication; }, { cout, "\n" });
		break;
	}
	case 4:
	{
		string publishing_house;
		publishing_house = Input<string>("\n������� ������������ �����: ", [](auto x) {return !x.empty(); });
		library.list_of_books_not_issued.PrintAll([&publishing_house](const BookNotIssued& p) { return p.publishing_house == publishing_house; }, { cout, "\n" });
		break;
	}
	case 5:
	{
		int price;
		price = Input<int>("\n������� ���� �����: ", [](auto x) {return x > 0; });
		library.list_of_books_not_issued.PrintAll([&price](const BookNotIssued& p) { return p.price == price; }, { cout, "\n" });
		break;
	}
	default:
	{
		cout << "������ �����!\n";
		break;
	}
	}
}


// ���������� ������ ����
void FilteringBooksIssued(VirtualLibrary& library)
{
	short input;
	input = Input<short>("�������� ��������:\n"
		"[1] ������������� �� ������ ������������� ������ ������������\n"
		"[2] ������������� �� ������� ������������\n"
		"[3] ������������� �� ������ �����\n"
		"[4] ������������� �� �������� �����\n"
		"[5] ������������� �� ���� ������� �����\n"
		"[6] ������������� �� ������������ �����\n"
		"[7] ������������� �� ���� �����\n"
		"[8] ������������� �� ���� ������\n"
		"[9] ������������� �� ���� ��������\n", [](auto x) {return x == 1 || x == 2 || x == 3 || x == 4 || x == 5 || x == 6 || x == 7 || x == 8 || x == 9; });
	switch (input)
	{
	case 1:
	{
		int ticket_number;
		ticket_number = Input<int>("\n������� ����� ������������� ������ ������������: ", [](auto x) {return x > 0; });
		library.list_of_books_issued.PrintAll([&ticket_number](const BookIssued& p) { return p.reader.ticket_number == ticket_number; }, { cout, "\n" });
		break;
	}
	case 2:
	{
		string surname;
		surname = Input<string>("\n������� ������� ������������: ", [](auto x) {return !x.empty(); });
		library.list_of_books_issued.PrintAll([&surname](const BookIssued& p) { return p.reader.surname == surname; }, { cout, "\n" });
		break;
	}
	case 3:
	{
		string author;
		author = Input<string>("\n������� ������ �����: ", [](auto x) {return !x.empty(); });
		library.list_of_books_issued.PrintAll([&author](const BookIssued& p) { return p.book_not_issued.author == author; }, { cout, "\n" });
		break;
	}
	case 4:
	{
		string name_book;
		name_book = Input<string>("\n������� �������� �����: ", [](auto x) {return !x.empty(); });
		library.list_of_books_issued.PrintAll([&name_book](const BookIssued& p) { return p.book_not_issued.name_book == name_book; }, { cout, "\n" });
		break;
	}
	case 5:
	{
		int year_of_publication;
		year_of_publication = Input<int>("\n������� ��� ������� �����: ", [](auto x) {return x > 0; });
		library.list_of_books_issued.PrintAll([&year_of_publication](const BookIssued& p) { return p.book_not_issued.year_of_publication == year_of_publication; }, { cout, "\n" });
		break;
	}
	case 6:
	{
		string publishing_house;
		publishing_house = Input<string>("\n������� ������������ �����: ", [](auto x) {return !x.empty(); });
		library.list_of_books_issued.PrintAll([&publishing_house](const BookIssued& p) { return p.book_not_issued.publishing_house == publishing_house; }, { cout, "\n" });
		break;
	}
	case 7:
	{
		int price;
		price = Input<int>("\n������� ���� �����: ", [](auto x) {return x > 0; });
		library.list_of_books_issued.PrintAll([&price](const BookIssued& p) { return p.book_not_issued.price == price; }, { cout, "\n" });
		break;
	}
	case 8:
	{
		Date date_of_issue;
		date_of_issue = Input<Date>("\n������� ���� ������ �����: ", [](auto x) {return x > 0; });
		library.list_of_books_issued.PrintAll([&date_of_issue](const BookIssued& p) { return p.date_of_issue == date_of_issue; }, { cout, "\n" });
		break;
	}
	case 9:
	{
		Date return_date;
		return_date = Input<Date>("\n������� ���� �������� �����: ", [](auto x) {return x > 0; });
		library.list_of_books_issued.PrintAll([&return_date](const BookIssued& p) { return p.return_date == return_date; }, { cout, "\n" });
		break;
	}
	default:
	{
		cout << "������ �����!\n";
		break;
	}
	}
}