#include <fstream>
#include <iostream>

template <class Graph>
inline void BiCore(Graph &G, size_t num_buckets = 16, size_t bipartition = 2, size_t peel_core_alpha = 0, size_t peel_core_beta = 0)
{
    std::cout << "begin" << std::endl;
    auto G = graph; 
    auto changed = false;
    
    // AlphaMax[v][B]
    auto AlphaMax = sequence<sequence<size_t>>(n_b, [&G, &n_a](size_t i){return sequence<size_t>(1 + G.get_vertex(i + n_a).out_degree(), [](size_t i){return 0;});});
    // BetaMax[u][A]
    auto BetaMax = sequence<sequence<size_t>>(n_a, [&G](size_t i) { return sequence<size_t>(1 + G.get_vertex(i).out_degree(), [](size_t i) { return 0; }); });

    auto ret = KCore(G, num_buckets);
    const uintE delta = static_cast<size_t>(pbbslib::reduce_max(ret));
    // delta is the max unicore
    auto PeelFixAllA = [&]()
    {
        for (int i = 0; i < delta + 1; ++i)
        {
            timer t_in;
            t_in.start();
            auto retA = PeelFixA(G, BetaMax, AlphaMax, core, bipartition, num_buckets);
        }
    }
}
template <class Graph>
inline std::pair<size_t, size_t> PeelFixA(Graph &G, size_t max_beta, size_t max_alpha, size_t core, size_t bipartition = 2, size_t num_buckets = 16)
{
    timer bt, ft, pt; /// bt: begin time, ft: finish time, pt: processing time
    pt.start();
    // get degree of all vertices in A and delete all with degree < max_alpha
    // then, decrement degree of all neighboring vertices in A by 1
    for (auto v : G.vertices())
    {
        if (G.get_vertex(v).out_degree() < max_alpha)
        {
            
            auto ngh = G.get_vertex(v).edgeMapCount_sparse(); 
            G.remove_vertex(v);
        }
    }
    auto msgA = "complete PeelFixA";
}

template <class Graph>


