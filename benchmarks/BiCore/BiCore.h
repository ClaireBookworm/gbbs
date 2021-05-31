// This code is part of the project "Theoretically Efficient Parallel Graph
// Algorithms Can Be Fast and Scalable", presented at Symposium on Parallelism
// in Algorithms and Architectures, 2018.
// Copyright (c) 2018 Laxman Dhulipala, Guy Blelloch, and Julian Shun
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all  copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "gbbs/gbbs.h"
#include "gbbs/julienne.h"
#include "benchmarks/KCore/JulienneDBS17/KCore.h"

namespace gbbs
{

	using arr_dyn = pbbslib::dyn_arr<size_t>;

	// bipartition gives the last vertex id in first partition
	// size_t bipartition = P.getOptionLongValue("-bi", 2);

	template <class Graph>
	inline void BiCore(Graph &G, size_t num_buckets = 16, size_t bipartition = 2)
	{
		std::cout << "begin" << std::endl;

		const size_t n = G.n;
		const size_t n_b = n - bipartition - 1;
		const size_t n_a = bipartition + 1;
		auto ret = KCore(G, num_buckets);
		uintE delta = pbbslib::reduce_max(ret);

		// AlphaMax[v][B]
		auto AlphaMax = sequence<arr_dyn>(n_b, [](size_t i){ return arr_dyn(16);});
		// BetaMax[u][A]
		auto BetaMax = sequence<arr_dyn>(n_a, [](size_t i){ return arr_dyn(16);});

		PeelFixA(G, BetaMax, AlphaMax, 1, num_buckets, bipartition);
		std::cout << "complete PeelFixA" << std::endl;

		PeelFixB(G, BetaMax, AlphaMax,1, num_buckets, bipartition);
		std::cout << "complete PeelFixB" << std::endl;

		size_t size_AlphaMax = pbbslib::reduce_add(sequence<size_t>(n_b,[&AlphaMax](size_t i){return AlphaMax[i].size;}));
		size_t size_BetaMax = pbbslib::reduce_add(sequence<size_t>(n_a,[&BetaMax](size_t i){return BetaMax[i].size;}));

		std::cout<<size_AlphaMax<<"  "<<size_BetaMax<<std::endl;
	}

	template <class Graph>
	inline void PeelFixA(Graph &G, sequence<arr_dyn> &BetaMax, sequence<arr_dyn> &AlphaMax, size_t alpha, size_t num_buckets = 16, size_t bipartition = 2)
	{

		const size_t n = G.n;
		const size_t n_b = n - bipartition - 1;
		const size_t n_a = bipartition + 1;

		size_t finished = 0, rho_alpha = 0, max_beta = 0;

		// [0, bipartition] interval for U
		// [bipartition+1, n-1]  interval V

		// uintE is vertex id
		// uintT is edge id

		auto em = hist_table<uintE, uintE>(std::make_tuple(UINT_E_MAX, 0), (size_t)G.m / 50);
		auto D =
			sequence<uintE>(n, [&](size_t i) {
				return G.get_vertex(i).out_degree();
			});

		auto mask = sequence<std::tuple<bool, uintE>>(n, [&](size_t i) {
			if (i >= n_a)
				return std::make_tuple<bool, uintE>(false, 0);
			return std::make_tuple<bool, uintE>(G.get_vertex(i).out_degree() < alpha, 0);
		});

		auto uDel = vertexSubsetData<uintE>(n, std::move(mask));

		auto cond_fu = [&D, &alpha](const uintE &u) { return D[u] >= alpha; };
		auto cond_fv = [&D, &max_beta](const uintE &v) { return D[v] > max_beta; };
		// instead of tracking whether a vertex is peeled or not using a boolean arr, we can just see whether its degree is above or below the cutoff

		auto clearZeroV = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE v = std::get<0>(p), edgesRemoved = std::get<1>(p);
			uintE deg = D[v];
			uintE new_deg = deg - edgesRemoved;
			D[v] = new_deg;
			if (new_deg == 0)
				return wrap(v, 0);
			return std::nullopt;
		};

		auto clearU = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE u = std::get<0>(p), edgesRemoved = std::get<1>(p);
			uintE deg = D[u];
			uintE new_deg = deg - edgesRemoved;
			D[u] = new_deg;
			if (new_deg < alpha && max_beta>0)
			{
				if(BetaMax[u].size<=alpha){
					BetaMax[u].resize(alpha+1-BetaMax[u].size);
					BetaMax[u].size=alpha+1;
				}
				BetaMax[u].A[alpha] = max_beta;

				return wrap(u, 0);
			}
			return std::nullopt;
		};

		// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
		while (!uDel.isEmpty())
		{
			std::cout << "uDel size " << uDel.size() << std::endl;
			vertexSubsetData<uintE> vDel = nghCount(G, uDel, cond_fv, clearZeroV, em, no_dense);
			std::cout << "vDel size " << vDel.size() << std::endl;
			uDel = nghCount(G, vDel, cond_fu, clearU, em, no_dense);
		}

		size_t vCount = 0;

		auto vD =
			sequence<uintE>(n, [&](size_t i) {
				if (i <= bipartition || D[i] == 0)
					return std::numeric_limits<uintE>::max();
				return D[i];
			});

		auto bbuckets = make_vertex_buckets(n, vD, increasing, num_buckets);
		// make num_buckets open buckets such that each vertex i is in D[i] bucket
		// note this i value is not real i value; realI = i+bipartition+1 or i+n_a
		timer bt;

		vCount = pbbslib::reduce_add(sequence<uintE>(n, [&](size_t i) {
			if (i <= bipartition || D[i] == 0)
				return 0;
			return 1;
		}));

		auto getVBuckets = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE v = std::get<0>(p), edgesRemoved = std::get<1>(p);
			uintE deg = D[v];
			uintE new_deg = std::max(deg - edgesRemoved, static_cast<uintE>(max_beta));
			D[v] = new_deg;
			return wrap(v, bbuckets.get_bucket(new_deg));
		};

		std::cout << "initial peeling done, vCount left is " << vCount << std::endl;

		while (finished != vCount)
		{
			bt.start();
			auto vbkt = bbuckets.next_bucket();
			bt.stop();
			max_beta = std::max(max_beta, vbkt.id);
			if (vbkt.id == 0)
				continue;

			auto activeV = vertexSubset(n, std::move(vbkt.identifiers)); // container of vertices
			finished += activeV.size();

			parallel_for(0, activeV.size(), [&](size_t i) {
				size_t index = activeV.vtx(i)-n_a;
				uintE expand_size = max_beta-(AlphaMax[index].size-1);
				if(expand_size>0){
					AlphaMax[index].resize(expand_size);
					AlphaMax[index].size=max_beta+1;
				}
				parallel_for(1, max_beta, [&](size_t j) {
					AlphaMax[index].A[j] = std::max(AlphaMax[index].A[j], alpha);
				});
			});

			vertexSubsetData deleteU = nghCount(G, activeV, cond_fu, clearU, em, no_dense);
			// "deleteU" is a wrapper storing a sequence id of deleted vertices in U

			vertexSubsetData movedV = nghCount(G, deleteU, cond_fv, getVBuckets, em, no_dense);
			// "movedV" is a wrapper storing a sequence of tuples like (id, newBucket)

			bt.start();
			bbuckets.update_buckets(movedV);
			bt.stop();
			rho_alpha++;
		}
		std::cout << "### rho_alpha = " << rho_alpha << " beta_{max} = " << max_beta << "\n";
		debug(bt.reportTotal("bucket time"));
		return;
	}

	template <class Graph>
	inline void PeelFixB(Graph &G, sequence<arr_dyn> &BetaMax, sequence<arr_dyn> &AlphaMax, size_t beta, size_t num_buckets = 16, size_t bipartition = 2)
	{
		const size_t n = G.n;
		const size_t n_b = n - bipartition - 1;
		const size_t n_a = bipartition + 1;

		size_t finished = 0, rho_beta = 0, max_alpha = 0;

		auto em = hist_table<uintE, uintE>(std::make_tuple(UINT_E_MAX, 0), (size_t)G.n / 50);
		auto D =
			sequence<uintE>(n, [&](size_t i) {
				return G.get_vertex(i).out_degree();
			});

		auto mask = sequence<std::tuple<bool, uintE>>(n, [&](size_t i) {
			if (i < n_a)
				return std::make_tuple<bool, uintE>(false, 0);
			return std::make_tuple<bool, uintE>(G.get_vertex(i).out_degree() < beta, 0);
		});

		auto vDel = vertexSubsetData<uintE>(n, std::move(mask));

		auto cond_fv = [&D, &beta](const uintE &v) { return D[v] >= beta; };
		auto cond_fu = [&D, &max_alpha](const uintE &u) { return D[u] > max_alpha; };

		// if the U list is empty
		auto clearZeroU = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE u = std::get<0>(p), edgesRemoved = std::get<1>(p);
			uintE deg = D[u];
			uintE new_deg = deg - edgesRemoved;
			D[u] = new_deg;
			if (new_deg == 0)
				return wrap(u, 0);
			return std::nullopt;
		};

		auto clearV = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE v = std::get<0>(p), edgesRemoved = std::get<1>(p);
			uintE deg = D[v];
			uintE new_deg = deg - edgesRemoved;
			D[v] = new_deg;
			if (new_deg < beta && max_alpha>0)
			{
				if(AlphaMax[v-n_a].size<=beta){
					AlphaMax[v-n_a].resize(beta+1-AlphaMax[v-n_a].size);
					AlphaMax[v-n_a].size=beta+1;
				}
				AlphaMax[v-n_a].A[beta] = max_alpha;
				return wrap(v, 0);
			}
			return std::nullopt;
		};

		// nghCount counts the # of neighbors
		while (!vDel.isEmpty())
		{
			vertexSubsetData<uintE> uDel = nghCount(G, vDel, cond_fu, clearZeroU, em, no_dense);
			vDel = nghCount(G, uDel, cond_fv, clearV, em, no_dense);
		}

		size_t uCount = 0;

		auto abuckets = make_vertex_buckets(n_a, D, increasing, num_buckets);
		// makes num_buckets open buckets
		// for each vertex [0, n_a-1], it puts it in bucket D[i]
		timer bt;

		auto getUBuckets = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE u = std::get<0>(p), edgesRemoved = std::get<1>(p);
			uintE deg = D[u];
			uintE new_deg = std::max(deg - edgesRemoved, static_cast<uintE>(max_alpha));
			D[u] = new_deg;
			return wrap(u, abuckets.get_bucket(new_deg));
		};

		uCount = pbbslib::reduce_add(sequence<uintE>(n, [&](size_t i) {
			if (i > bipartition || D[i] == 0)
				return 0;
			return 1;
		}));

		std::cout << "initial peeling done, uCount left is " << uCount << std::endl;

		while (finished != uCount)
		{
			bt.start();
			auto ubkt = abuckets.next_bucket();
			bt.stop();
			max_alpha = std::max(max_alpha, ubkt.id);

			if (ubkt.id == 0)
				continue;

			auto activeU = vertexSubset(n, std::move(ubkt.identifiers));
			finished += activeU.size(); // add to finished set

			parallel_for(0, activeU.size(), [&](size_t i) {
				size_t index = activeU.vtx(i);
				uintE expand_size = max_alpha-(BetaMax[index].size-1);
				if(expand_size>0){
					BetaMax[index].resize(expand_size);
					BetaMax[index].size=max_alpha+1;
				}
				parallel_for(1, max_alpha, [&](size_t j) {
					BetaMax[index].A[j] = std::max(BetaMax[index].A[j], beta);
				});
			});

			vertexSubsetData deleteV = nghCount(G, activeU, cond_fv, clearV, em, no_dense);
			vertexSubsetData movedU = nghCount(G, deleteV, cond_fu, getUBuckets, em, no_dense);
			bt.start();
			abuckets.update_buckets(movedU);
			bt.stop();
			rho_beta++;
		}
		std::cout << "### rho_beta = " << rho_beta << " alpha_{max} = " << max_alpha << "\n";
		debug(bt.reportTotal("bucket time"));
	}

} // namespace gbbs
