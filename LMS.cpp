#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <unordered_map>
#include <stack>
#include <queue>
#include <windows.h>
#include <ctime>
#include <map>
#include <set>
using namespace std;

string pf_name = "patrons.txt";
string bf_name = "books.txt";
string cf_name = "checked.txt";

string get_f(const char c)
{
	return (c == 'p') ? pf_name : ((c == 'b') ? bf_name : cf_name);
}

void initialize_files()
{
	ifstream file("patrons.txt");
	file.close();
	ifstream file1("books.txt");
	file.close();
	ifstream file2("checked.txt");
	file.close();
}

void logo()
{

	cout << "\n\n";
	cout << "\t\t\t\t     **************\n";
	cout << "\t\t\t\t     *                                      *\n";
	cout << "\t\t\t\t     *                                      *\n";
	cout << "\t\t\t\t     *     ----------------------------     *\n";
	cout << "\t\t\t\t     *      LIBRARY MANAGEMENT SYSTEM       *\n";
	cout << "\t\t\t\t     *                                      *\n";
	cout << "\t\t\t\t     *               MADE BY                *\n";
	cout << "\t\t\t\t     *                                      *\n";
	cout << "\t\t\t\t     *       Talha Durrani [22K-4144]       *\n";
	cout << "\t\t\t\t     *        Asadullah   [22K-4138]        *\n";;
	cout << "\t\t\t\t     *     ----------------------------     *\n";
	cout << "\t\t\t\t     *                                      *\n";
	cout << "\t\t\t\t     *                                      *\n";
	cout << "\t\t\t\t     **************\n";
}

void log_in_as()
{
	cout << "DO YOU WANT TO LOGIN:\n\n";
	cout << "1) As Librarian (Admin)\n";
	cout << "2) As Student\n";
	cout << "0) To EXIT!\n";
	cout << "\nEnter your choice: ";
}

void as_admin()
{
	cout << "\n\n\t\tLOGGED IN AS AN ADMIN\n\n";
	cout << "Press\n";
	cout << "1) Add a book in the catalogue.\n";
	cout << "2) Remove a book from the catalogue.\n";
	cout << "3) Display the catalogue.\n";
	cout << "4) History.\n";
	cout << "5) Remove User.\n";
	cout << "0) To exit.\n";
	cout << "\nEnter your choice: ";
}

void as_student()
{
	cout << "Press\n";
	cout << "1) Login\n";
	cout << "2) Signup\n";
	cout << "0) To go back.\n";
	cout << "Enter your choice: ";
}

void logged_in()
{
	system("CLS");
	cout << "Press\n";
	cout << "1) Book catalogue\n";
	cout << "2) Notification Centre\n";
	cout << "3) Books To return.\n";
	cout << "4) Search Books by ID.\n";
	cout << "0) Log Out.\n";
	cout << "Enter your choice: ";
}

void reversed_stack(stack<string> t_stack)
{
	while (!t_stack.empty())
	{
		cout << t_stack.top() << "\n";
		t_stack.pop();
	}
}

class checkout_record
{
public:
	int book_id;
	int patron_id;
	bool checkout_status;
	int due_date;
};

class book
{
public:
	int id;
	string title;
	string author;
	bool available;

	book() :id(0), available(false) {}
	book(int id, string title, string author, bool available)
		:id(id), title(title), author(author), available(available) {}
};

class node
{
public:
	book bookn;
	node* left;
	node* right;

	node(book bookn) :bookn(bookn), left(nullptr), right(nullptr) {}
};

class bst
{
	node* root;

	node* insert(node* n, book book)
	{
		if (n == nullptr)
			return new node(book);
		if (book.id < n->bookn.id)
			n->left = insert(n->left, book);
		else if (book.id > n->bookn.id)
			n->right = insert(n->right, book);

		return n;
	}

	node* search_by_id(node* n, int book_id)
	{
		if (n == nullptr || n->bookn.id == book_id)
			return n;
		if (book_id < n->bookn.id)
			return search_by_id(n->left, book_id);
		else
			return search_by_id(n->right, book_id);
	}

public:
	bst() : root(nullptr) {}
	void insert(book book) { root = insert(root, book); }
	node* search_by_id(int book_id) { return search_by_id(root, book_id); }
};

class patron
{
public:
	string name;
	int id;

	patron() :name(""), id(0) {}
	patron(string name, int id) :name(name), id(id) {}
};

class library
{
private:
	vector<book> catalogue;
	unordered_map<int, bool> book_copies;
	unordered_map<int, queue<patron>> book_requests;
	stack<checkout_record> checkout_history;
	stack<string> admin_history;

	void load_patrons()
	{
		ifstream file("patron.txt");
		if (file.is_open())
		{
			patrons.clear();

			patron temp;
			while (file >> temp.name >> temp.id)
				patrons.push_back(temp);

			file.close();
		}
	}
	void save_patrons()
	{
		ofstream file("patrons.txt");
		if (file.is_open())
		{
			for (patron& temp : patrons)
				file << temp.name << " " << temp.id << "\n";

			file.close();
		}
	}
public:
	vector<patron> patrons;
	unordered_map<int, vector<int>> checkedout_books;
	unordered_map<int, vector<int>> checked_out;
	bst id_bst;

	unordered_map<int, queue<patron>>& get_book_requests() { return book_requests; }

	void add_book()
	{
		system("cls");
		cout << "\t\t\t\tBOOK ADDITION SECTION\n\n";

		int t_id;
		string t_title, t_auth;
		cout << "Enter details of the book you want to add.\n";
		cout << "Book ID: ";
		cin >> t_id;
		cin.ignore();
		cout << "Book Title: ";
		cin >> t_title;
		cout << "Book Author: ";
		cin >> t_auth;
		book t_book(t_id, t_title, t_auth, true);
		catalogue.push_back(t_book);
		book_copies[t_id] = true;

		ofstream file("books.txt", ios::app);
		if (file.is_open())
		{
			file << t_book.id << "\t" << t_book.title << "\t" << t_book.author << "\t" << "1\n";
			file.close();
			cout << "\nBOOK ADDED SUCCESSFULLY!\n";
		}
		else
			cout << "\nError opening the file to save book information.\n";
		system("pause");
		admin_history.push("Admin added a book: " + t_book.title);
	}

	void remove_book(int book_id)
	{
		ifstream file("books.txt");
		bool found = false;

		if (file.is_open())
		{
			int t_id;
			string t_title, t_auth;
			bool t_aval;

			vector<string> lines;

			while (file >> t_id >> ws && getline(file, t_title, '\t') && getline(file, t_auth, '\t') >> t_aval)
			{
				if (t_id == book_id)
					found = true;
				else
				{
					stringstream ss;
					ss << t_id << "\t" << t_title << "\t" << t_auth << "\t" << (t_aval ? "1" : "0");
					lines.push_back(ss.str());
				}
			}
			file.close();

			ofstream writef("books.txt");
			if (writef.is_open())
			{
				for (string& line : lines)
				{
					writef << line << "\n";
				}
				writef.close();
			}
			else
			{
				cout << "Error opening the file to update book information.\n";
				system("pause");
				return;
			}
		}
		else
		{
			cout << "Error opening the file to update book information.\n";
			system("pause");
			return;
		}

		if (found)
		{
			cout << "\nBOOK REMOVED SUCCESSFULLY!\n";
			book_copies.erase(book_id);
			admin_history.push("Admin removed a book with ID: " + to_string(book_id));
		}
		else
			cout << "\nThere is no book with such Id.\n";

		system("pause");
	}

	void add_user(patron& patron)
	{
		patrons.push_back(patron);
		save_patrons();
		cout << "\nUSER ADDED SUCCESSFULLY!\n";
		system("pause");
	}

	void remove_user(int id)
	{
		auto found = find_if(patrons.begin(), patrons.end(), [id](patron& patron) {return patron.id == id; });

		if (found != patrons.end())
		{
			patrons.erase(found);
			save_patrons();
			cout << "\nUSER REMOVED SUCCESSFULLY!\n";
			admin_history.push("Admin removed a user with ID: " + to_string(id));
		}
		else
			cout << "\nThere is no user with such ID.\n";
		system("pause");
	}

	void display_catalogue()
	{
		system("cls");
		cout << "\n\t\t\t\tBOOK CATALOGUE\n\n";
		ifstream file("books.txt");
		if (file.is_open())
		{
			cout << "Book ID\t\tTitle\t\tAuthor\t\tAvailability\n---------------------------------------------------------------\n";

			int t_id;
			string t_title, t_auth;
			bool t_aval;

			while (file >> t_id >> t_title >> t_auth >> t_aval)
			{
				cout << t_id << "\t\t" << t_title << "\t\t" << t_auth << "\t\t" << (t_aval ? "Available" : "Not Avaialable") << "\n";
			}
			file.close();
			system("pause");
		}
		else
		{
			cout << "Error opening the file to read book information.\n";
			system("pause");
			return;
		}
	}

	void request_book(int t_id, patron& patron)
	{
		book_requests[t_id].push(patron);
		cout << "Your request for the book has been added. You will be notified when it's available.\n";
	}

	void checkout_book(int b_id, patron& patron)
	{
		ifstream file(get_f('b'));
		bool avail = false;

		if (file.is_open())
		{
			int t_id;
			string t_title, t_auth;
			bool t_aval;

			vector<string> lines;

			while (file >> t_id >> ws && getline(file, t_title, '\t') && getline(file, t_auth, '\t') >> t_aval)
			{
				if (t_id == b_id && t_aval)
				{
					t_aval = false;
					book_copies[b_id] = true;
					avail = true;
				}

				stringstream ss;
				ss << t_id << "\t" << t_title << "\t" << t_auth << "\t" << (t_aval ? "1" : "0");
				lines.push_back(ss.str());
			}
			file.close();

			ofstream file(get_f('b'));
			if (file.is_open())
			{
				for (string line : lines)
					file << line << "\n";
				file.close();
			}
			else
			{
				cout << "Error opening the file to update book information.\n";
				system("pause");
				return;
			}
		}
		else
		{
			cout << "Error opening the file to update book information.\n";
			system("pause");
			return;
		}
		if (avail && book_copies[b_id])
		{
			book_copies[b_id] = false;
			checkout_record tr;
			tr.book_id = b_id;
			tr.patron_id = patron.id;
			tr.checkout_status = true;

			checkedout_books[patron.id].push_back(b_id);
			ofstream file(get_f('c'), ios::app);
			if (file.is_open())
			{
				file << patron.id << "\t" << b_id << "\n";
				file.close();
			}
			else
				cout << "\nError opening the file to save checked-out book information.\n";

			time_t now = time(0);
			tm* localTime = localtime(&now);
			localTime->tm_mday += 7;
			mktime(localTime);
			tr.due_date = localTime->tm_mday;
			checkout_history.push(tr);
			cout << "\nBook checked out successfully! Due date : " << tr.due_date << " th of this month.\n";
		}
		else
		{
			cout << "\nSorry, the book is currently not available.\n Do you want to request this book? (1 for yes, 0 for no):";
			int c;
			cin >> c;
			if (c)
				request_book(b_id, patron);
		}
		system("pause");
	}

	void update_availability(int b_id, bool avail)
	{
		ifstream file(get_f('b'));
		bool found = false;

		if (file.is_open())
		{
			int t_id;
			string t_title, t_auth;
			bool t_aval;

			vector<string> lines;

			while (file >> t_id >> ws && getline(file, t_title, '\t') && getline(file, t_auth, '\t') >> t_aval)
			{
				if (t_id == b_id && t_aval)
				{
					t_aval = avail;
					found = true;
				}

				stringstream ss;
				ss << t_id << "\t" << t_title << "\t" << t_auth << "\t" << (t_aval ? "1" : "0");
				lines.push_back(ss.str());
			}
			file.close();

			ofstream file(get_f('b'));
			if (file.is_open())
			{
				for (string line : lines)
					file << line << "\n";
				file.close();
			}
			else
			{
				cout << "Error opening the file to update book information.\n";
				system("pause");
				return;
			}
		}
		else
		{
			cout << "Error opening the file to update book information.\n";
			system("pause");
			return;
		}
		if (!found)
			cout << "Error updating book availability. Book ID not found.\n";
	}

	vector<int>& get_checkedout_books(int id)
	{
		auto it = checked_out.find(id);
		if (it != checked_out.end())
			return it->second;
	}

	void set_duedate(int b_id, int date)
	{
		stack<checkout_record> t_stack;
		while (!checkout_history.empty())
		{
			checkout_record record = checkout_history.top();
			checkout_history.pop();
			if (record.book_id == b_id)
				record.due_date = date;

			t_stack.push(record);
		}
		while (!t_stack.empty())
		{
			checkout_history.push(t_stack.top());
			t_stack.pop();
		}
		cout << "\nDue date set successfully!\n";
		system("pause");
	}

	void notifications()
	{
		system("cls");
		cout << "\n\t\t\t\tNOTIFICATIONS\n\n";

		auto& all_requests = get_book_requests();
		for (auto it = all_requests.begin(); it != all_requests.end(); ++it)
		{
			int b_id = it->first;
			queue<patron>& request_queue = it->second;
			if (!request_queue.empty())
			{
				cout << "Book ID: " << b_id << " has pending requests.\n";
				cout << "Patron(s) waiting for the book:\n";

				queue<patron> t_queue = request_queue;
				while (!t_queue.empty())
				{
					cout << "\t- " << t_queue.front().name << " (ID: " << t_queue.front().id << ")\n";
					t_queue.pop();
				}

				stack<checkout_record> t_stack;
				bool found = false;

				while (!checkout_history.empty())
				{
					checkout_record tr = checkout_history.top();
					checkout_history.pop();
					if (tr.book_id == b_id && tr.checkout_status)
					{
						found = true;
						cout << "Due date: " << tr.due_date << " th of this month.\n";
					}
					t_stack.push(tr);
				}
				while (!t_stack.empty())
				{
					checkout_history.push(t_stack.top());
					t_stack.pop();
				}
				cout << "-----------------------------------------\n";
			}
		}
	}

	void checkout_process(patron& patron)
	{
		display_catalogue();
		int b_id;
		cout << "Enter the ID of the book you want to checkout (0 to go back): ";
		cin >> b_id;
		if (b_id != 0)
			checkout_book(b_id, patron);
	}

	void display_histroy()
	{
		system("cls");
		cout << "\n\t\t\t\tRECENT ACTIVITY\n\n";
		if (admin_history.empty())
			cout << "No recent activity.\n";
		else
		{
			stack<string>t_stack = admin_history;
			reversed_stack(t_stack);
		}
		system("pause");
	}

	void file_bst(bst& book_bst)
	{
		ifstream file(get_f('b'));
		if (file.is_open())
		{
			int t_id;
			string t_title, t_auth;
			bool t_aval;

			vector<string> lines;

			while (file >> t_id >> ws && getline(file, t_title, '\t') && getline(file, t_auth, '\t') >> t_aval)
			{
				book book(t_id, t_title, t_auth, t_aval);
				book_bst.insert(book);
			}
			file.close();
		}
	}

	void search_id(int b_id)
	{
		node* node = id_bst.search_by_id(b_id);
		if (node == NULL) {
			cout << "\nBOOK NOT FOUND!\n";
			cout << endl;
			system("pause");
		}
		else {
			cout << "\nBOOK FOUND!\n";
			cout << "ID: " << node->bookn.id << "\n";
			cout << "Title: " << node->bookn.title << "\n";
			cout << "Author: " << node->bookn.author << "\n";
			cout << "Availability: " << (node->bookn.available ? "Available" : "Not Available") << "\n";
		}
	}

	void display_checkedout(patron& patron)
	{
		system("cls");
		cout << "\n\t\t\t\tCHECKED-OUT BOOKS\n\n";

		auto it = checkedout_books.find(patron.id);
		if (it != checkedout_books.end())
		{
			vector<int>& t_checks = it->second;
			if (t_checks.empty())
				cout << "No books checked out by " << patron.name << " (ID: " << patron.id << ").\n";
			else
			{
				cout << "Book ID\t\tTitle\t\tAuthor\n";
				cout << "---------------------------------------------------------------\n";
				for (int b_id : t_checks)
				{
					auto find_id = find_if(catalogue.begin(), catalogue.end(), [b_id](const book& book) { return book.id == b_id; });
					if (find_id != catalogue.end())
						cout << b_id << "\t\t" << find_id->title << "\t\t" << find_id->author << "\n";
				}
			}
		}
		else
		{
			cout << "No books checked out by " << patron.name << " (ID: " << patron.id << ").\n";
		}
		system("pause");
	}

	void return_book(int b_id)
	{
		auto it = find_if(catalogue.begin(), catalogue.end(), [b_id](const book& book) { return book.id == b_id; });

		if (it != catalogue.end())
		{
			it->available = true;
			ifstream file(get_f('b'));
			bool found = false;
			if (file.is_open())
			{
				int t_id;
				string t_title, t_auth;
				bool t_aval;

				vector<string> lines;

				while (file >> t_id >> ws && getline(file, t_title, '\t') && getline(file, t_auth, '\t') >> t_aval)
				{
					if (t_id == b_id)
					{
						t_aval = true;
						found = true;
					}

					stringstream ss;
					ss << t_id << "\t" << t_title << "\t" << t_auth << "\t" << (t_aval ? "1" : "0");
					lines.push_back(ss.str());
				}
				file.close();

				ofstream file(get_f('b'));
				if (file.is_open())
				{
					for (string line : lines)
						file << line << "\n";
					file.close();
				}
				else
				{
					cout << "Error opening the file to update book information.\n";
					system("pause");
					return;
				}
			}
			else
			{
				cout << "Error opening the file to update book information.\n";
				system("pause");
				return;
			}
			if (found)
			{
				cout << "\nBook returned successfully!\n";
				for (auto& patron_books : checkedout_books)
				{
					auto& b_ids = patron_books.second;
					b_ids.erase(remove(b_ids.begin(), b_ids.end(), b_id), b_ids.end());
				}
				ifstream file(get_f('c'));
				vector<string> lines;
				if (file.is_open())
				{
					int p_id, c_id;
					while (file >> p_id >> c_id)
					{
						if (c_id != b_id)
						{
							stringstream ss;
							ss << p_id << "\t" << c_id;
							lines.push_back(ss.str());
						}
					}
					file.close();

					ofstream file(get_f('c'));
					if (file.is_open())
					{
						for (string& line : lines)
							file << line << "\n";

						file.close();
					}
					else
					{
						cout << "\nError opening the file\n";
					}
				}
				else
				{
					cout << "\nError opening the file\n";
				}
			}
			else
			{
				cout << "\nThere is no book with such ID.\n";
			}
		}
		else
		{
			cout << "\nThere is no book with such ID.\n";
		}
	}
};

int main()
{
	library lib;
	int choice1;
	initialize_files();
	logo();
loginas:
	log_in_as();
	cin >> choice1;
	system("cls");
	int pass_count = 0;
	while (choice1 != 0)
	{
		if (choice1 == 1)
		{
			string pass;
			cout << "Enter password to login as an admin (pass = admin123): ";
			cin >> pass;
			system("cls");

			while (pass_count < 5)
			{
				if (pass == "admin123")
				{
					int choice2;
					as_admin();
					cin >> choice2;
					system("cls");
					while (choice2 != 0)
					{
						if (choice2 == 1)
						{
							lib.add_book();
						}
						else if (choice2 == 2)
						{
							system("cls");
							cout << "\t\t\t\tBOOK REMOVAL SECTION\n";
							int book_id;
							cout << "Enter id of the book you want to remove: ";
							cin >> book_id;
							lib.remove_book(book_id);
						}
						else if (choice2 == 3)
						{
							lib.display_catalogue();
						}
						else if (choice2 == 4)
						{
							lib.display_histroy();
						}
						else if (choice2 == 5)
						{
							system("cls");
							int user_id;
							cout << "Enter the ID of the user to remove: ";
							cin >> user_id;
							lib.remove_user(user_id);
						}
						else if (choice2 == 0)
						{
							break;
						}
						system("cls");
						as_admin();
						cin >> choice2;
					}
					if (choice2 == 0)
					{
						system("cls");
						goto loginas;
					}
				}
				else
				{
					pass_count++;
					system("cls");
					cout << "Wrong password! Please try again.\n";
					cout << "Enter password to login as an admin: ";
					cin >> pass;
				}
			}
			if (pass_count >= 5)
			{
				system("cls");
				goto loginas;
			}
		}
		else if (choice1 == 2)
		{
			system("cls");
			int s_choice;
		logout:
			as_student();
			cin.ignore();
			cin >> s_choice;
			switch (s_choice)
			{
			case 1:
			{
				int user_id;
				cout << "Enter your user ID:";
				cin >> user_id;
				ifstream file(get_f('p'));
				bool found = false;
				if (file.is_open())
				{
					patron ptrn;
					while (file >> ptrn.name >> ptrn.id)
					{
						if (ptrn.id == user_id)
						{
							found = true;
							cout << "Login successful!\n";
						login2:
							logged_in();
							int opt;
							cin >> opt;
							while (opt != 0)
							{
								if (opt == 1)
								{
									lib.checkout_process(ptrn);
								}
								else if (opt == 2)
								{
									system("cls");
									lib.notifications();
									system("pause");
								}
								if (opt == 3)
								{
									system("cls");
									int b_id;
									lib.display_checkedout(ptrn);
									cout << "\n\nEnter the ID of the book you want to return (0- back): \n\n";
									cin.ignore();
									cin >> b_id;
									if (b_id == 0)
									{
										goto login2;
									}
									lib.return_book(b_id);
									system("cls");
								}
								if (opt == 4)
								{
									system("cls");
									lib.file_bst(lib.id_bst);
									int b_id;
									cout << "Enter the id of the book you want to search: ";
									cin.ignore();
									cin >> b_id;
									lib.search_id(b_id);
									cout << "\n\n\n\n";
									system("pause");
									system("cls");
								}
								if (opt == 0)
								{
									break;
								}
								logged_in();
								cin >> opt;
							}
							if (opt == 0)
							{
								system("cls");
								goto logout;
							}
							cout << "\n\n\n\n";
							system("pause");
							system("cls");
							break;
						}
					}
					file.close();
				}
				if (!found)
				{
					cout << "User not found. Please signup.\n";
					system("pause");
					system("cls");
					goto logout;
				}
				system("pause");
				break;
			}
			case 2:
			{
				string user;
				int id;
				do
				{
					cout << "Enter your name: ";
					cin.ignore();
					cin >> user;
					cout << "Enter your desired user ID: ";
					cin.ignore();
					cin >> id;
					auto exist = find_if(lib.patrons.begin(), lib.patrons.end(), [id](patron& patron) {return patron.id == id; });
					if (exist != lib.patrons.end())
						cout << "User ID already exists. Please choose a different one.\n";
					else
						break;
				} while (true);
				patron ptrn(user, id);
				lib.add_user(ptrn);
				cout << "Signup successful! You can now login.\n";
				system("pause");
				break;
			}
			default:
				break;
			}
		}
		else
		{
			system("CLS");
			cout << "Invalid choice! Please try again.\n";
		}
		system("cls");
		log_in_as();
		cin >> choice1;
	}
	return 0;
}