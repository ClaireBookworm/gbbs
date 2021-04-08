
#pragma once

#include "gbbs/gbbs.h"
#include "gbbs/pbbslib/sparse_additive_map.h"
#undef NDEBUG
#include <assert.h>

namespace gbbs {

	template <class W>
	struct BFS_F {
		uintE *Parents;
		BFS_F(uintE *_Parents) : Parents(_Parents) {}
		inline bool update(uintE s, uintE d, W w)
		{
			if (Parents[d] == UINT_E_MAX) {
				Parents[d] = s;
				return 1;
			}
			else {
				return 0;
			}
		}
		inline bool updateAtomic(uintE s, uintE d, W w)
		{
			return (pbbslib::atomic_compare_and_swap(&Parents[d], UINT_E_MAX, s));
		}
		inline bool cond(uintE d) { return (Parents[d] == UINT_E_MAX); }
	};

	template <class Graph>
	inline sequence<uintE> BFS(Graph &G, uintE src) {
		using W = typename Graph::weight_type;
		/* Creates Parents array, initialized to all -1, except for src. */
		auto Parents = sequence<uintE>(G.n, [&](size_t i) { return UINT_E_MAX; });
		Parents[src] = src;

		vertexSubset Frontier(G.n, src);
		size_t reachable = 0;
		while (!Frontier.isEmpty())
		{
			std::cout << Frontier.size() << "\n";
			reachable += Frontier.size();
			Frontier = edgeMap(G, Frontier, BFS_F<W>(Parents.begin()), -1, sparse_blocked | dense_parallel);
		}
		std::cout << "Reachable: " << reachable << "\n";
		return Parents;
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

} // namespace gbbs
