#include "ClaireTest.h"
namespace gbbs
{

  
  template <class Graph>
  double BFS_runner(Graph &G, commandLine P)
  {
    uintE src = static_cast<uintE>(P.getOptionLongValue("-src", 0));
    std::cout << "### Application: BFS" << std::endl;
    std::cout << "### Graph: " << P.getArgument(0) << std::endl;
    std::cout << "### Threads: " << num_workers() << std::endl;
    std::cout << "### n: " << G.n << std::endl;
    std::cout << "### m: " << G.m << std::endl;
    std::cout << "### Params: -src = " << src << std::endl;
    std::cout << "### ------------------------------------" << std::endl;
    std::cout << "### ------------------------------------" << std::endl;
    timer t;
    t.start();
    auto parents = BFS(G, src);
    double tt = t.stop();
    std::cout << "### Running Time: " << tt << std::endl;
    std::cout << "Out degree of each vertex" << std::endl;
    for (int i = 0; i < G.n; i++)
    { //added code
      std::cout << "Vertex[" << i << "] = " << G.get_vertex(i).out_degree() << std::endl;
    }
    return tt;
  }
} // namespace gbbs
generate_main(gbbs::BFS_runner, false);