// Chapter 23, exercise 14:
// Write a program like the sample program from 23.8.7, but read input
// from file into memory and experiment with patterns including '\n'

#include <regex>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

int main()
{
	string filename = "dates2.txt", text;
	ifstream ifs{ filename };
	if (!ifs)
	{
		cerr << "Cannot read from file " << filename << '\n';
		exit(1);
	}
	for (string s; getline(ifs, s);)
		text += s + '\n';

	while (true)
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
			cerr << "Incorrect expression: " << pat << endl;
			exit(1);
		}

		smatch matches;
		if (regex_search(text, matches, pattern))
		{
			for (int i = 0; i < matches.size(); i++)
				cout << "\tmatches[" << i << "]: " << matches[i] << endl;
		}
		else
			cout << "does not match\n";
	}
}