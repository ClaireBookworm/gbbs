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
}