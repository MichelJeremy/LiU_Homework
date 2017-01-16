#ifndef BATTERY_H
#define BATTERY_H

#include "ConnectionPoint.h"
#include "Component.h"

class Battery : public Component {
  public:
    Battery(std::string pname, ConnectionPoint* pcp1, ConnectionPoint* pcp2, double pvoltage);
    // resets battery so that it never runs out
    void movePotential(const double &exec_time) override;

    // overrides getCurrent with Battery specific formula. Current is always 0.
    double getCurrent() const override;
};

#endif
