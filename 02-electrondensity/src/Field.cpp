#include "Field.hpp"
#include "Atom.hpp"
#include "WaveFunction.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>



Field::Field(Wavefunction &wf, float rmin, float delta) : wf(wf), xmin(rmin), ymin(rmin), zmin(rmin), delta(delta){

    npoints_x = static_cast<int>(fabs(2.*xmin / delta));
    npoints_y = static_cast<int>(fabs(2.*ymin / delta));
    npoints_z = static_cast<int>(fabs(2.*zmin / delta));

    nsize = npoints_x * npoints_y * npoints_z;
}

float Field::Density(int norb, int npri, const int *icnt, const int *vang,
                           const float *r, const float *coor,
                           const float *depris, const float *nocc,
                           const float *coef) {
  float den = 0.0;
  const float x = r[0];
  const float y = r[1];
  const float z = r[2];

  for (int i = 0; i < norb; i++) {
    float mo = 0.0;
    const int i_prim = i * npri;
    for (int j = 0; j < npri; j++) {
      const int vj = 3 * j;
      const int centerj = 3 * icnt[j];
      const float difx = x - coor[centerj];
      const float dify = y - coor[centerj + 1];
      const float difz = z - coor[centerj + 2];
      const float rr = difx * difx + dify * dify + difz * difz;

      const float expo = exp(-depris[j] * rr);
      const float lx = vang[vj];
      const float ly = vang[vj + 1];
      const float lz = vang[vj + 2];
      const float facx = pow(difx, lx);
      const float facy = pow(dify, ly);
      const float facz = pow(difz, lz);

      mo += facx * facy * facz * expo * coef[i_prim + j];
    }
    den += nocc[i] * mo * mo;
  }

  return den;
}


#include "functionED.xx"


 // dumpXYZ("structure.xyz");




