#pragma once

#include "gbbs/gbbs.h"
#include "gbbs/pbbslib/sparse_additive_map.h"
#undef NDEBUG
#include <assert.h>

namespace gbbs
{
    template <class Graph>
    void DegCount(Graph &G)
    {
        using W = typename Graph::weight_type;

        auto empty = std::make_tuple(UINT_E_MAX - 1, UINT_E_MAX - 1);
        auto ST = pbbslib::sparse_additive_map<uintE, uintE>(G.n, empty);

        auto map_f = [&ST](const uintE &u, const uintE &v, const W &wgh) {
            ST.insert(std::make_tuple(u, 1));
            ST.insert(std::make_tuple(v, 1));
        };

        G.mapEdges(map_f);

        auto ET = ST.entries();
        std::cout << "size: " << ET.size() << "\n";
        for (size_t i = 0; i < G.n; i++)
        {
            auto v = std::get<0>(ET[i]);
            uintE count = std::get<1>(ET[i]) / 2;
            std::cout << v << ", " << G.get_vertex(v).out_degree() << ", " << count << std::endl;
            assert(G.get_vertex(v).out_degree() == count);
        }
        // parallel_for(0, G.n, [&](size_t i) {
        //     auto v = std::get<0>(ET[i]);
        //     uintE count = std::get<1>(ET[i]) / 2;
        //     std::cout << G.get_vertex(v).out_degree() << ", " << count << std::endl;
        //     assert(G.get_vertex(v).out_degree() == count);
        // });
        std::cout << "finished"
                  << "\n";
    }
}
