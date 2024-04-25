#include "Book.h"
#include "Check.h"


std::istream& ReadReaderFromFile(std::istream& in, Reader& reader)
{
	in >> reader.ticket_number;
	in.ignore(255, '\n');
	std::getline(in, reader.surname);

	return in;
}


std::istream& ReadReaderFromScreen(std::istream& in, Reader& reader)
{
	reader.ticket_number = Input<int>("Номер читательского билета: ", [](auto x) { return x > 0; });
	reader.surname = Input<string>("Фамилия: ", [](auto x) { return !x.empty(); });

	return in;
}


std::istream& operator>>(std::istream& in, Reader& reader)
{
	return (typeid(in) == typeid(std::ifstream) ? ReadReaderFromFile : ReadReaderFromScreen)(in, reader);
}


std::istream& ReadBookNotIssuedFromFile(std::istream& in, BookNotIssued& book_not_issued)
{
	std::getline(in, book_not_issued.author);
	std::getline(in, book_not_issued.name_book);
	in >> book_not_issued.year_of_publication;
	in.ignore(255, '\n');
	std::getline(in, book_not_issued.publishing_house);
	in >> book_not_issued.price;
	if (!in.eof())
		in.ignore(255, '\n');
	return in;
}


std::istream& ReadBookNotIssuedFromScreen(std::istream& in, BookNotIssued& book_not_issued)
{
	book_not_issued.author = Input<string>("Автор книги: ", [](auto x) { return !x.empty(); });
	book_not_issued.name_book = Input<string>("Название книги: ", [](auto x) { return !x.empty(); });
	book_not_issued.year_of_publication = Input<int>("Год публикации книги: ");
	book_not_issued.publishing_house = Input<string>("Издательство книги: ", [](auto x) { return !x.empty(); });
	book_not_issued.price = Input<int>("Цена книги: ");

	return in;
}


std::istream& operator>>(std::istream& in, BookNotIssued& book_not_issued)
{
	return (typeid(in) == typeid(std::ifstream) ? ReadBookNotIssuedFromFile : ReadBookNotIssuedFromScreen)(in, book_not_issued);
}


std::istream& ReadBookIssuedFromFile(std::istream& in, BookIssued& book_issued)
{
	ReadReaderFromFile(in, book_issued.reader);
	ReadBookNotIssuedFromFile(in, book_issued.book_not_issued);
	in >> book_issued.date_of_issue;
	in >> book_issued.return_date;

	return in;
}


std::istream& ReadBookIssuedFromScreen(std::istream& in, BookIssued& book_issued)
{
	ReadReaderFromScreen(in, book_issued.reader);
	ReadBookNotIssuedFromScreen(in, book_issued.book_not_issued);

	book_issued.date_of_issue = Input<Date>("Дата выдачи книги: ");
	book_issued.return_date = Input<Date>("Дата возврата книги: ", [book_issued](auto x) {return x > book_issued.date_of_issue; });

	return in;
}


std::istream& operator>>(std::istream& in, BookIssued& book_issued)
{
	return (typeid(in) == typeid(std::ifstream) ? ReadBookIssuedFromFile : ReadBookIssuedFromScreen)(in, book_issued);
}


std::ostream& WriteReaderToFile(std::ostream& out, const Reader& reader)
{
	out << reader.ticket_number << "\n";
	out << reader.surname << "\n";

	return out;
}


std::ostream& WriteReaderToScreen(std::ostream& out, const Reader& reader)
{
	out << reader.ticket_number << "\n";
	out << reader.surname << "\n";

	return out;
}


std::ostream& operator<<(std::ostream& out, const Reader& reader)
{
	return (typeid(out) == typeid(std::ofstream) ? WriteReaderToFile : WriteReaderToScreen)(out, reader);
}


std::ostream& WriteBookNotIssuedToFile(std::ostream& out, const BookNotIssued& book_not_issued)
{
	out << book_not_issued.author << "\n";
	out << book_not_issued.name_book << "\n";
	out << book_not_issued.year_of_publication << "\n";
	out << book_not_issued.publishing_house << "\n";
	out << book_not_issued.price;

	return out;
}


std::ostream& WriteBookNotIssuedToScreen(std::ostream& out, const BookNotIssued& book_not_issued)
{
	cout << "\nАвтор книги: ";
	out << book_not_issued.author;

	cout << "\nНазвание книги: ";
	out << book_not_issued.name_book;

	cout << "\nГод издания: ";
	out << book_not_issued.year_of_publication;

	cout << "\nИздательство: ";
	out << book_not_issued.publishing_house;

	cout << "\nЦена: ";
	out << book_not_issued.price;

	return out;
}


std::ostream& operator<<(std::ostream& out, const BookNotIssued& book_not_issued)
{
	return (typeid(out) == typeid(std::ofstream) ? WriteBookNotIssuedToFile : WriteBookNotIssuedToScreen)(out, book_not_issued);
}


std::ostream& WriteBookIssuedToFile(std::ostream& out, const  BookIssued& book_issued)
{
	WriteReaderToFile(out, book_issued.reader);
	WriteBookNotIssuedToFile(out, book_issued.book_not_issued);
	out << "\n" << book_issued.date_of_issue;
	out << "\n" << book_issued.return_date;

	return out;
}


std::ostream& WriteBookIssuedToScreen(std::ostream& out, const BookIssued& book_issued)
{
	WriteReaderToScreen(out, book_issued.reader);
	WriteBookNotIssuedToScreen(out, book_issued.book_not_issued);

	cout << "\nДата выдачи книги:\n";
	out << book_issued.date_of_issue;

	cout << "\nДата возврата книги:\n";
	out << book_issued.return_date;

	return out;
}


std::ostream& operator<<(std::ostream& out, const BookIssued& book_issued)
{
	return (typeid(out) == typeid(std::ofstream) ? WriteBookIssuedToFile : WriteBookIssuedToScreen)(out, book_issued);
}


std::istream& ReadDateFromFile(std::istream& in, Date& date)
{
	in >> date.year;
	in >> date.month;
	in >> date.day;

	return in;
}


std::istream& ReadYearFromScreen(std::istream& in, Date& date)
{
	bool error = true;
	while (error)
	{
		cout << "Введите год: ";
		in >> date.year;
		error = date.year > 2023 || date.year < 0;
		if (error)
			cout << "Ошибка! Повторите ввод!\n";
	}

	return in;
}


std::istream& ReadMonthFromScreen(std::istream& in, Date& date)
{
	bool error = true;
	while (error)
	{
		cout << "Введите номер месяца: ";
		in >> date.month;
		error = date.month < 1 || date.month > 12;
		if (error)
			cout << "Ошибка! Повторите ввод!\n";
	}

	return in;
}


std::istream& ReadDayFromScreen(std::istream& in, Date& date)
{
	bool error = true;
	while (error)
	{
		cout << "Введите день: ";
		in >> date.day;
		switch (date.month)
		{
		case 1: case 3: case 5:
		case 7: case 8: case 10:
		case 12:
			error = date.day > 31;
			break;
		case 4: case 6: case 9:
		case 11:
			error = date.day > 30;
			break;
		case 2:
			error = date.day > (date.year % 4 != 0 ? 29 : 28);
			break;
		}
		error = error || date.day < 1;
		if (error)
			cout << "Ошибка! Повторите ввод!\n";
	}

	return in;
}


std::istream& ReadDateFromScreen(std::istream& in, Date& date)
{
	ReadYearFromScreen(in, date);
	ReadMonthFromScreen(in, date);
	ReadDayFromScreen(in, date);

	return in;
}


std::istream& operator>>(std::istream& in, Date& date)
{
	return (typeid(in) == typeid(std::ifstream) ? ReadDateFromFile : ReadDateFromScreen)(in, date);
}


std::ostream& operator<<(std::ostream& out, const Date& date)
{
	if (typeid(out) == typeid(std::ofstream))
	{
		out << date.year << '\n';
		out << date.month << '\n';
		out << date.day;
	}
	else
	{
		out << "Год: " << date.year << '\n';
		out << "Месяц: " << date.month << '\n';
		out << "День: " << date.day;
	}

	return out;
}


Date::Date(size_t days)
{
	const int _Zx = days + 719468; // Shift epoch to 0000-03-01
	// Formula 2-2'
	const int _Century = (_Zx >= 0 ? 4 * _Zx + 3 : 4 * _Zx - 146093) / 146097;
	// Formula 2-2
	const unsigned int _Day_of_century =
		static_cast<unsigned int>(_Zx - ((146097 * _Century) >> 2)); // [0, 36524]
	// Formula 2-1'
	const unsigned int _Year_of_century = (91867 * (_Day_of_century + 1)) >> 25; // [0, 99]
	const int _Yp = static_cast<int>(_Year_of_century) + _Century * 100; // Where March is the first month
	// Formula 2-1
	const unsigned int _Day_of_year = _Day_of_century - ((1461 * _Year_of_century) >> 2); // [0, 365]
	// Formula 1'
	const unsigned int _Mp = (535 * _Day_of_year + 333) >> 14; // [0, 11]
	// Formula 1
	const unsigned int _Day = _Day_of_year - ((979 * _Mp + 19) >> 5) + 1; // [1, 31]
	const unsigned int _Month = _Mp + (_Mp < 10 ? 3 : static_cast<unsigned int>(-9)); // [1, 12]
	year = _Yp + (_Month <= 2);
	month = _Month;
	day = _Day;
}