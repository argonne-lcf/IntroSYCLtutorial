# SYCL Introduction 


### Architecture

<img width="1303" alt="sycl" src="/images/image4.png">

SYCL is a framework designed to help programmers use a variety of computing devices—like CPUs, GPUs, and FPGAs—in one application, despite this not being directly addressed in the standard C++ language. It aims to guide the future development of C++ by showing how heterogeneous computing can be integrated effectively. Although SYCL doesn't promise seamless performance across different hardware types, it provides a consistent way to write and adjust code for various architectures. It uses advanced C++ features like templates and lambda functions to make it easier to write efficient code that works with different backend technologies, such as OpenCL and CUDA. For more information you can look at the *Kronos* page here [SYCL](https://www.khronos.org/sycl/).

### Lambda functions 

In SYCL, lambdas play a crucial role similar to their use in general programming, but they are specifically tailored for defining operations on data that will be executed on parallel devices like GPUs and CPUs. Like in other programming contexts, lambdas in SYCL allow for writing concise, anonymous functions. This capability is especially valuable in SYCL due to the nature of parallel computing, where operations often need to be defined locally and executed across a range of data elements.

Lambdas in SYCL are structured similarly to standard C++ lambdas, but are specifically utilized within the SYCL framework to define the functionality of kernels that execute on parallel compute devices. The basic syntax of a lambda in SYCL can be summarized as follows:

```cpp
[capture_clause](input_signature) -> return_specification {
    // execution_block
}
```

In the context of SYCL you typically encounter the following types of captures:

- `[]` : Captures nothing from the enclosing scope. This is used when the lambda does not need to access any external variables.

- `[&]` : Captures all accessible variables from the surrounding scope by reference. Useful when you need to modify the external variables or when copying them is expensive.

- `[=]` : Captures all accessible variables from the surrounding scope by value. This is safe when the lambda is executed asynchronously or on a separate device, ensuring that it works with a consistent copy of the data.

For example, when defining a SYCL kernel, a developer might use a lambda to specify the computation that each thread should perform on the elements of a buffer. This lambda can capture necessary variables from its surrounding scope to use within the kernel execution:

```cpp
buffer<float, 1> buf(data, range<1>(data_size));
queue.submit([&](handler& cgh) {
    auto acc = buf.get_access<access::mode::read_write>(cgh);
    cgh.parallel_for<class myKernel>(range<1>(data_size), [=](id<1> idx) {
        acc[idx] *= 2; // Example operation: double each element
    });
});

```

# Connecting to Intel DevCloud via SSH

For reference you can also check [SYCL Academy](https://github.com/codeplaysoftware/syclacademy) for how to set up devcloud and compile oprtions. 

1. **Create an Account:** Sign up for an account on [Intel DevCloud](https://www.intel.com/content/www/us/en/homepage.html) if you haven't already.
2. **Configure SSH:** Click on [Automated Configuration](https://devcloud.intel.com/oneapi/documentation/connect-with-ssh-linux-macos/) to initialize your SSH setup. Follow the provide d instructions to configure your SSH settings.
3. **Connect:** Once the setup is complete, simply run `ssh devcloud` to connect.

### SYCL Academy examples

1. **Clone the Repository:** Visit the [SYCL Academy](https://github.com/codeplaysoftware/syclacademy?tab=readme-ov-file) repository and clone it in the DevCloud environment using the following command:
  
  ```bash
  git clone --recursive https://github.com/codeplaysoftware/syclacademy.git
  ```

2. **Load CMake:** After cloning the repository, load CMake with the following command:

  ```bash
  module load cmake
  ```
  You can check what modules you currently have importanted by running `module list` and you should see the following 
  
  ```
  Currently Loaded Modulefiles:
 1) cmake/3.21.2(default) 
  ```
  
  If intrested in how the code is being compiled look at the `CMakeLists.txt` file for this.
  
3. Everyting you need to run every code exercises is by performing the following:

```bash 
cd syclacademy
mkdir build
cd build
cmake ../ "-GUnix Makefiles" -DSYCL_ACADEMY_USE_DPCPP=ON -DSYCL_ACADEMY_ENABLE_SOLUTIONS=ON -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx
```

* `"-GUnix Makefiles"`: This specifies that the generated build system should use Unix Makefiles.
* `-DSYCL_ACADEMY_USE_DPCPP=ON` : This option enables the use of DPCPP (Data Parallel C++) compiler in the project.
* `-DSYCL_ACADEMY_ENABLE_SOLUTIONS=ON` : This option includes solutions in the build.
* `-DCMAKE_C_COMPILER=icx` : This sets the C compiler to Intel's `icx`.

4. Once you have completed the above head back to the `syclacademy` directory and change into the `Code_Exercise` directory. If you would like to perform a simple test to make sure if you're able to compile sycle code head into the `What_is_SYCL` directory and modify the `source.cpp` file to include the header `#include <sycl/sycl.hpp>`. Once complete head into `~/syclacademy/build/Code_Exercises/What_is_SYCl`and run `make`. Finally test the output by running `./What_is_SYCL_source` and you should see the following. 

  ```bash 
    ===============================================================================
    All tests passed (1 assertion in 1 test case)
  ```
  
  ### Compute Nodes
  
  For more computational heavy programs we will request access to a node with gpu hardaware. For testing wrap the binary into   a script `job_submission` for the `What_is_SYCL_source` program 
  
  ```bash
  vim job_submission
  ```
  
  Paste the following
  
  ```bash
  #!/bin/bash
  ./What_is_SYCL_source
  ```
  
  run the following to add request a node 
  
  ```bash
  qsub -l nodes=1:gpu:ppn=2 -d . job_submission
  ```
  
  You should see the following 
  
  ```bash
  <your id>.v-qsvr-1.aidevcloud
  ```
  
  To check the status of your job run `qstat -u $USER`
  
  ### Scalar Add 
  
  In the following exercise we test SYCL functionality and basic structure now that we have the import working. 
  In the following the we see how to use SYCL to perform a simple addition operation between two integers on a device, using     SYCL's buffer and accessor concepts to manage data:
  
  ```cpp
  int a = 18, b = 24, r = 0;

  auto defaultQueue = sycl::queue{};

  {
    auto bufA = sycl::buffer{&a, sycl::range{1}};
    auto bufB = sycl::buffer{&b, sycl::range{1}};
    auto bufR = sycl::buffer{&r, sycl::range{1}};

    defaultQueue
        .submit([&](sycl::handler &cgh) {
          auto accA = sycl::accessor{bufA, cgh, sycl::read_only};
          auto accB = sycl::accessor{bufB, cgh, sycl::read_only};
          auto accR = sycl::accessor{bufR, cgh, sycl::write_only};

          cgh.single_task<scalar_add_buff_acc>([=] { accR[0] = accA[0] + accB[0]; });
        })
        .wait();
  }
  ```
  
  1. **Queue Creation:** `auto defaultQueue = sycl::queue{};` creates a SYCL queue that manages the exection order of kernels   and memeory commands. 
  2. **Buffers:** Buffers for `a`, `b`, and `r` are created with `sycl::buffer{&a, sycl::range{1}}` and smiliar lines. These     buffers are used to synchronize data between the host (**CPU**) and the device (**GPU**, for example).
  3. **Submit a Kernel:** 
      * The kernel is submitted to the queue with `defaultQueue.submit(...)`.
      * Inside the kernel submission, accessors for `a`, `b`, and `r` are created with specific access permissions (*read-            only* for `a` and `b`, *write-only* for `r`).
4.  **Execution and Sunchronization:** `.wait();` ensures that the host waits for the kernel execution to complete befroe         proceeding. 

### Test and compile 

To compile the above modify the `source.cpp` to have have the correct solution or use the already provided `solution.cpp` in the directory `Managing_Data`. To compile head into `~/syclacademy/build/Code_Exercises/Managing_Data/` and run `make`. This will give you 2 output files `Managing_Data_solution` and `Managing_Data_source` if you coded your on solution in `source.cpp` then to test the program we need to run the job on a compute node. Following the same precedure for the above case for testing the import you can create another file for the script `Managing_Data_solution` with the following:

```bash
# past this into the file (#!/bin/bash ./Managing_Data_solution)
vim job_submission
```

Then run again the following line 

```bash 
qsub -l nodes=1:gpu:ppn=2 -d . job_submission
```
 To check the output of either file simply run `cat job_submission.o<job id>` or `cat job_submission.e<job id> `
 

## UnoAPI Data-Parallel C++ Examples

For the following we will be working on the example on [unoapi-dpcpp-examples](https://github.com/LoyolaChicagoCode/unoapi-dpcpp-examples/tree/main) in the directory for *montecarlo* example. 

We will work through the example first on Intel DevCloud using the documented instrcutions on the github repo 

Follow these steps if working on **DevCloud**:
1. **Clone:** git clone https://github.com/LoyolaChicagoCode/unoapi-dpcpp-examples
2. Check which version of `camke` you are currently using if you're using aversion < 3.23.5 you can switch into a new version in devcloud with the following: `module switch cmake/<your version> cmake/3.26.3`.
3. **Set vars:** `source ./setvars.sh`
4. **CMake:** run `cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build`
5. **Build:** `cmake --build build`

*Disclaimer ran into an error when compiling. Check for fixes on the following error*

```cpp
/home/u222228/unoapi-dpcpp-examples/technical_analysis/main.cpp:56:17: error: no viable constructor or deduction guide for deduction of template arguments of 'vector'
   56 |     std::vector all(std::istream_iterator<std::string>(std::cin), {});
```
The above error did not affect the *montecarlo* example

**MonteCarlo**

1. **Queue and Buffers**
    - A `sycl::queue` is created with the `default_selector`, which picks the best available device (**CPU**, **GPU**, etc.)
    - A `dpc_common::exception_handler` is used to handle exceptions that occur during queue operations.
    - `sycl::buffer` abstracts data storage and allows the SYCL runtime to manage memory usage across different compute               devices.
      
```cpp
  sycl::queue q{sycl::default_selector_v, dpc_common::exception_handler};
  sycl::buffer<uint64_t> c_buf{counts.data(), sycl::range<1>(counts.size())};
  sycl::buffer<uint64_t> s_buf{&sum, 1};
```

2. **Logging Device Information**
    - This logs the name of the device and its maximum workgroup size, which is useful for debugging and performance tuning.

```cpp
spdlog::info("Device: {}", q.get_device().get_info<sycl::info::device::name>());
spdlog::info("Max workgroup size: {}", q.get_device().get_info<sycl::info::device::max_work_group_size>());

```

3. **Submitting a Kernel to the Queue**:
    - A task is submitted to the queue. Within this task, a kernel is defined where each invocation of the kernel computes the      number of darts that land inside a circle in a dart-throwing simulation.
    - The kernel is executed in parallel for `number_of_players` times. Each parallel invocation represents a player throwing         `number_of_darts`.
    - Two types of random number generators are used: `minstd_rand` and `ranlux48`. 
    - Each player throws darts, and the code calculates whether each dart falls inside a predefined circle based on their `x`       and `y` coordinates.

```cpp
 q.submit([&](auto &h) {
            const auto c = c_buf.get_access<sycl::access_mode::write>(h);

            h.parallel_for(number_of_players, [=](const auto index) {
                const auto offset = 37 * index.get_linear_id() + 13;
                oneapi::dpl::minstd_rand minstd(seed, offset);
                oneapi::dpl::ranlux48 ranlux(seed, offset);

                //constexpr double R{1.0};
                //oneapi::dpl::uniform_real_distribution<double> distr(0.0, R);
                constexpr uint64_t R{3037000493UL}; // largest prime <= sqrt(ULONG_MAX / 2)
                oneapi::dpl::uniform_int_distribution<uint64_t> distr(0, R);
                const auto r_square{R * R};

                auto darts_within_circle{0UL};
                for (auto i{0UL}; i < number_of_darts; i++) {
                    const auto x{use_ranlux ? distr(ranlux) : distr(minstd)};
                    const auto y{use_ranlux ? distr(ranlux) : distr(minstd)};
                    const auto d_square{x * x + y * y};
                    if (d_square <= r_square)
                        darts_within_circle++;
                }
                c[index] = darts_within_circle;
            });
        });
```

4. **Reduction to Sum Results**
    - After all players have thrown their darts, a reduction operation is performed to sum up the results from all players.
    - Data is read from the buffer, and a reduction operation is set up using `sycl::plus` to sum up all the values.
    - A parallel for loop runs the reduction operation across all elements, combining them into the final sum.

```cpp
q.submit([&](auto &h) {
    const auto c{c_buf.get_access<sycl::access_mode::read>(h)};
    const auto sum_reduction{sycl::reduction(s_buf, h, sycl::plus<>())};

    h.parallel_for(sycl::range<1>{number_of_players}, sum_reduction, [=](const auto index, auto &sum) {
        sum.combine(c[index]);
    });
});
```

*The full implementation can be found here [montecarlo](https://github.com/LoyolaChicagoCode/unoapi-dpcpp-examples/blob/main/montecarlo/main.cpp)*

**Side note:** useful inforamtion for DevCloud on running jobs can be found here [Launch and manage jobs](https://devcloud.intel.com/oneapi/documentation/job-submission/)

Now we can run the example for **Montecarlo** head into `~/unoapi-dpcpp-examples/build/bin` create another job submission file as we need to run the job on a compute node using a specific hardware. Exmaple `job_submission_monte` and paste the following 

```bash
#!/bin/bash
./montecarlo
```

Now we need to adjust our request for a node using different hardware than the above because this example performs specific computation and we targer a specific *GPU*  we submit a job to an Intel® UHD Graphics P630 use the gen9 property:

```bash
qsub -l nodes=1:gen9:ppn=2 -d . job_submission_monte
```

Once finished you view the output by running `./cat job_submission_monte.o<job id>` and you should see the following:

```bash
########################################################################
#      Date:           Wed Jun 12 10:18:22 AM PDT 2024
#    Job ID:           2555039.v-qsvr-1.aidevcloud
#      User:           u222228
# Resources:           cput=75:00:00,neednodes=1:gen9:ppn=2,nodes=1:gen9:ppn=2,walltime=06:00:00
########################################################################

[2024-06-12 10:18:24.819] [info] 4 players are going to throw 1000000 darts each
[2024-06-12 10:18:24.824] [info] using minstd engine with real distribution
[2024-06-12 10:18:24.824] [info] randomization is off
[2024-06-12 10:18:29.488] [info] Device: Intel(R) UHD Graphics P630
[2024-06-12 10:18:29.488] [info] Max workgroup size: 256
[2024-06-12 10:18:31.370] [info] done submitting to queue...waiting for results
[2024-06-12 10:18:32.950] [info] result[0] = 786512
[2024-06-12 10:18:32.950] [info] result[1] = 786064
[2024-06-12 10:18:32.950] [info] result[2] = 786509
[2024-06-12 10:18:32.950] [info] result[3] = 786054
[2024-06-12 10:18:32.950] [info] sum = 3145139
pi = 3.145139

########################################################################
# End of output for job 2555039.v-qsvr-1.aidevcloud
# Date: Wed Jun 12 10:18:33 AM PDT 2024
########################################################################
```

# Profiling SYCL applications on DevCloud
First steps how how to run profiling can be found here [Intel® VTune™ Profiler](https://www.intel.com/content/www/us/en/docs/vtune-profiler/cookbook/2023-0/using-vtune-server-with-vs-code-intel-devcloud.html#FINISH).

I outline the steps I took to simplify matters for others looking to do the same. *Here we go!*

If you managed to step up **DevCloud** with **VS Code** you can follow the steps in *Option 1*.

I document the steps in *Option 2* using SSH.

1. Log into the DevCloud login node:

```bash 
ssh devcloud
```

2. Reserve the compute node:
 
 ```bash
 qsub -I
 ```
 > **Note** DO NOT CLOSE THE TERMINAL.

3. Open a new terminal 
 > To simplify this procedure, run VTune on a specific port (55001 in this example). You can select a different port if 55001 is busy.
4. Log into the DevCloud node again, this time with SSH Port forwarding enabled:

```bash 
ssh -L 127.0.0.1:55001:127.0.0.1:55001 devcloud
```
5. Establish an SSH connection from the login node to the compute node with one more SSH tunnel:

```bash 
ssh -L 127.0.0.1:55001:127.0.0.1:55001 s000-n000
```
 Replace s000-n000 with your compute node name. To find your compute node name run `qstat -f <job_id>` and look for `exec_host = s000-n000/0-1` your s000-n000 will be different.
 
 **Not working correctly double check the above steps**
 
# FPGAs

This is a review and walk through of how to use and test *Field Programmable Gate Arrays (FPGAs)* in **DevCloud**. 
Once you have logged in you wanna clone the following examples provided by **oneAPI**. For another resoruce you can look here [ MeluXina supercomputer](https://docs.lxp.lu/fpga/oneapi/dpcpp/).
```bash
git clone https://github.com/oneapi-src/oneAPI-samples.git
```
To expedite navigation, create a symbolic link at the root of your home directory that points to this folder:

```bash
cd
ln -s oneAPI-samples/DirectProgramming/C++SYCL_FPGA/Tutorials/GettingStarted
```



Now we need access to compute nodes for FPGAs in **DevCloud** provided in the following command:
Useful resource and additional documentation is available here [oneAPI-samples](https://github.com/oneapi-src/oneAPI-samples/blob/master/DirectProgramming/C++SYCL_FPGA/README.md#build-and-run-the-samples-on-intel-devcloud-optional).

```bash
qsub -I -l nodes=1:fpga_runtime:ppn=2 -d .
```


# Polaris

If you have an account and have access to Polaris we will work on how to replicate the above exmaples on **Polaris** as well.
Once you have access to **Polaris** you can check which modules are currently loaded in by running `module list`. To be able to work with **oneAPI** run the following commands:

```bash
module use /soft/modulefiles
module load oneapi/upstream
module load spack-pe-base cmake
```
You should be able to see these modules when running `module list`.

To run the examples for **SYCL Academy** clone the repo using the same procedure as the **Intel DevCLoud** set up. 
The only changes need to made to the following flags to use *clang* instead of *icx*

```bash
cd syclacademy
mkdir build
cd build
cmake ../ "-GUnix Makefiles" -DSYCL_ACADEMY_USE_DPCPP=ON -DSYCL_ACADEMY_ENABLE_SOLUTIONS=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```
Once compiled just run `make` this can take a mintue so be patient :) 

 # oneMKL
oneMKL, or oneAPI Math Kernel Library, is a comprehensive and highly optimized library developed by Intel. It offers a wide range of mathematical functions designed to maximize performance on various computing architectures, including CPUs, GPUs, and other accelerators.

You can view a description of **oneMKL*** and the provided excercise and example here [oneMKL](https://github.com/codeplaysoftware/syclacademy/tree/main/Code_Exercises/OneMKL_gemm)

Navigate into the `syclacademy/Code_Exercises/OneMKL_gemm/` and you can view the excercise ` source_onemkl_buffer_gemm.cpp` and `source_onemkl_usm_gemm.cpp` one for *buffers* and *USM*.

I explain the solution in the following if you want to work on the solution yourself you can do now before continuing. 

### Matrix product using buffers

1. Initialize random matrices:

```cpp
 // A(M, N)
  for (size_t i = 0; i < M; i++)
    for (size_t j = 0; j < N; j++)
      A[i * N + j] = dis(gen);
  // B(N, P)
  for (size_t i = 0; i < N; i++)
    for (size_t j = 0; j < P; j++)
      B[i * P + j] = dis(gen);

  // Resultant matrix: C_serial = A*B
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < P; j++) {
      for (size_t d = 0; d < N; d++) {
        C_host[i * P + j] += A[i * N + d] * B[d * P + j];
      }
    }
  }
```

2. Declare Queue and device:

```cpp
 // Create a SYCL in-order queue targetting GPU device
  sycl::queue Q{sycl::gpu_selector_v, sycl::property::queue::in_order{}};
  // Prints some basic info related to the hardware
  print_device_info(Q);
```

3. Declare buffers:

```cpp
// TODO: Allocate memory on device, (using sycl::malloc_device APIs)
  // Creating 1D buffers for matrices which are bound to host memory array
  sycl::buffer<T, 1> a{A.data(), sycl::range<1>{M * N}};
  sycl::buffer<T, 1> b{B.data(), sycl::range<1>{N * P}};
  sycl::buffer<T, 1> c{C_host.data(), sycl::range<1>{M * P}};
```

4.  Use **oneMKL GEMM** library for the matrix multiplication  

```cpp
 // TODO: Use oneMKL GEMM USM API
  oneapi::mkl::transpose transA = oneapi::mkl::transpose::nontrans;
  oneapi::mkl::transpose transB = oneapi::mkl::transpose::nontrans;
  oneapi::mkl::blas::column_major::gemm(Q, transA, transB, n, m, k, alpha, b,
                                        ldB, a, ldA, beta, c, ldC);
  Q.wait();
  sycl::host_accessor C_device{c};
```

These libabries like **GMM** are great for specific vendors who provide efficent and optimized functionality for specific harderware that we can use using oneMLK. 

> We dont have to do the heavy lifting ourself we let **oneMKL*** do it for us.
 


### Electron Density on Polaris

We will walk throuigh a short description on the project and the following implemenation can be found here [handleWF](https://github.com/rayhe88/handleWF)

**Review and explain the FIELD** 

# TODO
1. check devices 
2. Review performance 
3. Time output check

#

> "Beware of bugs in the above code; I have only proved it correct, not tried it."
> 
Donald Knuth

