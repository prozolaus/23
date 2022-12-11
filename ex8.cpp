// Chapter 23. Exercise 8.
// Modify the program from §23.8.7 so that it takes as inputs a pattern and a file name.

#include <regex>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

int main()
try
{
	regex pattern;

	string pat;
	cout << "Input pattern: ";	//For example: <\w+(\.\w+)?@\w+\.\w+>
	getline(cin, pat);

	try
	{
		pattern = pat;
		cout << "pattern: " << pat << endl;
	}
	catch (const std::exception&)
	{
		cout << "Incorrect expression: " << pat << endl;
		exit(1);
	}
	
	string filename;
	cout << "Input file name: ";	//mail2.txt
	getline(cin, filename);
	ifstream ifs{ filename };
	if (!ifs)
		throw runtime_error("Cannot open " + filename);

	int lineno = 0;

	for (string line; getline(ifs, line);)
	{
		++lineno;
		smatch matches;
		if (regex_search(line, matches, pattern))
		{
			cout << "string " << lineno << ": " << line << "\n";
			for (int i = 0; i < matches.size(); i++)
				cout << "\tmatches[" << i << "]: " << matches[i] << endl;
		}
	}
}
catch (exception& e)
{
	cerr << "Exception: " << e.what() << endl;
	return 1;
}
catch (...)
{
	cerr << "Unknown exception" << endl;
	return 2;
}