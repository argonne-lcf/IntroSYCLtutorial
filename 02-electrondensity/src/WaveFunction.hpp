#ifndef _WAVEFUNCTION_HPP
#define _WAVEFUNCTION_HPP

#include "Atom.hpp"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

class Field;
class Wavefunction {
public:
  Wavefunction();
  ~Wavefunction();
  void loadWF(string);
  // friend ostream &operator<<(ostream &o, const Wavefunction &);
  void printWF();

private:
  int natm;
  int norb;
  int npri;
  std::vector<int> icntrs;
  std::vector<int> itypes;
  std::vector<int> vang;
  std::vector<float> depris;
  std::vector<float> dnoccs;
  std::vector<float> dcoefs;
  std::vector<Atom> atoms;

  template <typename T>
  void readVector(std::ifstream &file, std::vector<T> &vector,
                  std::string endblock);

  template <typename T> void printVector(const std::vector<T> &vector);

  void printCoefficients();

  void addAtom(Atom a);
  void setAngularVector();
  void setScientificOutput();
  void setIntegerOutput();

  friend class Field;
};

#endif
