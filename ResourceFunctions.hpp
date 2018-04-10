//Levi Purdy
//4/10/2018

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <string>
using std::string;
using std::getline;
#include <sstream>
using std::istringstream;

int getInt(string prompt) {
	while (true) {
		// Input a line, with error checking
		string line;
		cout << prompt << endl;;
		getline(cin, line);
		if (!cin)
		{
			cout << "Error in input" << endl;
			continue;
		}
		if (line == "") {
			cout << "Error in input" << endl; continue;
		}

		// Input an integer from the above line
		istringstream in(line);
		if (!in) {
			cout << "Please type a positive integer" << endl;
			cout << endl;
			continue;
		}
		int n;  // Integer to input from line
		in >> n;
		return n;
	}

}

string getLine(string prompt) {
	while (true) {
		string line;

		cout << prompt << endl;

		getline(cin, line);
		if (!cin) {
			cout << "Error in input" << endl;
			continue;
		}
		if (line == "") {
			cout << "Error in input" << endl; continue;
		}
		else return line;
	}
}
