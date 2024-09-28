
# Compile on Polaris 

If you have access to Polaris with the following directions you will be able to test the algorithm with a default graph that is indicated on the Parallel SSSP on [Wiki](https://en.wikipedia.org/wiki/Parallel_single-source_shortest_path_algorithm)

1. Start with loading modules that we need to correctly add all necessary depencedinces and oneapi.
   ```bash
    module use /soft/modulefiles
    module load oneapi/upstream
    module load nvhpc-mixed
    module load craype-accel-nvidia80
    module unload nvhpc-mixed
    module load spack-pe-base cmake
   ```
2. Add the following `FLAGS`
   ```bash
    EXTRA_FLAGS="-sycl-std=2020 -O3 -fsycl -fsycl-targets=nvptx64-nvidia-cuda -Xsycl-target-backend --cuda-gpu-arch=sm_80"
    export CFLAGS="-ffp-model=precise"
    export CXXFLAGS="-ffp-model=precise -fsycl $EXTRA_FLAGS"
    export CC=clang
    export CXX=clang++
   ```
3. Clone the repo `git clone https://github.com/JorgeV92/SSSP.git`

Head into the directory `SSSP` and run the following commands 
```bash
  cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build`
  cmake --build build
```