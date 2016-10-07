#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;

/*
This function takes a value and counts its decimals.
it return 1 if there are two or less decimals and -1 if there are more than two decimals
*/
int countDecimals(double inputValue) {
    int decimalCounter = 0;
    double inputValueNoInt = inputValue;
    inputValueNoInt *= 100; // take two decimals at most into int part
    inputValueNoInt -= (int) inputValueNoInt; // get only decimals
    cout << inputValueNoInt;
    if (inputValueNoInt > (double)0) {
        return -1; // if there is still something left there were more than two decimals
    } else {
        return 1;
    }
}


/*
This function returns true if the stride is correct (a multiple of the difference between last and first pride)
and returns false if the stride is incorrect
*/
bool isStrideCorrect(float stride, float firstPrice, float lastPrice) {
    float difference = lastPrice - firstPrice;
    //TODO
}

int main() {
    float firstPrice;
    float lastPrice;
    float stride;
    float taxPercent;

    cout << "INPUT PART\n";
    cout << "==========";

    while (1) { // input validation
        cout << "\nEnter first price: ";
        cin >> firstPrice;
        if (firstPrice <= 0,00 || firstPrice >= 100000,00) {
            cout << "\nA first price cannot be lesser or equal to 0 or greater than 100000";
        } else if (countDecimals(firstPrice) != 1) { // this function makes sure that there are only two decimals
            cout << "\nPlease only enter two decimals at most";
        } else {
            cout << firstPrice;
            cin.clear();
            cin.ignore(numeric_limits<streamsize> :: max(), '\n');
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize> :: max(), '\n');
    }

    while (1) { // input validation
        cout << "\nEnter last price: ";
        cin >> lastPrice;
        if (lastPrice <= firstPrice || lastPrice >= 100000,00) {
                cout << "\nA last price must be greater than the first price and lesser than 100000";
            } else if (countDecimals(lastPrice) != 1) { // this function makes sure that there are only two decimals
                cout << "\nPlease only enter two decimals at most";
            } else {
                cout << lastPrice;
                cin.clear();
                cin.ignore(numeric_limits<streamsize> :: max(), '\n');
                break;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize> :: max(), '\n');
    }

    //TODO
    while (1) { // input validation
        cout << "\nEnter stride ";
        cin >> stride;

        if (stride <= 0) {
            cout << "stride must be at least 0.01";
        } else if (countDecimals(stride) != 1) {
            cout << "\nPlease only enter two decimals at most";
        }
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize> :: max(), '\n');

    cout << "\nEnter tax percent: ";
    cin >> lastPrice;
    cin.clear();
    cin.ignore(numeric_limits<streamsize> :: max(), '\n');

    //here we make the calculation for the prices taking the stride into account

    cout << "\n\nTAX TABLE";
    cout << "\n=========\n";
    cout << "\tPrice\t\tTax\t\tPrice with tax\n"; // one tab before, two tab between each others
    cout << "------------------------------------------------------";

}
