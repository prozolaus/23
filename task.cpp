// Stroustrup - Programming Principles & Practice
// Chapter 23 - Regex Zip Code Example

#include <iostream>
#include <stdexcept>
#include <regex>
#include <string>
#include <fstream>

using namespace std;

int main()
try 
{
	ifstream in{ "zip_codes.txt" };
	if (!in) 
		cerr << "No file\n";

	regex pat{ R"([[:alpha:]]{2}\s*\d{5}(-\d{4})?)" };

	int lineno = 0;
	for (string line; getline(in, line); ) 
	{
		++lineno;
		smatch matches;
		if (regex_search(line, matches, pat)) 
		{
			cout << lineno << ": " << matches[0] << '\n';
			if (1 < matches.size() && matches[1].matched)
				cout << "\t: " << matches[1] << '\n';
		}
	}
}
catch (std::exception& e) 
{
	std::cerr << "Exception: " << e.what() << '\n';
	return 1;
}
catch (...) 
{
	std::cerr << "Unknown exception\n";
	return 2;
}

