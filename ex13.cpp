// Chapter 23, exercise 13: test if the regular expression '.' matches the newline character '\n'
// SPOILER: It doesn't.

#include <iostream>
#include <regex>

using namespace std;

int main()
{
    string s = "\n";
    regex pat{ "." };
    if (regex_match(s, pat))
        cout << "'.' matches '\\n'" << endl;
    else
        cout << "'.' does not match '\\n'" << endl;
}
