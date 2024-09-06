#include "Rvector.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

Rvector::Rvector(float *r) {
  x = r[0];
  y = r[1];
  z = r[2];
}

Rvector::Rvector(float rx, float ry, float rz) {
  x = rx;
  y = ry;
  z = rz;
}

void Rvector::set_x(float rx) { x = rx; }

void Rvector::set_y(float ry) { y = ry; }

void Rvector::set_z(float rz) { z = rz; }

float Rvector::get_x(void) { return x; }

float Rvector::get_y(void) { return y; }

float Rvector::get_z(void) { return z; }

float Rvector::operator[](int idx) {
  switch (idx) {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  default:
    cout << " Out of range, returning 0" << endl;
    return 0;
  }
}

void Rvector::operator+=(const Rvector &r) {
  Rvector q = r;
  x += q[0];
  y += q[1];
  z += q[2];
}

Rvector Rvector::operator+(const Rvector &r) {
  Rvector q(x, y, z);
  q += r;

  return q;
}

void Rvector::operator-=(const Rvector &r) {
  Rvector q = r;
  x -= q[0];
  y -= q[1];
  z -= q[2];
}

Rvector Rvector::operator-(const Rvector &r) {
  Rvector q(x, y, z);
  q -= r;

  return q;
}

void Rvector::operator*=(float k) {
  x *= k;
  y *= k;
  z *= k;
}

void Rvector::operator*=(int k) {
  x *= (float)k;
  y *= (float)k;
  z *= (float)k;
}

void Rvector::operator/=(float k) {
  x /= k;
  y /= k;
  z /= k;
}

Rvector Rvector::operator*(float k) {
  Rvector q(x, y, z);
  q *= k;

  return q;
}

Rvector Rvector::operator*(int k) {
  Rvector q(x, y, z);
  q *= k;

  return q;
}

Rvector Rvector::operator/(float k) {
  Rvector q(x, y, z);
  q /= k;

  return q;
}

float Rvector::dot(const Rvector &r) {
  Rvector q = r;

  return (q[0] * x + q[1] * y + q[2] * z);
}

float Rvector::norm() {
  float r2 = x * x + y * y + z * z;
  return sqrt(r2);
}

void Rvector::normalize() {
  float n = sqrt(x * x + y * y + z * z);

  x /= n;
  y /= n;
  z /= n;
}

ostream &operator<<(ostream &o, const Rvector &r) {
  o << " (" << setw(10) << setprecision(6) << fixed << r.x << "," << setw(10)
    << setprecision(6) << fixed << r.y << "," << setw(10) << setprecision(6)
    << fixed << r.z << ")";
  return o;
}
