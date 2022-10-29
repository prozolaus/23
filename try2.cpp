#include <regex>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

int main()
{
	regex pattern;

	string pat;
	cout << "Input pattern: ";
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

	int lineno = 0;
	cout << "Input strings:\n";

	for (string line; getline(cin, line);)
	{
		++lineno;
		smatch matches;
		if (regex_search(line, matches, pattern))
		{
			cout << "string " << lineno << ": " << line << "\n";
			for (int i = 0; i < matches.size(); i++)
				cout << "\tmatches[" << i << "]: " << matches[i] << endl;
		}
		else
			cout << "does not match\n";
	}
}