#include <regex>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

template<typename Target, typename Source>
Target to(Source arg)
{
	stringstream interpreter;
	Target result;
	if (!(interpreter << arg) || !(interpreter >> result) || !(interpreter >> std::ws).eof())
		throw runtime_error("failure in to<>()");
	return result;
}

int main()
{
	string s{ "3.14" };
	double d = to<double, string>(s);
	cout << d << endl;

}