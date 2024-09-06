#ifndef _ATOM_H_
#define _ATOM_H_

#include "Rvector.hpp"
#include <iostream>
#include <string>
using std::string;

using namespace std;

class Atom {
public:
  Rvector coor;

private:
  int zatom = -1;
  float mass = 0.1;
  string symb = "00";
  string setSymbol(int);

  int setAtomicNumberfromSymbol(string);
  float setAtomicMass(int);

public:
  //  Rvector coor;
  Atom(int, float, float, float);
  Atom(int, float *);
  Atom(int, Rvector);
  ~Atom();
  //***********************************************
  Atom(string, float, float, float);
  Atom(string, float *);
  Atom(string, Rvector);
  Atom(const char *, float, float, float);
  Atom(const char *, float *);
  Atom(const char *, Rvector);
  //***********************************************
  Atom &operator=(const Atom &at);
  //***********************************************
  float getMass();
  string getSymbol();
  Rvector getCoors();
  float get_x();
  float get_y();
  float get_z();
  int get_atnum();
  float get_charge();
  //***********************************************
  friend ostream &operator<<(ostream &o, const Atom &);
};

#endif
