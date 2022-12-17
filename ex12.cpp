// Chapter 23 Exercise 12 - Date Finder and Replacer
// Write a program that replaces various date formats within a file with the
// ISO yyyy-mm-dd format. Produce an output file that is identical to the input
// with the dates changed.

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

string convert_month(const string& month)
{
	if (month == "January") return "01";
	else if (month == "February") return "02";
	else if (month == "March") return "03";
	else if (month == "April") return "04";
	else if (month == "May") return "05";
	else if (month == "June") return "06";
	else if (month == "July") return "07";
	else if (month == "August") return "08";
	else if (month == "September") return "09";
	else if (month == "October") return "10";
	else if (month == "November") return "11";
	else if (month == "December") return "12";
	throw runtime_error("Wrong month!");
}

string date_string(const string& line)
{
	// month or day first, separated by / or . or -
	regex pat{ R"((\s+|^)(\d{1,2})[-/\.](\d{1,2})[-/\.](\d{4})(\D))" };
	// January 31, 2008
	regex pat2{ R"((\s+|^)(January|February|March|April|May|June|July|August|September|October|November|December)\s+(\d{1,2}),\s+(\d{4})(\D))" };
	// 31 January 2008
	regex pat3{ R"((\s+|^)(\d{1,2})\s+(January|February|March|April|May|June|July|August|September|October|November|December)\s+(\d{4})(\D))" };

	smatch matches;
	string string_with_date, n_line = line;

	if (regex_search(line, matches, pat))
	{
		string m = (matches[3].str() > "12" ? matches[2].str() : matches[3].str());
		string d = (m == matches[3].str() ? matches[2].str() : matches[3].str());
		string_with_date = matches[1].str() + matches[4].str() + "-" + m + "-" + d + matches[5].str();
		n_line = regex_replace(n_line, pat, string_with_date);
	}
	if (regex_search(line, matches, pat2))
	{
		string_with_date = matches[1].str() + matches[4].str() + "-" + convert_month(matches[2].str()) + "-" + matches[3].str() + matches[5].str();
		n_line = regex_replace(n_line, pat2, string_with_date);
	}
	if (regex_search(line, matches, pat3))
	{
		string_with_date = matches[1].str() + matches[4].str() + "-" + convert_month(matches[3].str()) + "-" + matches[2].str() + matches[5].str();
		n_line = regex_replace(n_line, pat3, string_with_date);
	}

	return n_line;
}

int main()
try
{
	string filenamein{ "dates.txt" };
	ifstream ifs{ filenamein };
	if (!ifs)
		throw runtime_error("Cannot open " + filenamein);
	string filenameout{ "dates_out.txt" };
	ofstream ofs{ filenameout };
	if (!ofs)
		throw runtime_error("Cannot open " + filenameout);

	string line;
	while (getline(ifs, line))
		ofs << date_string(line) << endl;

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
