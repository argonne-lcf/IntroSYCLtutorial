# Parallel single-source shortest path algorithm 

The single-source shortest path problem is a classic graph theory problem where, given a graph and a source vertex, we aim to find the shortest path to every other vertex in the graph. Parallel algorithms for this problem aim to speed up the computation by distributing the work across multiple processors.

In this example we study the Delta stepping algorithm. This algorithm is efficient because it focuses on exploring the most likely places to find the shortest path first. This helps avoid exploring unnecessary places, saving time and effort.

<hr>  
<p align="center">
  <img src="sssp/animation/MovingVertices.gif" alt="Space Points">
</p>

Start here
-[01-intro-sssp.ipynb](01-intro-sssp.ipynb)

## Acknowledgements
This research used resources of the Argonne Leadership Computing Facility, which is a DOE Office of Science User Facility supported under Contract DE-AC02-06CH11357. Argonne National Laboratory’s work was supported by the U.S. Department of Energy, Office of Science, under contract DE-AC02-06CH11357.
