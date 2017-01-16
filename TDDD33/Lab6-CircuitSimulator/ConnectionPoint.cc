#include "headers/ConnectionPoint.h"

using namespace std;

ConnectionPoint::ConnectionPoint(){
  potential = 0;
}

void ConnectionPoint::setPotential(const double &value) {
  potential = value;
}

double ConnectionPoint::getPotential() const {
  return potential;
}
