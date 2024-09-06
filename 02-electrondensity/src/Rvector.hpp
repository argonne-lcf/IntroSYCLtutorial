#ifndef _RVECTOR_HPP_
#define _RVECTOR_HPP_

#include <iostream>
using namespace std;

class Rvector {
private:
  float x;
  float y;
  float z;

public:
  Rvector(float rx = 0.0, float ry = 0.0, float rz = 0.0);
  Rvector(float *);

  void set_x(float);
  void set_y(float);
  void set_z(float);

  float get_x(void);
  float get_y(void);
  float get_z(void);

  float operator[](int);

  void operator+=(const Rvector &);
  void operator-=(const Rvector &);
  void operator*=(int k);
  void operator*=(float k);
  void operator/=(float k);
  Rvector operator+(const Rvector &);
  Rvector operator-(const Rvector &);
  Rvector operator*(int k);
  Rvector operator*(float k);
  Rvector operator/(float k);

  float dot(const Rvector &);
  float norm();

  void normalize();

  friend ostream &operator<<(ostream &o, const Rvector &);
};

#endif
