#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <vector>
#include <stdexcept>

#include "headers/ConnectionPoint.h"
#include "headers/Component.h"
#include "headers/Battery.h"
#include "headers/Resistor.h"
#include "headers/Capacitor.h"

#define EXEMPLE3

using namespace std;

// Note: this is complementary work for all files together.
// 5-4. "for types with a non-trivial constructor".


/* USAGE:
 * As specified in the subject ./a.out 200000 10 0.01 24 will work as intended.
 * To change which example should be ran, change the define a few lines above.
 * No additions have been made.
 */

// frees memory
// delete call raises a warning but it is alright in our case, as we will never
// call delete on a subclass object
void deallocate_component(const vector<Component*> &vec) {
  for_each(vec.begin(), vec.end(), []( Component *p ) { ::delete p; });
}

// print first two lines on terminal
void printHeaders(const vector<Component*> &net) {
  for (size_t i = 0; i < net.size(); i++) {
    if (i == 0) cout << "        " << net.at(i) -> getName();
    else cout << "          " << net.at(i) -> getName();
  }
  cout << endl;
  for (size_t i = 0; i < net.size(); i++) {
    cout << " Volt  " << "Curr ";
  }
  cout << endl;
}

// function that takes care of all cases of the annoying output requirement
void printValue(const double &val1, const double &val2) {
  if ((val1 > 10) && (val2 > 10)) {
    cout << setprecision(2) << fixed << val1 << " " << val2;
  } else if ((val1 > 10) && (val2 < 10)) {
    cout << setprecision(2) << fixed << val1 << "  " << val2;
  } else if ((val1 < 10) && (val2 > 10)) {
    cout << setprecision(2) << fixed << " " << val1 << " " << val2;
  } else if ((val1 < 10) && (val2 < 10)) {
    cout << setprecision(2) << fixed << " " << val1 << "  " << val2;
  }
  cout << " ";
}

void simulate(const vector<Component*> &net, const double &simul_time,
              const int &lines, const double &step) {
  printHeaders(net);
  double target_time_line = simul_time / lines;
  for (double time = 0; time <= simul_time; time += 1) {
    if ((time >= target_time_line)) {
      for (size_t i = 0; i < net.size(); i++) {
        printValue(net.at(i) -> getVoltage(), net.at(i) -> getCurrent());
      }
      target_time_line += simul_time / lines;
      cout << endl;
    }

    for (size_t i = 0; i < net.size(); i++) {
      net.at(i) -> movePotential(step);
    }
  }
  cout << endl;
}

bool checkForTypeValidity(const string &value) {
  try {
    stod(value);
  } catch(invalid_argument) {
    cerr  << "Error on argument " << value
          << ": strings are not acceptable parameters." << endl;
    return false;
  } catch (out_of_range) {
    cerr  << "Value " << value << " is out of range of C++ double type." << endl;
    return false;
  }
  return true;
}

bool checkForValueValidity(const double &value) {
  if (value < 0) {
    cerr << "Values cannot be negative";
    return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  if (argc != 5) {
    cerr  << "Invalid arguments. Usage: ./a.out simul_time nbrOfLines step voltage"
          << endl;
  } else {
    // skip 0
    for (int i = 1; i < argc; i++) {
      if (!checkForTypeValidity(argv[i])) {
        cout << "Exiting ..." << endl;
        return -1;
      }

      if (!checkForValueValidity(stod(argv[i]))) {
        cout << "Exiting ..." << endl;
        return -1;
      }
    }
    // if both checks are passed, go on with the program
    double voltage = stod(argv[4]);
    double step = stod(argv[3]);
    double lines = stod(argv[2]);
    double simul_time = stod(argv[1]);

    // define is at the top of main.cc file
    #ifdef EXEMPLE1
      ConnectionPoint *p{new ConnectionPoint()};
      ConnectionPoint *l{new ConnectionPoint()};
      ConnectionPoint *r{new ConnectionPoint()};
      ConnectionPoint *n{new ConnectionPoint()};
      vector<Component*> net;

      net.push_back(new Battery("Bat", p, n, voltage));
      net.push_back(new Resistor("R1", p, l, 6.0));
      net.push_back(new Resistor("R2", l, r, 4.0));
      net.push_back(new Resistor("R3", r, n, 8.0));
      net.push_back(new Resistor("R4", l, n, 12.0));
    #endif

    #ifdef EXEMPLE2
      ConnectionPoint *p{new ConnectionPoint()};
      ConnectionPoint *l{new ConnectionPoint()};
      ConnectionPoint *r{new ConnectionPoint()};
      ConnectionPoint *n{new ConnectionPoint()};
      vector<Component*> net;

      net.push_back(new Battery("Bat", p, n, voltage));
      net.push_back(new Resistor("R1", p, l, 150.0));
      net.push_back(new Resistor("R2", p, r, 50.0));
      net.push_back(new Resistor("R3", l, r, 100.0));
      net.push_back(new Resistor("R4", l, n, 300.0));
      net.push_back(new Resistor("R5", r, n, 250.0));
    #endif

    #ifdef EXEMPLE3
      ConnectionPoint *p{new ConnectionPoint()};
      ConnectionPoint *l{new ConnectionPoint()};
      ConnectionPoint *r{new ConnectionPoint()};
      ConnectionPoint *n{new ConnectionPoint()};
      vector<Component*> net;

      net.push_back(new Battery("Bat", p, n, voltage));
      net.push_back(new Resistor("R1", p, l, 150.0));
      net.push_back(new Resistor("R2", p, r, 50.0));
      net.push_back(new Capacitor("C3", l, r, 1.0));
      net.push_back(new Resistor("R4", l, n, 300.0));
      net.push_back(new Capacitor("C5", r, n, 0.75));
    #endif

    simulate(net, simul_time, lines, step);
    deallocate_component(net);
    // frees ConnectionPoint
    delete p;
    delete l;
    delete r;
    delete n;
    return 0;
  }
}
