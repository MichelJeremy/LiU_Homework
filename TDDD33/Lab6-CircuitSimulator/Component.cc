#include <iostream>
#include <cmath>

#include "headers/Component.h"

using namespace std;

Component::Component(string pname, ConnectionPoint *pcp1, ConnectionPoint *pcp2 ) {
  name = pname;
  cp1 = pcp1;
  cp2 = pcp2;
}

string Component::getName() const {
  return name;
}

double Component::getVoltage() const {
 return abs(cp1->getPotential() - cp2->getPotential());
}
