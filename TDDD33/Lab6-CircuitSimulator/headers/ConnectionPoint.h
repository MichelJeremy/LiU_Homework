#ifndef CONNECTIONPOINT_H
#define CONNECTIONPOINT_H

class ConnectionPoint {
  public:
    ConnectionPoint();
    void setPotential(const double &value);
    double getPotential() const;
  private:
    double potential = 0;
};

#endif
