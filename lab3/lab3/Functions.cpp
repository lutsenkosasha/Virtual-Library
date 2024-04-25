#include "Functions.h"
#include <chrono>
#include "Check.h"

using namespace std::chrono;


// Главное меню
void MainMenu(VirtualLibrary& library)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	library.InputListOfReadersFromFile();		// Ввод списка читателей из файла
	library.InputBooksNotIssuedFromFile();		// Ввод списка не взятых книг из файла
	library.InputBooksIssuedFromFile();			// Ввод списка взятых книг из файла

	short choice_1;
	choice_1 = Input<short>("[1] Начать работу программы\n"
		"[2] Завершить работу программы\n", [](auto x) {return x == 1 || x == 2; });
	while (choice_1 != 2 && choice_1 == 1)
	{
		short choice_2;
		choice_2 = Input<short>("Выберите режим работы:\n"
			"[1] Читатель\n"
			"[2] Библиотекарь\n"
			"[0] Выход\n", [](auto x) {return x == 1 || x == 2 || x == 0; });
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
			cout << "Ошибка ввода!\n";
			break;
		}
		}
	}
}


// Режим пользователя
void ReaderMode(VirtualLibrary& library, Reader& reader)
{
	int num;
	deque<Reader>::iterator iterator;
	cout << "\nСписок читателей:\n";
	library.PrintReaders();
	num = Input<int>("Введите номер вашего читательского билета: ", [](auto x) {return x > 0; });
	iterator = library.FindReader([&num](const Reader& reader) { return num == reader.ticket_number; });				// Поиск читателя по номеру билета
	if (iterator != library.list_of_readers.End())																		// Если нашелся, то:
	{
		reader = *iterator;
		short input;
		input = Input<short>("Выберите действие:\n"
			"[1] Посмотреть список доступных книг\n"
			"[2] Найти книгу по заданному критерию (любое поле или набор полей)\n"
			"[3] Взять книгу\n"
			"[4] Вернуть книгу\n", [](auto x) {return x == 1 || x == 2 || x == 3 || x == 4; });
		switch (input)
		{
		case 1:
		{
			library.PrintBooksNotIssued();													// Печать списка не взятых книг
			break;
		}
		case 2:
		{
			BookSearchMenu(library);														// Меню поиска книги
			break;
		}
		case 3:
		{
			TakeBook(library, reader);														// Взять книгу
			break;
		}
		case 4:
		{
			ReturnBook(library, reader);													// Вернуть книгу
			break;
		}
		default:
		{
			cout << "Ошибка ввода!\n";
			break;
		}
		}
	}
	else
		cout << "\nПользователя с таким номером читательского билета нет в базе!\n"
		"Обратитесь к библиотекарю с просьбой вас зарегистрировать\n";
}


// Меню поиска книги
void BookSearchMenu(VirtualLibrary& library)
{
	bool array[5] = { false, false, false, false, false };
	deque<BookNotIssued>::iterator iterator;
	short input;
	string author, name_book, publishing_house;
	int year_of_publication, price;

	input = Input<short>("Найти книгу по автору?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
	if (input == 1)
	{
		author = Input<string>("Введите автора книги: ", [](auto x) {return !x.empty(); });
		array[0] = true;
	}

	input = Input<short>("Найти книгу по названию?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
	if (input == 1)
	{
		name_book = Input<string>("Введите название книги: ", [](auto x) {return !x.empty(); });
		array[1] = true;
	}

	input = Input<short>("Найти книгу по году издания?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
	if (input == 1)
	{
		year_of_publication = Input<int>("Введите год издания книги: ", [](auto x) {return x > 0; });
		array[2] = true;
	}

	input = Input<short>("Найти книгу по издательству?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
	if (input == 1)
	{
		publishing_house = Input<string>("Введите издательство книги: ", [](auto x) {return !x.empty(); });
		array[3] = true;
	}

	input = Input<short>("Найти книгу по цене?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
	if (input == 1)
	{
		price = Input<int>("Введите цену книги: ", [](auto x) {return x > 0; });
		array[4] = true;
	}

	iterator = library.list_of_books_not_issued.Find([author, name_book, year_of_publication, publishing_house, price, array](const BookNotIssued& p) {				// Поиск по всем введенным критериям
		return	(!array[0] || author == p.author)
			&& (!array[1] || name_book == p.name_book)
			&& (!array[2] || year_of_publication == p.year_of_publication)
			&& (!array[3] || publishing_house == p.publishing_house)
			&& (!array[4] || price == p.price);
	});

	if (iterator != library.list_of_books_not_issued.End())							// Если книга нашлась, то печатаем на экран
		library.list_of_books_not_issued.PrintData(iterator);
	else
		cout << "\nПо данным критериям мы не нашли подходящей для вас книги\n";
}


// Взять книгу
void TakeBook(VirtualLibrary& library, Reader& reader)
{
	deque<BookIssued>::iterator iterator;
	string name_book;
	name_book = Input<string>("Введите название книги, которую хотите взять: ", [](auto x) {return !x.empty(); });
	iterator = library.list_of_books_issued.Find([name_book](const BookIssued& p) { return name_book == p.book_not_issued.name_book; });			// Ищу книгу с таким названием среди взятых книг
	if (iterator != library.list_of_books_issued.End())																								// Если книга нашлась в списке взятых книг, то:
		cout << "\nЭту книгу уже кто-то взял! Извините :(\n";
	else
	{
		deque<BookNotIssued>::iterator it;
		it = library.list_of_books_not_issued.Find([name_book](const BookNotIssued& p) { return name_book == p.name_book; });						// Ищу книгу с таким названием в списке не взятых книг
		if (it != library.list_of_books_not_issued.End())																							// Если нашел, то:
		{
			Date return_date;
			Date ymd(floor<days>(system_clock::now()).time_since_epoch().count());																	// Дата сегодняшнего дня
			BookIssued book_issued;
			book_issued.reader = reader;
			book_issued.book_not_issued = (*it);
			book_issued.date_of_issue = ymd;
			cout << "\nВведите дату возврата книги:\n";
			cin >> return_date;
			book_issued.return_date = return_date;

			library.list_of_books_issued.Add(book_issued);																							// Добавляю взятую книгу в список взятых книг

			library.list_of_books_not_issued.DeleteData(it);																						// Из списка не взятых книг удаляю книгу, которую беру
			cout << "\nВы успешно взяли книгу\n";

			library.history_of_book_issuance.push_back("Пользователь " + reader.surname + " взял книгу под названием " + book_issued.book_not_issued.name_book);		// Добавляю запись в историю выдачи книг

			library.SaveFiles();																													// Сохраняем все файлы сразу
		}
		else
			cout << "\nКниг с таким названием в нашей библиотеке нет.\n";
	}
}


// Вернуть книгу
void ReturnBook(VirtualLibrary& library, Reader& reader)
{
	deque<BookNotIssued>::iterator iterator;
	string name_book;
	name_book = Input<string>("Введите название книги, которую хотите вернуть: ", [](auto x) {return !x.empty(); });
	iterator = library.list_of_books_not_issued.Find([name_book](const BookNotIssued& p) { return name_book == p.name_book; });						// Ищу книгу с таким названием среди не взятых книг
	if (iterator != library.list_of_books_not_issued.End())																							// Если книга нашлась в списке не взятых книг, то:
		cout << "\nЭту книгу уже вернули!\n";
	else
	{
		deque<BookIssued>::iterator it;
		it = library.list_of_books_issued.Find([name_book, &reader](const BookIssued& p)
			{ return (name_book == p.book_not_issued.name_book) && (reader.ticket_number == p.reader.ticket_number); });							// Ищу книгу с таким названием в списке взятых книг
																																					// и проверяю, что книга принадлежит текущему пользователю

		if (it != library.list_of_books_issued.End())																								// Если нашел, то:
		{
			BookNotIssued book_not_issued;
			book_not_issued = (*it).book_not_issued;

			library.list_of_books_not_issued.Add(book_not_issued);																					// Добавляю книгу в список не взятых книг

			library.list_of_books_issued.DeleteData(it);																							// Из списка взятых книг удаляю книгу, которую возвращаю
			cout << "\nВы успешно вернули книгу\n";

			library.history_of_book_issuance.push_back("Пользователь " + reader.surname + " вернул книгу под названием " + book_not_issued.name_book);		// Добавляю запись в историю выдачи книг

			library.SaveFiles();																													// Сохраняем все файлы сразу
		}
		else
			cout << "\nУ вас не числится книги с таким названием\n";
	}
}


// Режим библиотекаря
void LibrarianMode(VirtualLibrary& library)
{
	short input;
	input = Input<short>("Выберите действие:\n"
		"[1] Редактировать список читателей\n"
		"[2] Редактировать список книг\n"
		"[3] Просмотреть историю выдачи книг\n"
		"[4] Просмотреть список книг с текущим статусом каждой\n", [](auto x) {return x == 1 || x == 2 || x == 3 || x == 4; });
	switch (input)
	{
	case 1:
	{
		EditListReaders(library);																// Редактировать список читателей
		break;
	}
	case 2:
	{
		EditListBooks(library);																	// Редактировать список книг
		break;
	}
	case 3:
	{
		WatchHistoryOfBookIssuance(library);													// Просмотреть историю выдачи книг
		break;
	}
	case 4:
	{
		ViewListBooksWithCurrentStatusOfEach(library);											// Просмотреть список книг с текущим статусом каждой
		break;
	}
	default:
	{
		cout << "Ошибка ввода!\n";
		break;
	}
	}
}


// Редактировать список читателей
void EditListReaders(VirtualLibrary& library)
{
	short input;
	input = Input<short>("Выберите действие:\n"
		"[1] Зарегистрировать нового читателя\n"
		"[2] Удалить данные читателя\n"
		"[3] Редактировать данные читателя\n", [](auto x) {return x == 1 || x == 2 || x == 3; });
	switch (input)
	{
	case 1:
	{
		RegisterNewReader(library);									// Зарегистрировать нового читателя
		break;
	}
	case 2:
	{
		DeleteReader(library);										// Удалить данные читателя
		break;
	}
	case 3:
	{
		EditReader(library);										// Редактировать данные читателя
		break;
	}
	default:
	{
		cout << "Ошибка ввода!\n";
		break;
	}
	}
}


// Зарегистрировать нового читателя
void RegisterNewReader(VirtualLibrary& library)
{
	library.AddReader(cin);
	cout << "\nНовый пользователь добавлен успешно!\n";
	library.SaveFiles();
}


// Удалить данные читателя
void DeleteReader(VirtualLibrary& library)
{
	int num;
	deque<Reader>::iterator iterator;
	cout << "\nСписок читателей:\n";
	library.PrintReaders();
	num = Input<int>("Введите номер читательского билета для удаления пользователя: ", [](auto x) {return x > 0; });

	iterator = library.FindReader([&num](const Reader& reader) { return num == reader.ticket_number; });												// Поиск читателя, которого хотим удалить, по номеру билета
	if (iterator != library.list_of_readers.End())																										// Если нашелся, то:
	{
		deque<BookIssued>::iterator it;
		it = library.list_of_books_issued.Find([iterator](const BookIssued& book_issued) { return (*iterator) == book_issued.reader; });				// Проверяем, есть ли у этого пользователя взятые книги
		if (it != library.list_of_books_issued.End())																									// Если есть, то
		{
			cout << "\nУ данного читателя есть книги на руках!"
				"\nПеред удалением читателя произойдет автоматический возврат всех книг в библиотеку.\n";
			while (it != library.list_of_books_issued.End())
			{
				library.list_of_books_issued.DeleteData(it);																							// Из списка взятых книг удаляю книгу, которую возвращаю

				BookNotIssued book_not_issued;
				book_not_issued = (*it).book_not_issued;

				library.list_of_books_not_issued.Add(book_not_issued);																					// Добавляю книгу в список не взятых книг

				it = library.list_of_books_issued.Find([iterator](const BookIssued& book_issued) { return (*iterator) == book_issued.reader; });		// Проверяю, есть ли у него еще взятые книги
			}
			cout << "\nВсе книги данного пользователя возвращены в библиотеку.\n";
		}

		library.list_of_readers.DeleteData(iterator);																									// Удаляем читателя из списка читателей
		cout << "\nПользователь удален успешно\n";
		library.SaveFiles();																															// Сохраняем все файлы
	}
	else
		cout << "\nПользователя с таким номером читательского билета нет в базе!\n"
		"Сначала зарегистрируйте его\n";
}


// Редактировать данные читателя
void EditReader(VirtualLibrary& library)
{
	int num, new_ticket_number;
	string new_surname;
	short input;
	deque<Reader>::iterator iterator;
	cout << "\nСписок читателей:\n";
	library.PrintReaders();
	num = Input<int>("Введите номер читательского билета для изменения пользователя: ", [](auto x) {return x > 0; });

	iterator = library.FindReader([&num](const Reader& reader) { return num == reader.ticket_number; });										// Поиск читателя, которого хотим изменить, по номеру билета
	if (iterator != library.list_of_readers.End())																								// Если нашелся, то:
	{
		input = Input<short>("Хотите изменить номер читательского билета данного пользователя?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_ticket_number = Input<int>("Введите новый номер читательского билета: ", [](auto x) {return x > 0; });
			(*iterator).ticket_number = new_ticket_number;
		}

		input = Input<short>("Хотите изменить фамилию данного пользователя?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_surname = Input<string>("Введите новую фамилию: ", [](auto x) {return !x.empty(); });
			(*iterator).surname = new_surname;
		}

		cout << "\nДанные пользователя изменены успешно\n";
		library.SaveFiles();																													// Сохраняем все файлы
	}
	else
		cout << "\nПользователя с таким номером читательского билета нет в базе!\n"
		"Сначала зарегистрируйте его\n";
}


// Редактировать список книг
void EditListBooks(VirtualLibrary& library)
{
	short input;
	input = Input<short>("Выберите действие:\n"
		"[1] Изменить список не взятых книг\n"
		"[2] Изменить список взятых книг\n", [](auto x) {return x == 1 || x == 2; });
	switch (input)
	{
	case 1:
	{
		input = Input<short>("Выберите действие:\n"
			"[1] Добавить книгу в библиотеку\n"
			"[2] Удалить книгу из библиотеки\n"
			"[3] Редактировать книгу из библиотеки\n", [](auto x) {return x == 1 || x == 2 || x == 3; });
		switch (input)
		{
		case 1:
		{
			AddBookInLibrary(library);																// Добавить книгу в библиотеку
			break;
		}
		case 2:
		{
			DeleteBookFromLibrary(library);															// Удалить книгу из библиотеки
			break;
		}
		case 3:
		{
			EditBookFromLibrary(library);															// Редактировать книгу из библиотеки
			break;
		}
		default:
		{
			cout << "Ошибка ввода!\n";
			break;
		}
		}
		break;
	}
	case 2:
	{
		Reader reader;
		input = Input<short>("Выберите действие:\n"
			"[1] Выдать книгу читателю\n"
			"[2] Забрать книгу у читателя\n"
			"[3] Редактировать книгу читателя\n", [](auto x) {return x == 1 || x == 2 || x == 3; });
		switch (input)
		{
		case 1:
		{
			GiveBookToReader(library);																// Выдать книгу читателю
			break;
		}
		case 2:
		{	
			TakeBookFromReader(library);															// Забрать книгу у читателя
			break;
		}
		case 3:
		{
			EditReaderBook(library);																// Редактировать книгу читателя
			break;
		}
		default:
		{
			cout << "Ошибка ввода!\n";
			break;
		}
		}
		break;
	}
	default:
	{
		cout << "Ошибка ввода!\n";
		break;
	}
	}
}


// Добавить книгу в библиотеку
void AddBookInLibrary(VirtualLibrary& library)
{
	library.AddBookNotIssued(cin);
	cout << "\nКнига успешно добавлена!\n";
	library.SaveFiles();
}


// Удалить книгу из библиотеки
void DeleteBookFromLibrary(VirtualLibrary& library)
{
	string name_book;
	deque<BookNotIssued>::iterator iterator;
	cout << "\nСписок книг, которые есть в наличии:\n";
	library.PrintBooksNotIssued();
	name_book = Input<string>("\nВведите название книги, которую хотите удалить: ", [](auto x) {return !x.empty(); });

	iterator = library.list_of_books_not_issued.Find([name_book](const BookNotIssued& book) { return name_book == book.name_book; });					// Поиск книги, которую хотим удалить, по названию
	if (iterator != library.list_of_books_not_issued.End())																								// Если нашлась, то:
	{
		library.list_of_books_not_issued.DeleteData(iterator);																							// Удаляем
		library.SaveFiles();																															// Сохраняем
		cout << "\nКнига успешно удалена\n";
	}
	else
		cout << "\nКниг с таким названием нет в библиотеке!\n";
}


// Редактировать книгу из библиотеки
void EditBookFromLibrary(VirtualLibrary& library)
{
	short input;
	string name_book, new_name_book, new_author, new_publishing_house;
	int new_year_of_publication, new_price;
	deque<BookNotIssued>::iterator iterator;
	cout << "\nСписок книг, которые есть в наличии:\n";
	library.PrintBooksNotIssued();
	name_book = Input<string>("\nВведите название книги, которую хотите изменить: ", [](auto x) {return !x.empty(); });

	iterator = library.list_of_books_not_issued.Find([name_book](const BookNotIssued& book) { return name_book == book.name_book; });					// Поиск книги, которую хотим изменить, по названию
	if (iterator != library.list_of_books_not_issued.End())																								// Если нашлась, то:
	{
		input = Input<short>("Хотите изменить автора книги?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_author = Input<string>("Введите нового автора книги: ", [](auto x) {return !x.empty(); });
			(*iterator).author = new_author;
		}

		input = Input<short>("Хотите изменить название книги?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_name_book = Input<string>("Введите новое название книги: ", [](auto x) {return !x.empty(); });
			(*iterator).name_book = new_name_book;
		}

		input = Input<short>("Хотите изменить год издания книги?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_year_of_publication = Input<int>("Введите новый год издания книги: ", [](auto x) {return x > 0; });
			(*iterator).year_of_publication = new_year_of_publication;
		}

		input = Input<short>("Хотите изменить издательство книги?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_publishing_house = Input<string>("Введите новое издательство книги: ", [](auto x) {return !x.empty(); });
			(*iterator).publishing_house = new_publishing_house;
		}

		input = Input<short>("Хотите изменить цену книги?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
		if (input == 1)
		{
			new_price = Input<int>("Введите новую цену книги: ", [](auto x) {return x > 0; });
			(*iterator).price = new_price;
		}
		cout << "\nКнига успешно изменена\n";
		library.SaveFiles();																													// Сохраняем все файлы
	}
	else
		cout << "\nКниг с таким названием нет в библиотеке!\n";
}


// Выдать книгу читателю
void GiveBookToReader(VirtualLibrary& library)
{
	int num;
	short input;
	deque<Reader>::iterator iterator;
	cout << "\nСписок читателей:\n";
	library.PrintReaders();
	num = Input<int>("\nВведите номер читательского билета пользователя, которому выдаёте книгу: ", [](auto x) {return x > 0; });

	iterator = library.FindReader([&num](const Reader& reader) { return num == reader.ticket_number; });										// Поиск читателя по номеру билета, которому выдаем книгу
	if (iterator != library.list_of_readers.End())																								// Если нашелся, то:
		TakeBook(library, (*iterator));																											// Вызываем функцию "взять книгу"
	else
		cout << "\nПользователя с таким номером читательского билета нет в базе!\n"
		"Сначала зарегистрируйте его\n";
}


// Забрать книгу у читателя
void TakeBookFromReader(VirtualLibrary& library)
{
	int num;
	short input;
	deque<Reader>::iterator iterator;
	cout << "\nСписок читателей:\n";
	library.PrintReaders();
	num = Input<int>("\nВведите номер читательского билета пользователя, у которого забираете книгу: ", [](auto x) {return x > 0; });

	iterator = library.FindReader([&num](const Reader& reader) { return num == reader.ticket_number; });										// Поиск читателя по номеру билета, у которого забираем книгу
	if (iterator != library.list_of_readers.End())																								// Если нашелся, то:
		ReturnBook(library, (*iterator));																											// Вызываем функцию "вернуть книгу"
	else
		cout << "\nПользователя с таким номером читательского билета нет в базе!\n"
		"Сначала зарегистрируйте его\n";
}


// Редактировать книгу читателя
void EditReaderBook(VirtualLibrary& library)
{
	short input;
	string name_book, new_name_book, new_author, new_publishing_house;
	int new_year_of_publication, new_price, num;
	deque<Reader>::iterator iterator;
	cout << "\nСписок читателей:\n";
	library.PrintReaders();
	num = Input<int>("\nВведите номер читательского билета пользователя, книгу которого вы хотите изменить: ", [](auto x) {return x > 0; });

	iterator = library.FindReader([&num](const Reader& reader) { return num == reader.ticket_number; });										// Поиск читателя, у которого хотим изменить книгу
	if (iterator != library.list_of_readers.End())																								// Если нашелся, то:
	{
		name_book = Input<string>("Введите название книги, которую хотите изменить: ", [](auto x) {return !x.empty(); });
		deque<BookIssued>::iterator it;
		it = library.list_of_books_issued.Find([name_book, iterator](const BookIssued& p)
			{ return (name_book == p.book_not_issued.name_book) && ((*iterator).ticket_number == p.reader.ticket_number); });						// Ищу книгу с таким названием в списке взятых книг
																																				// и проверяю, что книга принадлежит текущему пользователю
		if (it != library.list_of_books_issued.End())																							// Если нашел, то:
		{
			input = Input<short>("Хотите изменить автора книги?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
			if (input == 1)
			{
				new_author = Input<string>("Введите нового автора книги: ", [](auto x) {return !x.empty(); });
				(*it).book_not_issued.author = new_author;
			}

			input = Input<short>("Хотите изменить название книги?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
			if (input == 1)
			{
				new_name_book = Input<string>("Введите новое название книги: ", [](auto x) {return !x.empty(); });
				(*it).book_not_issued.name_book = new_name_book;
			}

			input = Input<short>("Хотите изменить год издания книги?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
			if (input == 1)
			{
				new_year_of_publication = Input<int>("Введите новый год издания книги: ", [](auto x) {return x > 0; });
				(*it).book_not_issued.year_of_publication = new_year_of_publication;
			}

			input = Input<short>("Хотите изменить издательство книги?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
			if (input == 1)
			{
				new_publishing_house = Input<string>("Введите новое издательство книги: ", [](auto x) {return !x.empty(); });
				(*it).book_not_issued.publishing_house = new_publishing_house;
			}

			input = Input<short>("Хотите изменить цену книги?\n""[1] Да\n""[2] Нет\n", [](auto x) {return x == 1 || x == 2; });
			if (input == 1)
			{
				new_price = Input<int>("Введите новую цену книги: ", [](auto x) {return x > 0; });
				(*it).book_not_issued.price = new_price;
			}

			cout << "\nКнига успешно изменена\n";
			library.SaveFiles();																					// Сохраняем
		}
		else
			cout << "\nУ вас не числится книги с таким названием\n";
	}
	else
		cout << "\nПользователя с таким номером читательского билета нет в базе!\n"
		"Сначала зарегистрируйте его\n";
}


// Просмотреть историю выдачи книг
void WatchHistoryOfBookIssuance(VirtualLibrary& library)
{
	cout << "\nИстория выдачи/возврата книг нашей библиотеки:\n";
	library.PrintHistoryOfBookIssuance(cout);
}


// Просмотреть список книг с текущим статусом каждой
void ViewListBooksWithCurrentStatusOfEach(VirtualLibrary& library)
{
	short input;
	input = Input<short>("Выберите действие:\n"
		"[1] Просмотреть отсортированный список\n"
		"[2] Просмотреть список с фильтрацией\n", [](auto x) {return x == 1 || x == 2; });
	switch (input)
	{
	case 1:
	{
		input = Input<short>("Выберите действие:\n"
			"[1] Список не взятых книг\n"
			"[2] Список взятых книг\n", [](auto x) {return x == 1 || x == 2; });
		switch (input)
		{
		case 1:
		{
			SortingBooksNotIssued(library);																	// Сортировка не взятых книг
			break;
		}
		case 2:
		{
			SortingBooksIssued(library);																	// Сортировка взятых книг
			break;
		}
		default:
		{
			cout << "Ошибка ввода!\n";
			break;
		}
		}
		break;
	}
	case 2:
	{
		input = Input<short>("Выберите действие:\n"
			"[1] Список не взятых книг\n"
			"[2] Список взятых книг\n", [](auto x) {return x == 1 || x == 2; });
		switch (input)
		{
		case 1:
		{
			FilteringBooksNotIssued(library);																// Фильтрация не взятых книг
			break;
		}
		case 2:
		{
			FilteringBooksIssued(library);																	// Фильтрация взятых книг
			break;
		}
		default:
		{
			cout << "Ошибка ввода!\n";
			break;
		}
		}
		break;
	}
	default:
	{
		cout << "Ошибка ввода!\n";
		break;
	}
	}
}


// Сортировка не взятых книг
void SortingBooksNotIssued(VirtualLibrary& library)
{
	short input;
	input = Input<short>("Выберите действие:\n"
		"[1] Отсортировать по автору книги\n"
		"[2] Отсортировать по названию книги\n"
		"[3] Отсортировать по году издания книги\n"
		"[4] Отсортировать по издательству книги\n"
		"[5] Отсортировать по цене книги\n", [](auto x) {return x == 1 || x == 2 || x == 3 || x == 4 || x == 5; });
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
		cout << "Ошибка ввода!\n";
		break;
	}
	}
}


// Сортировка взятых книг
void SortingBooksIssued(VirtualLibrary& library)
{
	short input;
	input = Input<short>("Выберите действие:\n"
		"[1] Отсортировать по номеру читательского билета пользователя\n"
		"[2] Отсортировать по фамилии пользователя\n"
		"[3] Отсортировать по автору книги\n"
		"[4] Отсортировать по названию книги\n"
		"[5] Отсортировать по году издания книги\n"
		"[6] Отсортировать по издательству книги\n"
		"[7] Отсортировать по цене книги\n"
		"[8] Отсортировать по дате выдачи\n"
		"[9] Отсортировать по дате возврата\n", [](auto x) {return x == 1 || x == 2 || x == 3 || x == 4 || x == 5 || x == 6 || x == 7 || x == 8 || x == 9; });
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
		cout << "Ошибка ввода!\n";
		break;
	}
	}
}


// Фильтрация не взятых книг
void FilteringBooksNotIssued(VirtualLibrary& library)
{
	short input;
	input = Input<short>("Выберите действие:\n"
		"[1] Отфильтровать по автору книги\n"
		"[2] Отфильтровать по названию книги\n"
		"[3] Отфильтровать по году издания книги\n"
		"[4] Отфильтровать по издательству книги\n"
		"[5] Отфильтровать по цене книги\n", [](auto x) {return x == 1 || x == 2 || x == 3 || x == 4 || x == 5; });
	switch (input)
	{
	case 1:
	{
		string author;
		author = Input<string>("\nВведите автора книги: ", [](auto x) {return !x.empty(); });
		library.list_of_books_not_issued.PrintAll([&author](const BookNotIssued& p) { return p.author == author; }, { cout, "\n" });
		break;
	}
	case 2:
	{
		string name_book;
		name_book = Input<string>("\nВведите название книги: ", [](auto x) {return !x.empty(); });
		library.list_of_books_not_issued.PrintAll([&name_book](const BookNotIssued& p) { return p.name_book == name_book; }, { cout, "\n" });
		break;
	}
	case 3:
	{
		int year_of_publication;
		year_of_publication = Input<int>("\nВведите год издания книги: ", [](auto x) {return x > 0; });
		library.list_of_books_not_issued.PrintAll([&year_of_publication](const BookNotIssued& p) { return p.year_of_publication == year_of_publication; }, { cout, "\n" });
		break;
	}
	case 4:
	{
		string publishing_house;
		publishing_house = Input<string>("\nВведите издательство книги: ", [](auto x) {return !x.empty(); });
		library.list_of_books_not_issued.PrintAll([&publishing_house](const BookNotIssued& p) { return p.publishing_house == publishing_house; }, { cout, "\n" });
		break;
	}
	case 5:
	{
		int price;
		price = Input<int>("\nВведите цену книги: ", [](auto x) {return x > 0; });
		library.list_of_books_not_issued.PrintAll([&price](const BookNotIssued& p) { return p.price == price; }, { cout, "\n" });
		break;
	}
	default:
	{
		cout << "Ошибка ввода!\n";
		break;
	}
	}
}


// Фильтрация взятых книг
void FilteringBooksIssued(VirtualLibrary& library)
{
	short input;
	input = Input<short>("Выберите действие:\n"
		"[1] Отфильтровать по номеру читательского билета пользователя\n"
		"[2] Отфильтровать по фамилии пользователя\n"
		"[3] Отфильтровать по автору книги\n"
		"[4] Отфильтровать по названию книги\n"
		"[5] Отфильтровать по году издания книги\n"
		"[6] Отфильтровать по издательству книги\n"
		"[7] Отфильтровать по цене книги\n"
		"[8] Отфильтровать по дате выдачи\n"
		"[9] Отфильтровать по дате возврата\n", [](auto x) {return x == 1 || x == 2 || x == 3 || x == 4 || x == 5 || x == 6 || x == 7 || x == 8 || x == 9; });
	switch (input)
	{
	case 1:
	{
		int ticket_number;
		ticket_number = Input<int>("\nВведите номер читательского билета пользователя: ", [](auto x) {return x > 0; });
		library.list_of_books_issued.PrintAll([&ticket_number](const BookIssued& p) { return p.reader.ticket_number == ticket_number; }, { cout, "\n" });
		break;
	}
	case 2:
	{
		string surname;
		surname = Input<string>("\nВведите фамилию пользователя: ", [](auto x) {return !x.empty(); });
		library.list_of_books_issued.PrintAll([&surname](const BookIssued& p) { return p.reader.surname == surname; }, { cout, "\n" });
		break;
	}
	case 3:
	{
		string author;
		author = Input<string>("\nВведите автора книги: ", [](auto x) {return !x.empty(); });
		library.list_of_books_issued.PrintAll([&author](const BookIssued& p) { return p.book_not_issued.author == author; }, { cout, "\n" });
		break;
	}
	case 4:
	{
		string name_book;
		name_book = Input<string>("\nВведите название книги: ", [](auto x) {return !x.empty(); });
		library.list_of_books_issued.PrintAll([&name_book](const BookIssued& p) { return p.book_not_issued.name_book == name_book; }, { cout, "\n" });
		break;
	}
	case 5:
	{
		int year_of_publication;
		year_of_publication = Input<int>("\nВведите год издания книги: ", [](auto x) {return x > 0; });
		library.list_of_books_issued.PrintAll([&year_of_publication](const BookIssued& p) { return p.book_not_issued.year_of_publication == year_of_publication; }, { cout, "\n" });
		break;
	}
	case 6:
	{
		string publishing_house;
		publishing_house = Input<string>("\nВведите издательство книги: ", [](auto x) {return !x.empty(); });
		library.list_of_books_issued.PrintAll([&publishing_house](const BookIssued& p) { return p.book_not_issued.publishing_house == publishing_house; }, { cout, "\n" });
		break;
	}
	case 7:
	{
		int price;
		price = Input<int>("\nВведите цену книги: ", [](auto x) {return x > 0; });
		library.list_of_books_issued.PrintAll([&price](const BookIssued& p) { return p.book_not_issued.price == price; }, { cout, "\n" });
		break;
	}
	case 8:
	{
		Date date_of_issue;
		date_of_issue = Input<Date>("\nВведите дату выдачи книги: ", [](auto x) {return x > 0; });
		library.list_of_books_issued.PrintAll([&date_of_issue](const BookIssued& p) { return p.date_of_issue == date_of_issue; }, { cout, "\n" });
		break;
	}
	case 9:
	{
		Date return_date;
		return_date = Input<Date>("\nВведите дату возврата книги: ", [](auto x) {return x > 0; });
		library.list_of_books_issued.PrintAll([&return_date](const BookIssued& p) { return p.return_date == return_date; }, { cout, "\n" });
		break;
	}
	default:
	{
		cout << "Ошибка ввода!\n";
		break;
	}
	}
}