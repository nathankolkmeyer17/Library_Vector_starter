#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iostream>

#include "../includes_usr/library.h"
#include "../includes_usr/datastructures.h"
#include "../includes_usr/fileIO.h"
using namespace std;

//NOTE: please ensure patron and book data are loaded from disk before calling the following
//NOTE: also make sure you save patron and book data to disk any time you make a change to them
//NOTE: for files where data is stored see constants.h BOOKFILE and PATRONFILE
vector<book> books;
vector<patron> patrons;
/*
 * clear books and patrons containers
 * then reload them from disk 
 */
void reloadAllData() {
	patrons.clear();
	loadBooks(books, BOOKFILE.c_str());
	loadPatrons(patrons, PATRONFILE.c_str());
}

/* checkout a book to a patron
 * first load books and patrons from disk
 * make sure patron enrolled (patronid is assigned to a patron in patrons container)
 * make sure book in collection (bookid is assigned to a book in books container)
 * 
 * see if patron can check out any more books 
 * 	if not return TOO_MANY_OUT patron has the MAX_BOOKS_ALLOWED_OUT
 * 	
 * if so then check the book out to the patron, set the following fields for the book in the
 * books container
 * book.loaned_to_patron_id = patronid;
 * book.state = OUT;
 * 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 *         PATRON_NOT_ENROLLED
 * 		   BOOK_NOT_IN_COLLECTION
 *         TOO_MANY_OUT patron has the max number of books allowed checked out
 */
int checkout(int bookid, int patronid) {
	reloadAllData();
	book thisBook;
	patron thisPatron;
	bool bookHere;
	bool patronEn;
	int j;
	int h;
	for (j = 0 ;j < patrons.size(); j++) {
		if (patrons[j].patron_id == patronid) {
			patronEn = true;
			if (patrons[j].number_books_checked_out == MAX_BOOKS_ALLOWED_OUT) {
				return TOO_MANY_OUT;
			}
		}
	}
	for (h = 0; h < books.size(); h++) {
		if (books[h].book_id == bookid) {
			bookHere = true;
			if(patronEn == true){
				books[h].loaned_to_patron_id = patronid;
				books[h].state = OUT;
			}
		}
	}

	if (patronEn == true && bookHere == true) {
		thisBook.loaned_to_patron_id = patronid;
		thisBook.state = OUT;
		thisPatron.number_books_checked_out = thisPatron.number_books_checked_out+1;
	}
	if (patronEn == false) {
		return PATRON_NOT_ENROLLED;
	}
	if (bookHere == false) {
		return BOOK_NOT_IN_COLLECTION;
	}
	else{
			saveBooks(books, BOOKFILE.c_str());
			savePatrons(patrons, PATRONFILE.c_str());
			return SUCCESS;
	}
}

/* check a book back in 
 * first load books and patrons from disk
 * make sure book in collection (bookid is assigned to a book in books container)
 *  	
 * if so find the the patron the book is checked out to and decrement his/hers number_books_checked_out
 * then check the book back in by marking the book.loaned_to_patron_id = NO_ONE and the book.state = IN;
 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 * 		   BOOK_NOT_IN_COLLECTION
 */
int checkin(int bookid) {
	reloadAllData();
	bool bookIn;
	int bookID;
	for (int i = 0; i < books.size(); i++) {
		if (books[i].book_id == bookid) {
			bookIn = true;
			books[i].loaned_to_patron_id = NO_ONE;
			books[i].state = IN;
			bookID = books[i].loaned_to_patron_id;
		}
	}
	if (bookIn == true) {
		for (int i = 0; i < patrons.size(); i++) {
			if (patrons[i].patron_id == bookID) {
				patrons[i].number_books_checked_out =
						patrons[i].number_books_checked_out - 1;
			}
		}
		saveBooks(books, BOOKFILE.c_str());
		savePatrons(patrons, PATRONFILE.c_str());
		return SUCCESS;
	}
	return BOOK_NOT_IN_COLLECTION;

}

/*
 * enroll a patron, duplicate names are fine as patrons are uniquely identified by their patronid
 * first load books and patrons from disk
 * create a patron object, initialize its fields as appropriate, assign him/her the nextPatronID
 * then push the patron onto the patrons container
 * save all changes to the patrons container to disk
 * return 
 *    the patron_id of the person added
 */
int enroll(std::string &name) {
	reloadAllData();
	patron newPat;
	newPat.name = name;

	if (patrons.size() == 0) {
		newPat.patron_id = 0;
	} else {
		newPat.patron_id = patrons.size();
	}
	patrons.push_back(newPat);
	savePatrons(patrons, PATRONFILE.c_str());
	return newPat.patron_id;

}

/*
 * the number of books in the books container
 * (ie. if 3 books returns 3)
 * 
 */
int numbBooks() {
	loadBooks(books, BOOKFILE.c_str());
	return books.size();
}

/*
 * the number of patrons in the patrons container
 * (ie. if 3 patrons returns 3)
 */
int numbPatrons() {
	loadPatrons(patrons, PATRONFILE.c_str());
	return patrons.size();
}

/*the number of books patron has checked out
 *
 *returns a positive number indicating how many books are checked out 
 *        or PATRON_NOT_ENROLLED         
 */
int howmanybooksdoesPatronHaveCheckedOut(int patronid) {
	reloadAllData();
	for (int i = 0; i < patrons.size(); i++) {
		if (patrons[i].patron_id == patronid) {
			return patrons[i].number_books_checked_out;
		}
	}
	return PATRON_NOT_ENROLLED;

}

/* search through patrons container to see if patronid is there
 * if so returns the name associated with patronid in the variable name
 * 
 * returns SUCCESS found it and name in name
 *         PATRON_NOT_ENROLLED no patron with this patronid
 */
int whatIsPatronName(std::string &name, int patronid) {
	reloadAllData();
	int i = 0;
	while (i != patrons.size()) {
		if (patrons[i].patron_id == patronid) {
			name = patrons[i].name;
			return SUCCESS;
		}
		i++;
	}
	return PATRON_NOT_ENROLLED;
}

