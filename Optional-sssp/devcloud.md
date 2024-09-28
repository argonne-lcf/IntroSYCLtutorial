# Compile on Dev Cloud
To make it as simple as possible currently we will just compile the code once on dev cloud and in the directory `sssp` run the following command:

```bash
icpx -fsycl delta-stepping.cpp -o delta-stepping -std=c++17 -lOpenCL
```

Since we need access to a GPU run the following command to request a node: 

```bash
qsub -I -l nodes=1:gpu:ppn=2 -d .
```
