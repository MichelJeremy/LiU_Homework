#include <iostream>
#include <cmath>

#include "headers/Capacitor.h"

using namespace std;

Capacitor::Capacitor(string pname, ConnectionPoint* pcp1, ConnectionPoint* pcp2, double pcapacity)
  : Component(pname, pcp1, pcp2) {
    capacity = pcapacity;
    q = 0;
}

// move potential following capacity behaviour
void Capacitor::movePotential(const double &exec_step) {
  double potential_diff = abs(cp1->getPotential() - cp2->getPotential());
  double temp = capacity * (potential_diff - q) * exec_step;
  q += capacity * (potential_diff - q) * exec_step;
  if (cp1->getPotential() > cp2->getPotential()) {
    cp1->setPotential(cp1->getPotential() - temp);
    cp2->setPotential(cp2->getPotential() + temp);
  } else {
    cp1->setPotential(cp1->getPotential() + temp);
    cp2->setPotential(cp2->getPotential() - temp);
  }
  voltage = abs(cp1->getPotential() - cp2->getPotential());
}

// overrides getCurrent with Capacity specific formula
double Capacitor::getCurrent() const {
  return capacity * (voltage - q);
}

double Capacitor::getCapacity() const {
  return capacity;
}
