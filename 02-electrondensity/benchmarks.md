# handleWF

The `handleWF` project is designed to manipulate the wave function of an electronic structure calculation, specifically focusing on the WFX format. One of the primary applications of this code is the computation and analysis of electron density, which is a fundamental quantity in quantum chemistry and condensed matter physics. Electron density provides insights into the distribution of electrons within a molecule or material, influencing chemical reactivity, bonding properties, and other physical characteristics. By efficiently processing wave function data, this project allows researchers to extract electron density information with high precision, leveraging modern parallel computing techniques to handle large datasets and complex calculations.

Original repo can be located in Raymundo Hernandez-Esparza repository [handleWF](https://github.com/rayhe88/handleWF)

A code to manipulate the swave function of an electronic structure calculation in WFX format.



This is the version 2 written using C++.

## Compilation

Create the `build` subdirectory
```
mkdir build
```

Move to the `build`
```
cd build
```

Type the next commands
```
cmake -H. -DUSE_SYCL=On -DCMAKE_CXX_COMPILER=icpx ../src/
```

Finally build the project
```
cmake --build .
```
### Compilation in Polaris - ALCF Machine

```
module use /soft/modulefiles
module load oneapi/upstream
module load nvhpc-mixed
module load craype-accel-nvidia80
module unload nvhpc-mixed
module load spack-pe-base cmake
 
EXTRA_FLAGS="-sycl-std=2020 -O3 -fsycl -fsycl-targets=nvptx64-nvidia-cuda -Xsycl-target-backend --cuda-gpu-arch=sm_80"
export CFLAGS="-ffp-model=precise"
export CXXFLAGS="-ffp-model=precise -fsycl $EXTRA_FLAGS"
export CC=clang
export CXX=clang++
 
 
mkdir build
cmake ../src/ -DCMAKE_CXX_COMPILER=clang++
cmake --build .
```

## Testing
### DELL Laptop 
```
./handleWF.x ../test/dimer_HCOOH.wfx
Compilation Date: Jun 12 2024  14:19:26
Git SHA1: 77e969d-dirty
 Points ( 80,80,40)
 TotalPoints : 256000
 Running on 11th Gen Intel(R) Core(TM) i7-1185G7 @ 3.00GHz
 Points ( 80,80,40)
 TotalPoints : 256000
 Running on 11th Gen Intel(R) Core(TM) i7-1185G7 @ 3.00GHz
 Points ( 80,80,40)
 TotalPoints : 256000
 Time for CPU : 5.22786e+07 μs
 Time for GPU  : 1.53022e+06 μs (Kernel 1)
 Time for GPU  : 2.22982e+06 μs (Kernel 2)
 Ratio CPU/GPU (kernel1) : 34.1641
 Ratio CPU/GPU (kernel2) : 23.4452
```
### Sunspot - ALCF Machine
```
./handleWF.x ../test/dimer_HCOOH.wfx
Compilation Date: Jun 12 2024  19:24:00
Git SHA1: 77e969d-dirty
 Points ( 80,80,40)
 TotalPoints : 256000
 Running on Intel(R) Data Center GPU Max 1550
 Points ( 80,80,40)
 TotalPoints : 256000
 Running on Intel(R) Data Center GPU Max 1550
 Points ( 80,80,40)
 TotalPoints : 256000
 Time for CPU : 5.12023e+07 μs
 Time for GPU  : 1.60554e+06 μs (Kernel 1)
 Time for GPU  : 143117 μs (Kernel 2)
 Ratio CPU/GPU (kernel1) : 31.8911
 Ratio CPU/GPU (kernel2) : 357.765

```
### Polaris - ALCF Machine
```
./handleWF.x ../test/dimer_HCOOH.wfx 
Version: 0.0
Compilation Date: Jun 12 2024  22:11:56
Git SHA1: 77e969d-dirty
 Points ( 80,80,40)
 TotalPoints : 256000
 Running on NVIDIA A100-SXM4-40GB
 Points ( 80,80,40)
 TotalPoints : 256000
 Running on NVIDIA A100-SXM4-40GB
 Points ( 80,80,40)
 TotalPoints : 256000
 Time for CPU : 4.61134e+07 μs
 Time for GPU  : 2.62889e+06 μs (Kernel 1)
 Time for GPU  : 267336 μs (Kernel 2)
 Ratio CPU/GPU (kernel1) : 17.541
 Ratio CPU/GPU (kernel2) : 172.492
```
## Acknowledgements
This research used resources of the Argonne Leadership Computing Facility, which is a DOE Office of Science User Facility supported under Contract DE-AC02-06CH11357. Argonne National Laboratory’s work was supported by the U.S. Department of Energy, Office of Science, under contract DE-AC02-06CH11357.


All rights reserved. Copyright Argonne National Laboratory UChicago LLC. Raymundo Hernandez-Esparza

