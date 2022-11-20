// Stroustrup - Programming Principles & Practice
//
// Chapter 23 Exercise 1 - More Mail File Fun
// Get the email file example to run and test it with a larger file of emails.
// Try to break the program with extra addresses.
//
// Exercise 2 - Input Subject Search
// Add a multimap to hold subjects. Take keyboard input and 


#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

using namespace std;

using Line_iter = vector<string>::const_iterator;	//new style

class Message
{
	Line_iter first;
	Line_iter last;
public:
	Message(Line_iter p1, Line_iter p2) : first{ p1 }, last{ p2 } {}
	Line_iter begin() const { return first; }
	Line_iter end() const { return last; }
	//...
};

typedef vector<Message>::const_iterator Mess_iter;	//old style

struct Mail_file
{
	string name;
	vector<string> lines;
	vector<Message> m;
	Mail_file(const string& n);
	Mess_iter begin() { return m.begin(); }
	Mess_iter end() { return m.end(); }
};

Mail_file::Mail_file(const string& n)
{
	ifstream in{ n };

	if (!in)
	{
		cerr << "No file " << n << endl;
		exit(1);
	}

	for (string s; getline(in, s);)
		lines.push_back(s);

	auto first = lines.begin();
	for (auto p = lines.begin(); p != lines.end(); ++p)
		if (*p == "----") // end of message
		{
			if (p != first) // don't count consecutive ----'s
				m.push_back(Message{ first,p });
			first = p + 1;
		}
	if (m.empty())
		throw runtime_error("No messages in the file " + n);
}
//-----------------------------------------------------------------------------

int is_prefix(const string& s, const string& p)
{
	int n = p.size();
	if (string(s, 0, n) == p)
		return n;
	return 0;
}

bool find_sender(const Message* m, string& s)
{
	for (Line_iter li = m->begin(); li != m->end(); ++li)
		if (int n = is_prefix(*li, "From: "))
		{
			if (is_prefix(*(li + 1), "From: "))
				throw runtime_error("More than 1 sender!");
			s = string(*li, n);
			return true;
		}

	return false;
}

string find_subject(const Message* m)
{
	for (const auto& x : *m)
		if (int n = is_prefix(x, "Subject: "))
			return string(x, n);
	return "";
}
//-----------------------------------------------------------------------------
int main()
try
{
	Mail_file mfile{ "mail2.txt" };

	multimap<string, const Message*> senders, subjects;

	for (const auto& m : mfile)
	{
		string s;
		if (find_sender(&m, s))
			senders.insert(make_pair(s, &m));
		s = find_subject(&m);
		if (s.size())
			subjects.insert(make_pair(s, &m));
	}

	string subj;
	while (true)
	{
		cout << "Enter subject: ";
		getline(cin, subj);
		if (!cin)
			throw runtime_error("cin isn't good");
		auto pp = subjects.equal_range(subj);
		if (pp.first == pp.second)
			cout << "Subject not found!\n";
		for (auto p = pp.first; p != pp.second; ++p)
		{
			cout << endl;
			for (auto i = p->second->begin(); i != p->second->end(); ++i)
				cout << *i << endl;
		}
	}
	cout << "The file contains " << mfile.m.size() << " messages." << endl;
}
catch (std::exception& e) {
	cerr << "Exception: " << e.what() << '\n';
	return 1;
}
catch (...) {
	cerr << "Unknown exception\n";
	return 2;
}