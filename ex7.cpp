// Chapter 23, exercise 7.
// Write a program that finds and checks credit card numbers in a file.

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

bool checkLuhn(const string& input) 
{
	int sum = 0;

	for (int i = input.length() - 1; i >= 0; i--) {
		int number = input[i] - '0';

		if (i % 2 == 0) {
			number *= 2;

			if (number > 9) {
				number -= 9;
			}
		}

		sum += number;
	}

	return sum % 10 == 0;
}

bool has_ccnumbers(const string& line) //only for Ukraine
{
	// 16 digits starts with: 4 - Visa, 5 - Mastercard or 9 - Prostir
	regex pat{ R"((\D|^)((4|5|9)\d{3}\s?)(\d{4}\s?){3}(\D|$))"};

	smatch matches;
	return regex_search(line, matches, pat) && checkLuhn(matches[1]);
}

int main()
try
{
	string filename{ "credit_card_numbers.txt" };
	ifstream ifs{ filename };
	if (!ifs)
		throw runtime_error("Cannot open " + filename);

	int line_number = 0;
	string line;
	while (getline(ifs, line))
	{
		line_number++;
		if (has_ccnumbers(line))
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
	cerr << "Unknown exception" << endl;
	return 2;
}
