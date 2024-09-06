# Parallel single-source shortest path algorithm 

<p align="center">
  <img src="https://imgs.xkcd.com/comics/space_typography.png" alt="Space Typography">
</p>

_Comic by Randall Munroe, [XKCD](https://xkcd.com/590/)_

In the spirit of exploring complex ideas with clarity, as demonstrated in the comic above, we turn our attention to a significant topic in algorithmic graph theory: the shortest path problem. This challenge extends to the single-source shortest paths (SSSP) problem, which involves finding the shortest paths from a given source vertex $s$ to all other vertices in the graph.

Traditionally, this problem is addressed using classical sequential algorithms like Dijkstra's algorithm and the Bellman-Ford algorithm, as discussed in both Jeff Erickson's book on algorithms[^1] and *Introduction to Algorithms, 4th Edition* by Cormen, Leiserson, Rivest, and Stein[^2]. However, leveraging insights from the paper on the Delta-stepping algorithm by Meyer and Sanders[^3], I introduce a parallel approach designed to solve the SSSP problem. The Delta-stepping algorithm offers a sophisticated method to improve efficiency and performance in solving this fundamental problem. This is my attempt at studying such a problem.

[^1]: Jeff Erickson, *Algorithms*, available at [http://jeffe.cs.illinois.edu/teaching/algorithms/](http://jeffe.cs.illinois.edu/teaching/algorithms/).
[^2]: T. H. Cormen, C. E. Leiserson, R. L. Rivest, and C. Stein, *Introduction to Algorithms, 4th Edition*, MIT Press, 2022.
[^3]: U. Meyer and P. Sanders, "Δ-stepping: a parallel single source shortest path algorithm," in *European Symposium on Algorithms*, Springer, Berlin, Heidelberg, 2000, pp. 393-404.

## Delta stepping algorithm 


The delta stepping algorithm is designed to iteratively adjust vertex distances in a graph until they reach their final values. It does this by using a series of corrections throughout its steps. 

In this algorithm, vertices are placed into an array of buckets, with each bucket representing a specific range of distances determined by a parameter $\Delta$.  During each phase, the algorithm process vertices from the first non-empty bucket, relaxing their outgoing edges that have weights up to $\Delta$. Edges with weight greater than $\Delta$ are only relaxed later, after ensuring that the starting vertices have finished. The value $\Delta$, referred to as the step width or bucket width, is a positive number.  

To achieve parallelism, the algorithm removes all vertices from the first non-empty bucket simulatneously and relaxes their light edges. If a vertex $v$ is removed but does not yet have a final distance, it can be added back to the bucket, and its light edges will be relaxed again. Heavy edges are relaxed once the bucket is fully processed and becomes empty. The algorithm then proceeds to the next non-empty bucket and continues the process. 


The maximum shortest path weight for a source vertex $s$ is define as 

$$L(s) = \\max\\{dist(s, v) : dist(s, v) < \infty\\}$$

where, $dist(v)$ represents the distance between the source vertex $s$ and another vertex $v$, The size of a path if defined by the number of edges traversed in that path. Light edges are those with weights up to the value $\Delta$, while the heavy edges have weights greater than $\Delta$.

Delta Stepping:

- The Delta stepping algorithm is used for the Single-Source Shortest Paths (SSSP) problem, like Dijkstra's algorithm, but it is designed to work efficiently in parallel and on distributed systems.
- It organizes vertices into buckets based on their tentative distance values, similar to a more granular and distributed version of Dijkstra's priority queue.
- The algorithm processes vertices in small $\Delta$ which is a parameter that controls the trade-off between parallelism and work efficiency. Each bucket can be processed in parallel.
- Delta stepping relaxes edges within a bucket before moving to the next one, akin to Dijkstra’s systematic relaxation of edges, but it allows for parallel relaxation of edges within the same bucket.


<p align="center">
  <img src="images/delta.jpg" alt="SSSP" width="550" height="300">
</p>

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

# Compile on Dev Cloud
To make it as simple as possible currently we will just compile the code once on dev cloud and in the directory `sssp` run the following command:

```bash
icpx -fsycl delta-stepping.cpp -o delta-stepping -std=c++17 -lOpenCL
```

Since we need access to a GPU run the following command to request a node: 

```bash
qsub -I -l nodes=1:gpu:ppn=2 -d .
```


## TODO

- Proofread material and verify the accuracy of the algorithm pseudocode.
- Replace hard-coded graphs with `CLI` functionality and the ability to read from files.
- Implement Google Test to ensure correctness and identify bugs.
- Add visualizations using the [Manim](https://3b1b.github.io/manim/index.html#) engine.
- Working on a CUDA implementation coming soon!

