// Exercise 3 - Regex Update
// Modify the chapter example to use regular expressions to find the subject and sender.

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

int is_prefix(const string& s, const string& p)
{
	int n = p.size();
	if (string(s, 0, n) == p)
		return n;
	return 0;
}

bool find_sender(const Message* m, string& s)
{
	regex pat{ R"(^From:(\s*)?(.+)$)" };
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
	regex pat{ R"(^Subject:(\s*)?(.+)$)" };

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

	multimap<string, const Message*> mm;

	for (const auto& m : mfile)
	{
		string s;
		if (find_sender(&m, s))
			mm.insert(make_pair(s, &m));
	}

	auto pp = mm.equal_range("Todd Fehr <toddgod@example.com>");
	for (auto p = pp.first; p != pp.second; ++p)
		cout << find_subject(p->second) << endl;

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