// Chapter 23 Exercise 6 - Date Finder
// Write a program that finds dates in a text file. Write out each line that contains at least one date.

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

bool has_date(const string& line)
{
	// month or day first, separated by / or . or -
	regex pat{ R"(\d{1,2}[-/\.]\d{1,2}[-/\.]\d{4})" };
	// January 31, 2008
	regex pat2{ R"((January|February|March|April|May|June|July|August|September|October|November|December)\s+\d{1,2},\s+\d{4}\D)" };
	// 31 January 2008
	regex pat3{ R"(\d{1,2}\s+(January|February|March|April|May|June|July|August|September|October|November|December)\s+\d{4}\D)" };

	smatch matches;
	return regex_search(line, matches, pat) || regex_search(line, matches, pat2) || regex_search(line, matches, pat3);
}

int main()
try
{
	string filename{ "dates.txt" };
	ifstream ifs{ filename };
	if (!ifs)
		throw runtime_error("Cannot open " + filename);

	int line_number = 0;
	string line;
	while (getline(ifs,line))
	{
		line_number++;
		if (has_date(line))
			cout << line_number << ": " << line << endl;
	}

	return 0;
}
catch (exception& e)
{
	cerr << "Exception: " << e.what() << endl;
	return 1;
}
catch (...)
{
	cerr << "Unknown exception"<< endl;
	return 2;
}
