// Exercise 4 - Real Email
// Find a real email message file (containing real email messages)
// and modify example to extract subject lines from sender names given as input.

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <regex>

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
	regex pat{ R"(^From:(\s)?(.+)$)" };
	for (const auto& x : *m)
	{
		smatch matches;
		if (regex_search(x, matches, pat))
		{
			s = matches[2];
			return true;
		}
	}
	return false;
}

string find_subject(const Message* m)
{
	regex pat{ R"(^Subject:(\s)?(.+)$)" };

	for (const auto& x : *m)
	{
		smatch matches;
		if (regex_search(x, matches, pat))
			return matches[2];
	}
	return "";
}

//-----------------------------------------------------------------------------

int main()
try
{
	Mail_file mfile{ "mail2.txt" };

	multimap<string, const Message*> senders;

	for (const auto& m : mfile)
	{
		string s;
		if (find_sender(&m, s))
			senders.insert(make_pair(s, &m));
	}

	string sndr;
	while (true)
	{
		cout << "Enter sender name: ";
		getline(cin, sndr);
		if (sndr == "quit")
			return 0;
		auto pp = senders.equal_range(sndr);
		if (pp.first == pp.second)
			cout << "Sender not found!\n";
		else
			cout << "Subjects:\n";
		for (auto p = pp.first; p != pp.second; ++p)
		{
			string s = find_subject(p->second);
			if (s.empty())
				cout << "No subject!\n";
			else
				cout << s << endl;
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