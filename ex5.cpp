// Exercise 5 - Huge Mail Test
// Find a mail file of thousands of messages and time it as written with a multimap and an unordered_multimap.

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <regex>
#include <chrono>

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

bool find_sender(const Message* m, string& s)
{
	regex pat{ R"(^From:\s(.+)$)" };
	for (const auto& x : *m)
	{
		smatch matches;
		if (regex_match(x, matches, pat))
		{
			s = matches[1];
			return true;
		}
	}
	return false;
}

string find_subject(const Message* m)
{
	regex pat{ R"(^Subject:\s(.+)$)" };

	for (const auto& x : *m)
	{
		smatch matches;
		if (regex_match(x, matches, pat))
			return matches[1];
	}
	return "";
}

//-----------------------------------------------------------------------------

int main()
try
{
	Mail_file mfile{ "mail3.txt" };

	multimap<string, const Message*> senders;
	unordered_multimap<string, const Message*> usenders;

	auto t1 = chrono::system_clock::now();
	for (const auto& m : mfile)
	{
		string s;
		if (find_sender(&m, s))
			senders.insert(make_pair(s, &m));
	}
	auto t2 = chrono::system_clock::now();

	auto ut1 = chrono::system_clock::now();
	for (const auto& m : mfile)
	{
		string s;
		if (find_sender(&m, s))
			usenders.insert(make_pair(s, &m));
	}
	auto ut2 = chrono::system_clock::now();

	cout << "The file contains " << mfile.m.size() << " messages." << endl;
	cout << "The multimap contains " << senders.size() << " messages." << endl;
	cout << "The unordered_multimap contains " << usenders.size() << " messages." << endl;
	cout << "Task with the multimap took " << chrono::duration_cast<chrono::microseconds>(t2 - t1).count() << " microseconds\n";
	cout << "Task with the unordered_multimap took " << chrono::duration_cast<chrono::microseconds>(ut2 - ut1).count() << " microseconds\n";
}
catch (std::exception& e) {
	cerr << "Exception: " << e.what() << '\n';
	return 1;
}
catch (...) {
	cerr << "Unknown exception\n";
	return 2;
}