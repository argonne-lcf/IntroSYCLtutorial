# Synopsis

The Marching Cubes algorithm is one of the fundamental building blocks in scientific visualization. It is widely used to create polygonal meshes that represent isosurfaces in 3-dimensional scalar fields.

This project provides a parallel implementation of the Marching Cubes algorithm using SYCL.

# Tutorial

- [03-MarchingCubes.ipynb](03-MarchingCubes.ipynb)

# Compile & Run

The provided Jupyter notebook is self-contained and includes a script to build and run the code. Simply follow the instructions within the notebook to execute the implementation.

# Source Material

This implementation is based on the [NVIDIA CUDA SDK](https://github.com/NVIDIA/cuda-samples/tree/master/Samples/5_Domain_Specific/marchingCubes).

Erik Pautsch worked on porting the code during Summer 2024 as part of an internship at Argonne National Laboratory.

The data in the `data` directory was sourced from the CUDA SDK and was originally available at [http://www9.informatik.uni-erlangen.de/External/vollib/](http://www9.informatik.uni-erlangen.de/External/vollib/).

# Acknowledgements

This research used resources of the Argonne Leadership Computing Facility, a DOE Office of Science User Facility supported under Contract DE-AC02-06CH11357. Argonne National Laboratory’s work was supported by the U.S. Department of Energy, Office of Science, under contract DE-AC02-06CH11357.

All rights reserved. Copyright © Argonne National Laboratory. UChicago LLC.




