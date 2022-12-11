
// Chapter 23, exercise 9:
// Using eof() try to simplify the table-checking program from §23.9.
// 
// Chapter 23, exercise 10:
// Modify the table-checking program from §23.9 write a new table where the
// rows with the same initial digit are merged.
//
// Chapter 23, exercise 11:
// Modify the table-checking program to see if the number of students is 
// increasing or decreasing over the years.
// - Not sure what this is asking for, ex 10 groups the students by year so you
// can make an observation based on that output.
//

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <map>

using namespace std;

struct bad_from_string : bad_cast 
{
    const char* what() const noexcept override
    {
        return "bad cast from string";
    }
};

template<typename T> 
T from_string(const string& s)
{
    istringstream is{ s };
    T t;
    if (!(is >> t)) 
    {
        cout << s << endl;
        throw bad_from_string{};
    }
    return t;
}

// ex 10
struct Students 
{
    int boys = 0;
    int girls = 0;

    int total() const { return boys + girls; }
};

int main()
try 
{
    ifstream in{ "table.txt" };   // input file
    if (!in) 
        throw runtime_error("No input file!");

    regex header{ R"(^[\w ]+([[:blank:]]+[\w ]+)*$)" };           // header line
    regex row{ R"(^([\w ]+)([[:blank:]]+\d+)([[:blank:]]+\d+)([[:blank:]]+\d+)$)" };  // data line
    
    int lineno = 0;
    string line;                       // input buffer
    if (getline(in, line)) 
    { // check header line
        ++lineno;
        smatch matches;
        if (!regex_match(line, matches, header))
            throw runtime_error("No header!");
    }

    // column totals
    int boys = 0;
    int girls = 0;

    map<char, Students> years;

    while (getline(in, line)) 
    { // check data line
        ++lineno;
        smatch matches;
        if (!regex_match(line, matches, row))
            throw runtime_error("bad line" + to_string(lineno));

        // check row
        int curr_boy = from_string<int>(matches[2]);
        int curr_girl = from_string<int>(matches[3]);
        int curr_total = from_string<int>(matches[4]);
        if (curr_boy + curr_girl != curr_total)
            throw runtime_error("bad row sum\n");

        char year = matches[1].str()[0];

        if (isdigit(year)) 
        {
            years[year].boys += curr_boy;
            years[year].girls += curr_girl;
        }

        // ex 9
        if ((in >> ws).eof()) 
        {    // check if there's only whitespace between current line and eof()
            if (curr_boy != boys || curr_girl != girls)
                throw runtime_error("columns don't add up\n");
        }
        else 
        { // update totals
            boys += curr_boy;
            girls += curr_girl;
        }
    }

    // ex 10 output
    cout << "AR" << '\t' << "DRENG" << '\t'
        << "PIGE" << '\t' << "ELEVER" << '\n';

    for (const auto& y : years)
        cout << y.first << '\t'
        << y.second.boys << '\t'
        << y.second.girls << '\t'
        << y.second.total() << '\n';
}
catch (exception& e) {
    cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch (...) {
    cerr << "Unknown exception\n";
    return 2;
}

