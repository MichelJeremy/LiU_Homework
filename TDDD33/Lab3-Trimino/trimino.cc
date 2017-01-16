#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>

#include "trimino.h"

using namespace std;

/** CHANGE LOG
 * Ranges are now taken if not specified, so for example,
 * ./a.out trimino1.txt 0 100 will now work
 *
 * Also, if there are more than 3 arguments (i.e. ./a.out 1 2 3 4)
 * The program assumes it is a brick and will ask for min/max (special case)
 * Checked with TA, should be alright. Program can be modified to look for Ranges
 * depending on the amount of parameters, though (last two argv would be ranges since
 * url can have spaces).
 **/

int main(int argc, char* argv[]) {
	string filename; // name of the test file
	vector<string> lines;
	int nb=0, min_value, max_value;
	bool a;
	string line_formatted;
	// get the filename/brick from the console or ask for it if not there
	// If you enter at least 4 values separated by space, it will
	// be considered as a brick
	if (argc == 1) {
		while (true) {
			cout << "Please enter a filename:";
			getline(cin, filename); // get filename from cin
			// process filename, min and max values
			if (getFile(filename, min_value, max_value) == 1) break;
			else {
				cerr << "Error: could not open file, please try again\n\n";
				cin.clear();
			}
		}
	} else if (argc == 2) {
		filename = argv[1];
		if (getFile(filename, min_value, max_value) != 1) {
			cerr << "Error: could not open file. Exiting ...\n";
		}
	} else if (argc == 4){
		// input is filename min max
		filename = argv[1];
		min_value = atoi(argv[2]);
		max_value = atoi(argv[3]);
	} else if (argc > 4) {
		// this case is if the input is a correct brick
		// input type 1 2 3 this is a url we can't assume anything
		// min max are asked later for this case as an exception
		for (int i = 1; i < argc; i++) {
			if (i == argc - 1) {
				line_formatted.append(argv[i]);
			} else {
				line_formatted.append(argv[i]);
				line_formatted.append(" ");
			}
		}

		cout << "\nPlease give the min value: ";
		cin >> min_value;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.clear();
		cout << "\nPlease give the max value: ";
		cin >> max_value;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.clear();

		if (isCorrect(line_formatted, min_value, max_value)) {
			cout << "Brick entered is correct\n\n";
			return 0;
		} else {
			cout 	<< "Brick entered is not valid\n\n";
			return 0;
		}
	} else {
		cerr << "Incorrect number of arguments. Exiting ...\n";
		return 0;
	}

	// here, we made sure that filestream exists
	// now, we can process this file
	ifstream filestream(filename);
	for(string line; getline(filestream, line); )	{
		lines.push_back(line);
	}

	for (vector<string>::iterator i = lines.begin(); i != lines.end(); i++) {
		a = isCorrect(*i, min_value, max_value);
		if(!a) {
			cout 	<< "In file " << filename << ": error line " << i + 1 - lines.begin()
						<< endl << endl;
		}
	}
	return 1;
}

// finx maximum of two integers
int max_find(int a, int b) {
	if (a >= b) {
		return a;
	}
	return b;
}

// find minimum of two integers
int min_find(int a, int b) {
	if (a <= b) {
		return a;
	}
	return b;
}


bool clock_increasing(int a, int b, int c) {
	if ((a <= b) && (b <= c)) {
		return true;
	}
	if ((b <= c) && (c <= a)) {
		return true;
	}
	if ((c <= a) && (a <= b)) {
		return true;
	}
	return false;
}

// returns a bool matching the brick's correctness
bool isCorrect(string set, int min_v, int max_v) {
	stringstream ss;
	string the_split;
	vector<string> v;
 	vector<string>::iterator it{v.begin()};
	ss << set;
	int first_value;
	int second_value;
	int third_value;

	while (ss >> the_split) {
			v.push_back(the_split);
		}
		if(v.size() < 3) {
			cerr << "(Reason: bad number of parameters: " << v.size() << ") - ";
			return false;
		} else {
			if ((atoi(v.at(0).c_str())) && (atoi(v.at(1).c_str())) && (atoi(v.at(2).c_str()))) {
				first_value = stoi((v.at(0)));
				second_value = stoi((v.at(1)));
				third_value = stoi((v.at(2)));
			} else {
				cerr << "(Reason: the three first parameters must be numbers) - ";
				return false;
			}

			if (!clock_increasing(first_value, second_value, third_value)) {
					cerr << "(Reason: The numbers must be clockwise equal or increasing "
					     <<"starting from the smallest) - ";
							 return false;
			}

			if (min_v > min_find(min_find(first_value, second_value), third_value)
			      || max_v < max_find(max_find(first_value, second_value), third_value)) {
							cerr << "(Reason: bad range) - ";
				return false;
			}
		}
			cout << "Line working" << endl;
			return true;
}
int getFile(string const &filename, int &minvalue, int &maxvalue) {
	// const because filename is not going to ger reallocated
	ifstream filestream(filename); // try to open file stream, cast into pointer
	if (filestream) {
		cout << "\nPlease give the min value: ";
		cin >>minvalue;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.clear();
		cout << "\nPlease give the max value: ";
		cin >> maxvalue;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.clear();
		return 1;
	} else { // if file opening fails, ask for a new file name
		return -1;
	}
}
