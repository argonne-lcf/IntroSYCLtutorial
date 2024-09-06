#ifndef _FIELD_HPP_
#define _FIELD_HPP_

#include "WaveFunction.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <sycl/sycl.hpp>
#include <vector>

class Field {
public:
    Field(Wavefunction &wf, float rmin, float delta);
    
    float orbital(Rvector r, int i);
    float gaussiana(Rvector r, int mu);
    float Density(Rvector r);
    void evalDensity();
    void evalDensity2();
    void evalDensity2D();
    
    void evalDensity_sycl();
    void evalDensity_sycl2();
    static SYCL_EXTERNAL float Density(int, int, const int *, const int *,
                                           const float *, const float *,
                                           const float *, const float *,
                                           const float *);

    void spherical(std::string fname);
    
    void dumpXYZ(std::string filename);

//  void dumpCube(float, float, float, float, int, int, int, vector<float>,
//                std::string filename);
    void dumpCube(float xmin, float ymin, float zmin, float delta,
                       int nx, int ny, int nz, vector<float> field,
                       std::string filename) {
        std::ofstream fout(filename);
        if (fout.is_open()) {
    
                fout << "Density" << std::endl;
                fout << "By handleWF project" << std::endl;
                fout << std::setw(5) << std::fixed << wf.natm;
                fout << std::setw(13) << std::setprecision(6) << std::fixed << xmin << ' ';
                fout << std::setw(13) << std::setprecision(6) << std::fixed << ymin << ' ';
                fout << std::setw(13) << std::setprecision(6) << std::fixed << zmin;
                fout << std::endl;
                
                fout << std::setw(5) << std::fixed << nx;
                fout << std::setw(13) << std::setprecision(6) << std::fixed << delta << ' ';
                fout << std::setw(13) << std::setprecision(6) << std::fixed << 0.0 << ' ';
                fout << std::setw(13) << std::setprecision(6) << std::fixed << 0.0;
                fout << std::endl;
                
                fout << std::setw(5) << std::fixed << ny;
                fout << std::setw(13) << std::setprecision(6) << std::fixed << 0.0 << ' ';
                fout << std::setw(13) << std::setprecision(6) << std::fixed << delta << ' ';
                fout << std::setw(13) << std::setprecision(6) << std::fixed << 0.0;
                fout << std::endl;
                
                fout << std::setw(5) << std::fixed << nz;
                fout << std::setw(13) << std::setprecision(6) << std::fixed << 0.0 << ' ';
                fout << std::setw(13) << std::setprecision(6) << std::fixed << 0.0 << ' ';
                fout << std::setw(13) << std::setprecision(6) << std::fixed << delta;
                fout << std::endl;
                }
                
                for (auto atom : wf.atoms) {
                fout << std::setw(5) << std::fixed << atom.get_atnum();
                fout << std::setw(13) << std::setprecision(6) << std::fixed
                   << atom.get_charge() << ' ';
                fout << std::setw(13) << std::setprecision(6) << std::fixed << atom.get_x()
                   << ' ';
                fout << std::setw(13) << std::setprecision(6) << std::fixed << atom.get_y()
                   << ' ';
                fout << std::setw(13) << std::setprecision(6) << std::fixed << atom.get_z();
                fout << std::endl;
            }
    
        int cnt = 0;
        for (auto value : field) {
            cnt++;
            fout << std::setw(15) << std::setprecision(6) << std::fixed << std::scientific << value;
            if (cnt == 6) {
                fout << std::endl;
                cnt = 0;
            }
        }
        if (cnt != 0)
            fout << std::endl;
        
        fout.close();
    }


private:
    Wavefunction &wf;
    float xmin, ymin, zmin;
    float delta;
    int npoints_x;
    int npoints_y;
    int npoints_z;
    size_t nsize;
};


#endif
