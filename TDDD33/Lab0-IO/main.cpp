<<<<<<< HEAD
#include <iostream>
#include <string>
#include <limits>

using namespace std;

// 1-3.
// 1-13. Expecting 0 returned from main if all went well.
// GJ!


int main() {
    //variable declaration
    int inputInt;
    double inputDouble;
    char inputChar;
    string inputStr;

    cout << "Input one integer: ";
    cin >> inputInt;
    cout << "You entered the number: " << inputInt << endl << endl;
    cin.clear(); // normal procedure to clear the buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // removes every characters until the carriage return

    cout << "Enter four integers: ";
    cin >> inputInt;
    cout << "You entered the numbers: " << inputInt;
    cin >> inputInt;
    cout << " " << inputInt;
    cin >> inputInt;
    cout << " " << inputInt;
    cin >> inputInt;
    cout << " " << inputInt << flush << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter one integer and one real number: ";
    cin >> inputInt >> inputDouble;
    cout.precision(3);
    cout << "The real is:         " << fixed << inputDouble << endl;
    cout << "The integer is:       " << inputInt << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter one real and one integer number: ";
    cin >> inputDouble >> inputInt;
    cout.precision(3);
    cout << "The real is: ..." << fixed << inputDouble << endl;
    cout << "The integer is ........" << inputInt << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter a character : ";
    cin >> inputChar;
    cout << "You entered: " << inputChar << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter a word: ";
    cin >> inputStr;
    cout << "The word \'" << inputStr << "\' has " << inputStr.length() << " character(s)" << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter an integer and a word: ";
    cin >> inputInt >> inputStr;
    cout << "You entered \`" << inputInt <<"\` and \`" << inputStr << "\`." << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter an character and a word: ";
    cin >> inputChar >> inputStr;
    cout << "You entered the string \"" << inputStr << "\" and the character \`" << inputChar << "\`." << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter a word and a real number: ";
    cin >> inputStr >> inputDouble;
    cout.precision(3);
    cout << "You entered \"" << inputStr << "\" and \"" << fixed << inputDouble << "\"." << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter a text-line: ";
    getline(cin, inputStr);
    cout << "You entered: \""<< inputStr << "\"" << endl << endl;
    cin.clear();

    cout << "Enter a second line of text: ";
    getline(cin, inputStr);
    cout << "You entered: \`"<< inputStr << "\"" << endl << endl;
    cin.clear();

    cout << "Enter three words: ";
    cin >> inputStr;
    cout << "You entered: \`" << inputStr;
    cin >> inputStr;
    cout << " " << inputStr;
    cin >> inputStr;
    cout << " " << inputStr << "`" << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
=======
#include <iostream>
#include <string>
#include <limits>

using namespace std;

int main() {
    //variable declaration
    int inputInt;
    double inputDouble;
    char inputChar;
    string inputStr;

    cout << "Input one integer: ";
    cin >> inputInt;
    cout << "You entered the number: " << inputInt << endl << endl;
    cin.clear(); // normal procedure to clear the buffer
    // removes every characters until the carriage return:
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter four integers: ";
    cin >> inputInt;
    cout << "You entered the numbers: " << inputInt;
    cin >> inputInt;
    cout << " " << inputInt;
    cin >> inputInt;
    cout << " " << inputInt;
    cin >> inputInt;
    cout << " " << inputInt << flush << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter one integer and one real number: ";
    cin >> inputInt >> inputDouble;
    cout.precision(3);
    cout << "The real is:         " << fixed << inputDouble << endl;
    cout << "The integer is:       " << inputInt << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter one real and one integer number: ";
    cin >> inputDouble >> inputInt;
    cout.precision(3);
    cout << "The real is: ..." << fixed << inputDouble << endl;
    cout << "The integer is: ......." << inputInt << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter a character : ";
    cin >> inputChar;
    cout << "You entered: " << inputChar << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter a word: ";
    cin >> inputStr;
    cout    << "The word '" << inputStr << "' has "
            << inputStr.length() << " character(s)" << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter an integer and a word: ";
    cin >> inputInt >> inputStr;
    cout    << "You entered `" << inputInt <<"` and `"
            << inputStr << "`." << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter an character and a word: ";
    cin >> inputChar >> inputStr;
    cout    << "You entered the string \"" << inputStr
            << "\" and the character `" << inputChar
            << "`." << endl << endl;

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter a word and a real number: ";
    cin >> inputStr >> inputDouble;
    cout.precision(3);
    cout    << "You entered \"" << inputStr << "\" and \""
            << fixed << inputDouble << "\"." << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter a text-line: ";
    getline(cin, inputStr);
    cout << "You entered: \""<< inputStr << "\"" << endl << endl;
    cin.clear();

    cout << "Enter a second line of text: ";
    getline(cin, inputStr);
    cout << "You entered: `"<< inputStr << "\"" << endl << endl;
    cin.clear();

    cout << "Enter three words: ";
    cin >> inputStr;
    cout << "You entered: `" << inputStr;
    cin >> inputStr;
    cout << " " << inputStr;
    cin >> inputStr;
    cout << " " << inputStr << "`" << endl << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return 0;
}
>>>>>>> feedback
