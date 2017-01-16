#include <iostream>

#include "headers/Battery.h"

using namespace std;

Battery::Battery(string pname, ConnectionPoint* pcp1, ConnectionPoint* pcp2, double pvoltage)
  : Component(pname, pcp1, pcp2) {
    voltage = pvoltage;
    current = 0;
}

void Battery::movePotential(const double &exec_time) {
  cp1->setPotential(voltage);
  cp2->setPotential(0);
}

double Battery::getCurrent() const {
   return 0;
}
