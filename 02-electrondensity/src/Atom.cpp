#include "Atom.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

static string chemSymbols[119] = {
    "X",  "H",  "He", "Li", "Be", "B",  "C",  "N",  "O",  "F",  "Ne", "Na",
    "Mg", "Al", "Si", "P",  "S",  "Cl", "Ar", "K",  "Ca", "Sc", "Ti", "V",
    "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br",
    "Kr", "Rb", "Sr", "Y",  "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag",
    "Cd", "In", "Sn", "Sb", "Te", "I",  "Xe", "Cs", "Ba", "La", "Ce", "Pr",
    "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu",
    "Hf", "Ta", "W",  "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi",
    "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U",  "Np", "Pu", "Am",
    "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh",
    "Hs", "Mt", "Ds", "Rg", "--", "--", "--", "--", "--", "--", "--"};

Atom::Atom(int n, float rx, float ry, float rz) {
  zatom = n;
  mass = setAtomicMass(n);
  symb = setSymbol(n);
  coor = Rvector(rx, ry, rz);
}

Atom::Atom(int n, float *r) {
  zatom = n;
  mass = setAtomicMass(n);
  symb = setSymbol(n);
  coor = Rvector(r);
}

Atom::Atom(int n, Rvector r) {
  zatom = n;
  mass = setAtomicMass(n);
  symb = setSymbol(n);
  coor = Rvector(r);
}

Atom::Atom(string st, float rx, float ry, float rz) {
  zatom = setAtomicNumberfromSymbol(st);
  mass = setAtomicMass(zatom);
  symb = st;
  coor = Rvector(rx, ry, rz);
}

Atom::Atom(string st, float *r) {
  zatom = setAtomicNumberfromSymbol(st);
  mass = setAtomicMass(zatom);
  symb = st;
  coor = Rvector(r);
}

Atom::Atom(string st, Rvector r) {
  zatom = setAtomicNumberfromSymbol(st);
  mass = setAtomicMass(zatom);
  symb = st;
  coor = Rvector(r);
}

Atom::Atom(const char *st, float rx, float ry, float rz) {
  zatom = setAtomicNumberfromSymbol(string(st));
  mass = setAtomicMass(zatom);
  symb = string(st);
  coor = Rvector(rx, ry, rz);
}

Atom::Atom(const char *st, float *r) {
  zatom = setAtomicNumberfromSymbol(string(st));
  mass = setAtomicMass(zatom);
  symb = string(st);
  coor = Rvector(r);
}

Atom::Atom(const char *st, Rvector r) {
  zatom = setAtomicNumberfromSymbol(string(st));
  mass = setAtomicMass(zatom);
  symb = string(st);
  coor = Rvector(r);
}

Atom::~Atom(){};
//***********************************************
string Atom::setSymbol(int idx) {
  string noneSymbol = "--";

  if (idx < 0 || idx > 119) {
    return noneSymbol;
  }

  return chemSymbols[idx];
}

int Atom::setAtomicNumberfromSymbol(string symbol) {
  for (int idx = 0; idx < 119; idx++) {
    if (symbol.compare(chemSymbols[idx]) == 0)
      return idx;
  }
  cout << "No se encontro el simbolo!!" << endl;
  return 0;
}

float Atom::setAtomicMass(int idx) {
  static float chemAtomicMass[119] = {
      0.0,      1.008,    4.0026,   6.9410,   9.0122,   10.8110,  12.011,
      14.0067,  15.9994,  18.9984,  20.1797,  22.9898,  24.3050,  26.9815,
      28.0855,  30.9737,  32.0650,  35.4530,  39.9480,  39.0983,  40.0780,
      44.9559,  47.8670,  50.9415,  51.9961,  54.9380,  55.8450,  58.9332,
      58.6934,  63.5460,  65.3800,  69.7230,  72.6400,  74.9216,  78.9600,
      79.9040,  83.7980,  85.4678,  87.6200,  88.9059,  91.2240,  92.9064,
      95.9600,  98.0000,  101.0700, 102.9055, 106.4200, 107.8682, 112.411,
      114.8180, 118.7100, 121.7600, 127.6000, 126.9045, 131.2930, 132.91,
      137.33,   138.91,   140.12,   140.91,   144.24,   145.,     150.36,
      151.96,   157.25,   158.93,   162.50,   164.93,   167.26,   168.93,
      173.05,   174.97,   178.49,   180.95,   183.84,   186.21,   190.23,
      192.22,   195.08,   196.97,   200.59,   204.38,   207.2,    208.98,
      209.,     210.,     222.,     223.,     226.,     227.,     232.04,
      231.04,   238.03,   237.,     244.,     243.,     247.,     247.,
      251.,     252.,     257.,     258.,     259.,     262.,     261.,
      262.,     266.,     264.,     277.,     268.,     269.,     272.,
      0.,       0.,       0.,       0.,       0.,       0.,       0.};
  if (idx < 0 || idx > 119) {
    return 0.;
  }
  return chemAtomicMass[idx];
}

Rvector Atom::getCoors() { return coor; }

float Atom::get_x() { return coor[0]; }

float Atom::get_y() { return coor[1]; }

float Atom::get_z() { return coor[2]; }

float Atom::getMass() { return mass; }

string Atom::getSymbol() { return symb; }

int Atom::get_atnum() { return zatom; }

float Atom::get_charge() { return (float)zatom; }

ostream &operator<<(ostream &o, const Atom &a) {
  o << fixed << setw(3) << a.symb << setw(6) << a.zatom
    << setiosflags(ios::right) << setw(10) << fixed << setprecision(4)
    << a.mass;

  return o;
}
