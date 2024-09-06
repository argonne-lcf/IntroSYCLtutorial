# Setting Up the Environment for SYCL on Polaris

![Polaris](images/polaris.jpg)

*Image source: [ALCF Beginners Guide](https://github.com/argonne-lcf/ALCFBeginnersGuide/tree/master)*

#### System Requirements
- **Hardware**: Polaris features AMD EPYC Milan CPUs and NVIDIA A100 GPUs, which are well-suited for SYCL applications.
- **Operating System**: Polaris runs on the HPE Cray Operating System, which supports a range of HPC applications.

#### Installation Guide
Below are steps to set up a SYCL environment on Polaris using the DPC++ compiler, suitable for NVIDIA GPUs.

##### Step 1: Log In to Polaris
- Securely connect to Polaris via SSH with your credentials:
  ```bash
  ssh [your_username]@polaris.alcf.anl.gov
  ```

##### Step 2: Load the Required Modules
- Polaris uses a modular environment system. Load the modules for the HPE Cray programming environment, which includes support for SYCL/DPC++:
  ```bash
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
  ```

##### Step 3: Check DPC++ Availability
- Ensure that the DPC++ environment is correctly set up by checking the version:
  ```bash
  dpcpp --version
  ```

##### Step 4: Begin Development
- Create a directory for your SYCL projects and start coding:
  ```bash
  mkdir my_sycl_projects
  cd my_sycl_projects
  vi my_first_sycl_app.cpp  # or use another text editor available on Polaris
  ```

##### Step 5: Compile and Test Your SYCL Program
- Compile your SYCL program using DPC++:
  ```bash
  dpcpp my_first_sycl_app.cpp -o my_first_sycl_app
  ```
- Submit your job to the GPU nodes on Polaris using the job scheduler:
  ```bash
  # Example PBS script for job submission
  qsub -q debug -A YourAllocationHere -l select=1:system=polaris -l walltime=00:59:00 -l filesystems=home -I
  ```

### Tips
- **Documentation**: For detailed instructions and best practices, refer to the [Polaris user guide](https://www.alcf.anl.gov/polaris) and the [UnoAPI documentation](https://unoapi.org/80-running/running.html).
- **Support**: If you encounter issues, consult the ALCF support center or participate in community forums available to Polaris users.

This guide provides specific instructions tailored to the advanced hardware and software environment of Polaris, helping ensure that your SYCL applications are optimally configured and ready for development.
