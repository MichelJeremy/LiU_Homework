#include <iostream>
#include <limits>
#include <iomanip>


using namespace std;


int main() {
    float firstPrice; // (input) first price
    float lastPrice; // (input) last price
    float stride; // (input) stride
    float taxPercent; // (input) tax percent


    cout << "INPUT PART\n";
    cout << "==========";

    while (1) { // input validation
        cout << "\nEnter first price: ";
        cin >> firstPrice;
        if (firstPrice <= 0.00 || firstPrice >= 100000.00) {
            cerr << "ERROR: A first price must be greater or equal than 0 "
                 << "and lesser than 100000\n";
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize> :: max(), '\n');
            break;
        }
        cin.clear(); // io buffer clearing if value is wrong
        cin.ignore(numeric_limits<streamsize> :: max(), '\n');
    }

    while (true) { // input validation
        cout << "Enter last price:  ";
        cin >> lastPrice;
        if (lastPrice <= firstPrice || lastPrice >= 100000.00) {
                cerr << "ERROR: A last price must be greater than the first "
                     << "price and lesser than 100000\n";
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize> :: max(), '\n');
            break;
        }
        cin.clear(); // io buffer clearing if value is wrong
        cin.ignore(numeric_limits<streamsize> :: max(), '\n');
    }

    while (true) { // input validation
        cout << "Enter stride:      ";
        cin >> stride;

        if (stride <= 0) {
            cerr << "ERROR: Stride must be at least 0.01\n";
        } else { // case success
            cin.clear();
            cin.ignore(numeric_limits<streamsize> :: max(), '\n');
            break;
        }
        cin.clear(); // io buffer clearing if value is wrong
        cin.ignore(numeric_limits<streamsize> :: max(), '\n');
    }

    while (true) {
        cout << "Enter tax percent: ";
        cin >> taxPercent;

        if (taxPercent < 0.00 || taxPercent > 100.00) {
            cerr << "\nTax percent must be in the [0 - 100] interval.\n";
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize> :: max(), '\n');
            break;
        }
        cin.clear(); // io buffer clearing if value is wrong
        cin.ignore(numeric_limits<streamsize> :: max(), '\n');
    }


    //here we make the calculation for the prices taking the stride into account

    cout << "\n\nTAX TABLE";
    cout << "\n=========\n";
    // one tab before, two tab between each others
    cout << "\tPrice\t\t Tax\t Price with tax";
    cout << "\n-----------------------------------------------";

    // processing for tax and price with tax
    int numberOfSteps = (lastPrice - firstPrice) / stride;
    float tax;
    float priceWithTax;
    float price;
    for(int i = 0; i <= numberOfSteps; i++) {
        price = firstPrice + (i * stride);
        priceWithTax = price + (price * (taxPercent / 100.00));
        tax = priceWithTax - price;
        cout  << "\n\t" << fixed << setprecision(2) << price << "\t\t" << tax
              << "\t\t  " << priceWithTax;
    }
    cout << "\n"; // for the last line break
    return(0);
}
