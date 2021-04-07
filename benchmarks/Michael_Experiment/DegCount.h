#pragma once

#include "gbbs/gbbs.h"
#include "gbbs/pbbslib/sparse_additive_map.h"
#undef NDEBUG
#include <assert.h>

namespace gbbs
{
    template <class Sequence, class Graph>
    void check(Graph &G, Sequence &ET)
    {
        std::cout << "size: " << ET.size() << "\n";

        parallel_for(0, ET.size(), [&](size_t i) {
            auto v = std::get<0>(ET[i]);
            uintE count = std::get<1>(ET[i]);
            assert(G.get_vertex(v).out_degree() == count);
        });
        std::cout << "finished"
                  << "\n";
    }

    template <class Graph>
    void DegCount1(Graph &G)
    {
        using W = typename Graph::weight_type;

        auto empty = std::make_tuple(UINT_E_MAX, UINT_E_MAX);
        auto ST = pbbslib::sparse_additive_map<uintE, uintE>(G.n, empty);

        auto map_f = [&ST](const uintE &u, const uintE &v, const W &wgh) {
            ST.insert(std::make_tuple(u, 1));
        };

        G.mapEdges(map_f);

        auto ET = ST.entries();
        check(G, ET);
    }

    template <class Graph>
    void DegCount2(Graph &G)
    {
        using W = typename Graph::weight_type;

        auto empty = std::make_tuple(UINT_E_MAX, UINT_E_MAX);
        auto ST = pbbslib::sparse_additive_map<uintE, uintE>(G.n, empty);

        auto map_f = [&ST](const uintE &u, const uintE &v, const W &wgh) {
            ST.insert(std::make_tuple(u, 1));
        };

        parallel_for(0, G.n, [&](size_t i) {
            auto v = G.get_vertex(i);
            v.out_neighbors().map(map_f);
        });

        auto ET = ST.entries();
        check(G, ET);
    }
}
