#include "../includes_usr/fileIO.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stdlib.h>
#include <ctype.h>
using namespace std;
const char CHAR_TO_SEARCH_FOR = ',';

int stringToInt(string &myString) {
	//get rid of spaces
	while (myString.size() && isspace(myString.front()))
		myString.erase(myString.begin());
	//if empty say so
	if (myString[0] == '\0')
		return UNINITIALIZED;

	return atoi(myString.c_str());
}
/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 * 			SUCCESS if all data is loaded
 * */
int loadBooks(std::vector<book> &books, const char *filename) {
	ifstream thisStream;
	thisStream.open(filename);

	if (!thisStream.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}
	std::string line;
	books.clear();
	while (!thisStream.eof()) {
		std::string token;
		book thisBook;
		stringstream ss;

		getline(thisStream, line);
		if (line != "") {
			ss.str(line);

			getline(ss, token, CHAR_TO_SEARCH_FOR);
			thisBook.book_id = stringToInt(token);

			getline(ss, thisBook.title, CHAR_TO_SEARCH_FOR);

			getline(ss, thisBook.author, CHAR_TO_SEARCH_FOR);

			getline(ss, token, CHAR_TO_SEARCH_FOR);
			if (stringToInt(token) == 0) {
				thisBook.state = UNKNOWN;
			}
			if (stringToInt(token) == 1) {
				thisBook.state = IN;
			}
			if (stringToInt(token) == 2) {
				thisBook.state = OUT;
			}

			getline(ss, token, CHAR_TO_SEARCH_FOR);
			thisBook.loaned_to_patron_id = stringToInt(token);

			books.push_back(thisBook);

			ss.clear();
		}
	}
	thisStream.close();
	if (books.empty()) {
		return NO_BOOKS_IN_LIBRARY;
	} else {

		return SUCCESS;
	}

}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 * 			SUCCESS if all data is saved
 * */
int saveBooks(std::vector<book> &books, const char *filename) {
	if (books.empty()) {
		return NO_BOOKS_IN_LIBRARY;
	}

	ofstream newStream;
	newStream.open(filename);
	if (!newStream.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}

	string myBooks;
	for (int var = 0; var < books.size(); var++) {
		myBooks = to_string(books[var].book_id) + "," + books[var].title + ","
				+ books[var].author + "," + to_string(books[var].state) + ","
				+ to_string(books[var].loaned_to_patron_id);
		newStream << myBooks << std::endl;
	}
	newStream.close();
	return SUCCESS;
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 * 			SUCCESS if all data is loaded
 * */
int loadPatrons(std::vector<patron> &patrons, const char *filename) {

	ifstream thisFile;
	thisFile.open(filename);
	if (!thisFile.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}

	patrons.clear();
	std::string line;

	patrons.clear();
	while (!thisFile.eof()) {
		std::string token;
		patron thisPatron;
		stringstream ss;
		getline(thisFile, line);
		if (line != "") {
			ss.str(line);

			getline(ss, token, CHAR_TO_SEARCH_FOR);
			thisPatron.patron_id = stringToInt(token);

			getline(ss, thisPatron.name, CHAR_TO_SEARCH_FOR);

			getline(ss, token, CHAR_TO_SEARCH_FOR);
			thisPatron.number_books_checked_out = stringToInt(token);

			patrons.push_back(thisPatron);

			ss.clear();
		}
	}

	if (patrons.empty()) {
		return NO_PATRONS_IN_LIBRARY;
	} else {
		return SUCCESS;
	}

}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 * 			SUCCESS if all data is saved
 * */
int savePatrons(std::vector<patron> &patrons, const char *filename) {
	if (patrons.empty()) {
		return NO_PATRONS_IN_LIBRARY;
	}

	ofstream newStream;
	newStream.open(filename);
	if (!newStream.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}

	string myPatrons;
	for (int var = 0; var < patrons.size(); var++) {
		myPatrons = to_string(patrons[var].patron_id) + "," + patrons[var].name
				+ "," + to_string(patrons[var].number_books_checked_out);
		newStream << myPatrons << std::endl;
	}
	newStream.close();
	return SUCCESS;

}
