#include <fstream>
#include <iostream>

template <class Graph> inline void BiCore(Graph &G) {
    std::cout << "begin" << std::endl;
    auto G = graph; // imagine this is the graph you want to run the algorithm on
    // take in the graph and find any vertices with degree less than max alpha or beta
    // for each such vertex, find the neighbors of that vertex
    // for the neighbors decrement the degree of the vertex by 1
    // repeat until all vertices have degree less than max alpha or beta
    size_t max_alpha = 0;
    size_t max_beta = 0;
    for (auto v : G.vertices()) {
        if (G.degree(v) > max_alpha) {
            // max_alpha = G.degree(v);
        }
    }

    auto ret = KCore(G, num_buckets);
	const uintE delta = static_cast<size_t>(pbbslib::reduce_max(ret));

    auto PeelFixAllA = [&]() {
        for (int i = 0; i < delta+1; ++i) {
            timer t_in; t_in.start();
            auto retA = PeelFixA(G, BetaMax, AlphaMax, core, bipartition, num_buckets);
    }
}
template <class Graph> inline std::pair<size_t, size_t> PeelFixA (Graph &G, size_t max_beta, size_t max_alpha, size_t core, size_t bipartition = 2, size_t num_buckets = 16) {
    timer bt, ft, pt; /// bt: begin time, ft: finish time, pt: processing time
    pt.start();


}
