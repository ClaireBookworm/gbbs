# Efficient Algorithm for Parallel Bi-core Decomposition

You can find the original paper [here](https://github.com/ClaireBookworm/gbbs/blob/bicore-index/primes-conference-final.pdf), and our published paper [here (ACM-SIAM 2023)](https://epubs.siam.org/doi/abs/10.1137/1.9781611977578.ch2).

Introduction
--------

This is the codebase for our Efficient Algorithm for Parallel Bi-core Decomposition paper. The repository uses Graph Based Benchmark Suite ([GBBS](https://github.com/ParAlg/gbbs)). 

The repository consists of several branches, each corresponding to the four algorithms in our paper:

**SEQ-BASELINE** : branch _bicore-seq-base_

**SEQ-OPTIMIZED** : branch _bicore-seq-optim_

**PAR-BASELINE** : branch _bicore-par-base_

**PAR-OPTIMIZED** : branch _bicore-par-optim_

The code for parallel bi-core peeling is under the directory /benchmarks/BiCore in each of the branches. 

The branch _bicore-index_ contains the parallel index construction and query code and runs **PAR-OPTIMIZED** as a subroutine. The code for **PAR-INDEX** and **PAR-QUERY** is under /benchmarks/BiCoreIndex

The branch _bicore-par-fetch-add_ contains a fetch-and-add based implementation of parallel bi-core peeing.

The branch _bicore-par-aggregate_ contains an implementation of bi-core peeling based on aggregation-based degree update (this is the theoretically efficient algorithm introduced in the paper).

Compilation
--------

Compiler:
* g++ &gt;= 7.4.0 with support for Cilk Plus
* g++ &gt;= 7.4.0 with pthread support (Homemade Scheduler)

Build system:
* Make

The default compilation uses a lightweight scheduler developed at CMU (Homemade)
for parallelism, which results in comparable performance to Cilk Plus. 


Input Formats
-----------
We support the adjacency graph format used by the [Problem Based Benchmark
suite](http://www.cs.cmu.edu/~pbbs/benchmarks/graphIO.html)
and [Ligra](https://github.com/jshun/ligra).

The adjacency graph format starts with a sequence of offsets one for each
vertex, followed by a sequence of directed edges ordered by their source vertex.
The offset for a vertex i refers to the location of the start of a contiguous
block of out edges for vertex i in the sequence of edges. The block continues
until the offset of the next vertex, or the end if i is the last vertex. All
vertices and offsets are 0 based and represented in decimal. The specific format
is as follows:

```
AdjacencyGraph
<n>
<m>
<o0>
<o1>
...
<o(n-1)>
<e0>
<e1>
...
<e(m-1)>
```

### KONECT Input

You can convert a graph in [KONECT](http://konect.cc/) graph format to our adjacency graph format by formating the graph's name as XXX_konect and storing it under /inputs. Then run /utils/toEdgeList.cpp and input the name of the graph XXX into the standard input stream. The code should convert the graph to adjacency format, outputing a file named XXX_adj under /inputs

## Running the Code

Navigate into /benchmarks/BiCore or /benchmarks/BiCoreIndex

Specify the number of threads by 

```
export NUM_THREADS = x
```

Then compile it by

```
make
```

After the compilation, run the code using the following command

```
./BiCore -s -bi BIPARTITION -rounds NUMBER-OF-ROUNDS GRAPH-FILE-PATH
```

The BIPARTITION of a graph is the size of its first partition $- 1$. For example, we have a small example graph named `example`. It comes from the graph [South African Companies](http://konect.cc/networks/brunson_southern-women/). The graph's first partition has 6 vertices. Thus the BIPARTITION$=5$. NUMBER-OF-ROUNDS indicates the number of times to run this by. 

For example, for `example` we run it with

```
./BiCore -s -bi 5 -rounds 1 ../../inputs/example
```

This performs the bi-core peeling for `example` graph

To also construct the bi-core index structure, navigate to /benchmarks/BiCoreIndex and run

```
./BiCoreIndex -s -bi 5 -rounds 1 ../../inputs/example
```

`./BiCoreIndex` performs the bi-core peeling, constructs the bicore index, and performs 10 queries of the $(10,10)$-core for benchmarking the query runtime. 
