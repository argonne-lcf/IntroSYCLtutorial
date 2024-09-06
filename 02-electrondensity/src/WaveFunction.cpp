#include "WaveFunction.hpp"

Wavefunction::Wavefunction() {
  natm = 0;
  norb = 0;
  npri = 0;
}

Wavefunction::~Wavefunction() {}

void Wavefunction::loadWF(string fname) {
  int itmp;
  float tmp;
  float tx, ty, tz;
  std::vector<int> atomicNumbers;
  std::vector<Rvector> atomicCoordinates;

  std::ifstream file(fname);
  if (file.is_open()) {
    file.clear();
    file.seekg(0, std::ios::beg);
    std::string line;
    while (std::getline(file, line)) {
      if (line.find("<Number of Occupied Molecular Orbitals>") !=
          std::string::npos) {
        std::getline(file, line);
        norb = std::stoi(line);
      }
      if (line.find("<Number of Primitives>") != std::string::npos) {
        std::getline(file, line);
        npri = std::stoi(line);
      }
      if (line.find("<Primitive Centers>") != std::string::npos) {
        readVector(file, icntrs, "</Primitive Centers>");
        for (auto &i : icntrs)
          i -= 1;
      }
      if (line.find("<Primitive Types>") != std::string::npos) {
        readVector(file, itypes, "</Primitive Types>");
      }
      if (line.find("<Primitive Exponents>") != std::string::npos) {
        readVector(file, depris, "</Primitive Exponents>");
      }
      if (line.find("<Molecular Orbital Occupation Numbers>") !=
          std::string::npos) {
        readVector(file, dnoccs, "</Molecular Orbital Occupation Numbers>");
      }
      if (line.find("<MO Number>") != std::string::npos) {
        std::getline(file, line);
        std::getline(file, line);

        if (line.find("</MO Number>") != std::string::npos) {
          for (int j = 0; j < npri; j++) {
            file >> tmp;
            dcoefs.push_back(tmp);
          }
        }
      }
      if (line.find(string("<Number of Nuclei>")) != std::string::npos) {
        std::getline(file, line);
        natm = std::stoi(line);
      }
      if (line.find(string("<Nuclear Cartesian Coordinates>")) !=
          std::string::npos) {
        for (int i = 0; i < natm; i++) {
          std::getline(file, line);
          sscanf(line.c_str(), "%f %f %f", &tx, &ty, &tz);
          atomicCoordinates.push_back(Rvector(tx, ty, tz));
        }
      }
      if (line.find(string("<Atomic Numbers>")) != std::string::npos) {
        for (int i = 0; i < natm; i++) {
          getline(file, line);
          sscanf(line.c_str(), "%d", &itmp);
          atomicNumbers.push_back(itmp);
        }
      }
    }
    setAngularVector();

    for (int i = 0; i < natm; i++)
      addAtom(Atom(atomicNumbers[i], atomicCoordinates[i]));
    file.close();
  } else {
    std::cerr << " Error to open file " << fname << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Wavefunction::setAngularVector() {
  int j;
  vang.resize(3 * npri);

  for (int i = 0; i < npri; i++) {
    j = 3 * i;
    vang[j] = 0;
    vang[j + 1] = 0;
    vang[j + 2] = 0;
    switch (itypes[i]) {
    case 1:
      vang[j] = 0;
      vang[j + 1] = 0;
      vang[j + 2] = 0;
      break;

    case 2:
      vang[j] = 1;
      break;

    case 3:
      vang[j + 1] = 1;
      break;

    case 4:
      vang[j + 2] = 1;
      break;

    case 5:
      vang[j] = 2;
      break;

    case 6:
      vang[j + 1] = 2;
      break;

    case 7:
      vang[j + 2] = 2;
      break;

    case 8:
      vang[j] = 1;
      vang[j + 1] = 1;
      break;

    case 9:
      vang[j] = 1;
      vang[j + 2] = 1;
      break;

    case 10:
      vang[j + 1] = 1;
      vang[j + 2] = 1;
      break;

    case 11:
      vang[j] = 3;
      break;

    case 12:
      vang[j + 1] = 3;
      break;

    case 13:
      vang[j + 2] = 3;
      break;

    case 14:
      vang[j] = 2;
      vang[j + 1] = 1;
      break;

    case 15:
      vang[j] = 2;
      vang[j + 2] = 1;
      break;

    case 16:
      vang[j + 1] = 2;
      vang[j + 2] = 1;
      break;

    case 17:
      vang[j] = 1;
      vang[j + 1] = 2;
      break;

    case 18:
      vang[j] = 1;
      vang[j + 2] = 2;
      break;

    case 19:
      vang[j + 1] = 1;
      vang[j + 2] = 2;
      break;

    case 20:
      vang[j] = 1;
      vang[j + 1] = 1;
      vang[j + 2] = 1;
      break;

    case 21:
      vang[j] = 4;
      break;

    case 22:
      vang[j + 1] = 4;
      break;

    case 23:
      vang[j + 2] = 4;
      break;

    case 24:
      vang[j] = 3;
      vang[j + 1] = 1;
      break;

    case 25:
      vang[j] = 3;
      vang[j + 2] = 1;
      break;

    case 26:
      vang[j] = 1;
      vang[j + 1] = 3;
      break;

    case 27:
      vang[j + 1] = 3;
      vang[j + 2] = 1;
      break;

    case 28:
      vang[j] = 1;
      vang[j + 2] = 3;
      break;

    case 29:
      vang[j + 1] = 1;
      vang[j + 2] = 3;
      break;

    case 30:
      vang[j] = 2;
      vang[j + 1] = 2;
      break;

    case 31:
      vang[j] = 2;
      vang[j + 2] = 2;
      break;

    case 32:
      vang[j + 1] = 2;
      vang[j + 2] = 2;
      break;

    case 33:
      vang[j] = 2;
      vang[j + 1] = 1;
      vang[j + 2] = 1;
      break;

    case 34:
      vang[j] = 1;
      vang[j + 1] = 2;
      vang[j + 2] = 1;
      break;

    case 35:
      vang[j] = 1;
      vang[j + 1] = 1;
      vang[j + 2] = 2;
      break;

    case 36:
      vang[j + 2] = 5;
      break;

    case 37:
      vang[j + 1] = 1;
      vang[j + 2] = 4;
      break;

    case 38:
      vang[j + 1] = 2;
      vang[j + 2] = 3;
      break;

    case 39:
      vang[j + 1] = 3;
      vang[j + 2] = 2;
      break;

    case 40:
      vang[j + 1] = 4;
      vang[j + 2] = 1;
      break;

    case 41:
      vang[j + 1] = 5;
      break;

    case 42:
      vang[j] = 1;
      vang[j + 2] = 4;
      break;

    case 43:
      vang[j] = 1;
      vang[j + 1] = 1;
      vang[j + 2] = 3;
      break;

    case 44:
      vang[j] = 1;
      vang[j + 1] = 2;
      vang[j + 2] = 2;
      break;

    case 45:
      vang[j] = 1;
      vang[j + 1] = 3;
      vang[j + 2] = 1;
      break;

    case 46:
      vang[j] = 1;
      vang[j + 1] = 4;
      break;

    case 47:
      vang[j] = 2;
      vang[j + 2] = 3;
      break;

    case 48:
      vang[j] = 2;
      vang[j + 1] = 1;
      vang[j + 2] = 2;
      break;

    case 49:
      vang[j] = 2;
      vang[j + 1] = 2;
      vang[j + 2] = 1;
      break;

    case 50:
      vang[j] = 2;
      vang[j + 1] = 3;
      break;

    case 51:
      vang[j] = 3;
      vang[j + 2] = 2;
      break;

    case 52:
      vang[j] = 3;
      vang[j + 1] = 1;
      vang[j + 2] = 1;
      break;

    case 53:
      vang[j] = 3;
      vang[j + 1] = 2;
      break;

    case 54:
      vang[j] = 4;
      vang[j + 2] = 1;
      break;

    case 55:
      vang[j] = 4;
      vang[j + 1] = 1;
      break;

    case 56:
      vang[j] = 5;
      break;

    default:
      printf(" Type of primitive unsupported!!\n");
      exit(EXIT_FAILURE);
      break;
    } // Fin del switch
  }   // Fin del for
}

void Wavefunction::addAtom(Atom a) { atoms.push_back(a); }

template <typename T>
void Wavefunction::readVector(std::ifstream &file, std::vector<T> &vector,
                              std::string endblock) {
  std::string line;
  while (std::getline(file, line) && line != endblock) {
    std::istringstream iss(line);
    T value;
    while (iss >> value) {
      vector.push_back(value);
    }
  }
}

void Wavefunction::setIntegerOutput() {
  std::cout << std::setw(6) << std::fixed << std::setprecision(0);
}

void Wavefunction::setScientificOutput() {
  std::cout << std::setw(13) << std::scientific << std::setprecision(6);
};

template <typename T>
void Wavefunction::printVector(const std::vector<T> &vector) {
  int ncount;
  int count = 0;
  if (std::is_integral<T>::value) {
    ncount = 10;
    for (const auto &val : vector) {
      setIntegerOutput();
      count++;
      std::cout << val << ' ';
      if (count == ncount) {
        count = 0;
        std::cout << std::endl;
      }
    }
    if (count != 0)
      std::cout << std::endl;
  } else {
    if (std::is_floating_point<T>::value) {
      ncount = 5;
      for (const auto &val : vector) {
        setScientificOutput();
        count++;
        std::cout << val << ' ';
        if (count == ncount) {
          count = 0;
          std::cout << std::endl;
        }
      }
      if (count != 0)
        std::cout << std::endl;
    } else {
      for (const auto &val : vector) {
        std::cout << val << std::endl;
      }
    }
  }
}

void Wavefunction::printCoefficients() {
  int count;

  for (int i = 0; i < norb; i++) {
    std::cout << " Coefficients for Orbital : " << i + 1 << std::endl;
    count = 0;
    for (int j = 0; j < npri; j++) {
      setScientificOutput();
      count++;
      std::cout << dcoefs[i * npri + j] << ' ';
      if (count == 5) {
        count = 0;
        std::cout << std::endl;
      }
    }
    if (count != 0)
      std::cout << std::endl;
  }
}

void Wavefunction::printWF() {
  setIntegerOutput();
  std::cout << " Number of Atoms      : " << natm << std::endl;
  setIntegerOutput();
  std::cout << " Number of primitives : " << npri << std::endl;
  setIntegerOutput();
  std::cout << " Number of mol. orb.  : " << norb << std::endl;

  std::cout << " Centers of primitives: " << std::endl;
  printVector(icntrs);
  std::cout << " Type of primitives   : " << std::endl;
  printVector(itypes);
  std::cout << " Angular moments      : " << std::endl;
  printVector(vang);

  std::cout << " Exponent of primitive: " << std::endl;
  printVector(depris);
  std::cout << " Occupation number    : " << std::endl;
  printVector(dnoccs);
  std::cout << " Coefficients         : " << std::endl;
  printCoefficients();
  std::cout << " Atoms                : " << std::endl;
  printVector(atoms);
}
