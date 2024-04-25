#pragma once
#include <iostream>
#include <string>
#include <fstream>

using std::cout;
using std::cin;
using std::string;


struct Date
{
	int year, month, day;

	friend std::istream& operator>>(std::istream& in, Date& date);
	friend std::ostream& operator<<(std::ostream& out, const Date& date);
	auto operator <=> (const Date& other) const = default;
	Date() : year(0), month(1), day(1) {}
	Date(size_t days);
};


struct Reader
{
	int ticket_number;
	string surname;

	auto operator <=> (const Reader& other) const = default;
	friend std::istream& operator>>(std::istream& in, Reader& reader);
	friend std::ostream& operator<<(std::ostream& out, const Reader& reader);
};


struct BookNotIssued
{
	string author;
	string name_book;
	int year_of_publication;
	string publishing_house;
	int price;

	auto operator <=> (const BookNotIssued& other) const = default;
	friend std::istream& operator>>(std::istream& in, BookNotIssued& book_not_issued);
	friend std::ostream& operator<<(std::ostream& out, const BookNotIssued& book_not_issued);
};


struct BookIssued
{
	Reader reader;
	BookNotIssued book_not_issued;
	Date date_of_issue;
	Date return_date;

	auto operator <=> (const BookIssued& other) const = default;
	friend std::istream& operator>>(std::istream& in, BookIssued& book_issued);
	friend std::ostream& operator<<(std::ostream& out, const BookIssued& book_issued);
};