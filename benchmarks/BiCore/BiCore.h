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
#include <fstream>

namespace gbbs
{
	// bipartition gives the last vertex id in first partition
	// size_t bipartition = P.getOptionLongValue("-bi", 2);
	struct PeelingMemory{
		hist_table<uintE, uintE>* em = nullptr;//, em_b;
		using id_dyn_arr = pbbslib::dyn_arr<uintE>; 
		id_dyn_arr* bkts = nullptr;
		size_t total_buckets = 0;
		PeelingMemory(){}
		void alloc(const size_t size, const size_t num_buckets){ 
			auto empty = std::make_tuple(UINT_E_MAX, 0);
			if(em == nullptr)
				em = new hist_table<uintE, uintE>(empty, size);
			if(bkts == nullptr) 
				bkts = pbbslib::new_array<id_dyn_arr>(num_buckets);
			//std::cout<<std::get<0>(empty)<<" "<<std::get<1>(empty)<<std::endl;
			total_buckets = num_buckets;
		}
		inline void init(){
			auto empty = std::make_tuple(UINT_E_MAX, 0);
			par_for(0, em->size, 2048, [&] (size_t i) { em->table[i] = empty; });
		}
		~PeelingMemory(){ 
			em->del();
			delete em;
			for (size_t i = 0; i < total_buckets; i++) bkts[i].clear();
			pbbslib::free_array(bkts);
		}
	};

	template <class Graph>
	inline void BiCore(Graph &G, size_t num_buckets = 16, size_t bipartition = 2, size_t peel_core_alpha=0, size_t peel_core_beta=0)
	{
		std::cout << "begin" << std::endl;

		const size_t n = G.n;
		const size_t n_b = n - bipartition - 1;
		const size_t n_a = bipartition + 1;

		// AlphaMax[v][B]
		auto AlphaMax = sequence<sequence<size_t>>(n_b, [&G, &n_a](size_t i){ return sequence<size_t>(1+G.get_vertex(i+n_a).out_degree(),[](size_t i){return 0;}); });
		// BetaMax[u][A]
		auto BetaMax = sequence<sequence<size_t>>(n_a, [&G](size_t i){ return sequence<size_t>(1+G.get_vertex(i).out_degree(),[](size_t i){return 0;}); });

		// if(peel_core_alpha!=0){
		// 	PeelFixA(G, BetaMax, AlphaMax, peel_core_alpha, bipartition, num_buckets);
		// 	std::cout << "complete PeelFixA" << std::endl;
		// 	return;
		// }else if(peel_core_beta!=0){
		// 	PeelFixB(G, BetaMax, AlphaMax, peel_core_beta, bipartition, num_buckets);
		// 	std::cout << "complete PeelFixB" << std::endl;
		// 	return;
		// }
		auto ret = KCore(G, num_buckets);
		const uintE delta = static_cast<size_t>(pbbslib::reduce_max(ret));

		auto msgA = pbbslib::new_array_no_init<std::tuple<size_t,size_t,float_t>>(delta+1);
		auto msgB = pbbslib::new_array_no_init<std::tuple<size_t,size_t,float_t>>(delta+1);

		auto init_f = [&](PeelingMemory* mem){mem->alloc((size_t)G.m,num_buckets);};
		auto finish_f = [&](PeelingMemory* mem){mem->~PeelingMemory();};
		// block serialization
		// estimate work
		// prefix sum + greedy blocking
		// for{
		// 	par_for{
		// 		//estimate work and use it to inform the size of par_for so each run of par_for is similar
		// 	}
		// }
		PeelingMemory* mem = new PeelingMemory();
		init_f(mem);
		for(int core = 1; core <= delta; core++){
			timer t_in; t_in.start();
			// keep the array and reconstruct bucket each time
			mem->init();
			auto retA = PeelFixA(G, BetaMax, AlphaMax, core, bipartition, mem);
			msgA[core]=std::make_tuple(std::get<0>(retA),std::get<1>(retA),t_in.stop());
			mem->init();
			auto retB = PeelFixB(G, BetaMax, AlphaMax, core, bipartition, mem);
			msgB[core]=std::make_tuple(std::get<0>(retB),std::get<1>(retB),t_in.stop());
		}
		finish_f(mem);
		// parallel_for_alloc<PeelingMemory>(init_f, finish_f, 1,delta+1,[&](size_t core, PeelingMemory* mem){
		// 	timer t_in; t_in.start();
		// 	// keep the array and reconstruct bucket each time
		// 	mem->init();
		// 	auto retA = PeelFixA(G, BetaMax, AlphaMax, core, bipartition, mem);
		// 	msgA[core]=std::make_tuple(std::get<0>(retA),std::get<1>(retA),t_in.stop());
		// 	mem->init();
		// 	auto retB = PeelFixB(G, BetaMax, AlphaMax, core, bipartition, mem);
		// 	msgB[core]=std::make_tuple(std::get<0>(retB),std::get<1>(retB),t_in.stop());
		// });

		debug(for(size_t core=1; core<=delta; ++core) std::cout<<"coreA "<<core<<" "<<std::get<0>(msgA[core])<<" "<<std::get<1>(msgA[core])<<" "<<std::get<2>(msgA[core])<<'\n');
		debug(for(size_t core=1; core<=delta; ++core) std::cout<<"coreB "<<core<<" "<<std::get<0>(msgB[core])<<" "<<std::get<1>(msgB[core])<<" "<<std::get<2>(msgB[core])<<'\n');
		pbbslib::free_array(msgA);
		pbbslib::free_array(msgB);
	}

	template <class Graph>
	inline std::pair<size_t,size_t> PeelFixA(Graph &G, sequence<sequence<size_t>> &BetaMax, 
	sequence<sequence<size_t>> &AlphaMax, size_t alpha,
	 size_t bipartition = 2, PeelingMemory* mem = nullptr)
	{
		timer bt,ft,pt;
		pt.start();

		const size_t n = G.n;
		const size_t n_b = n - bipartition - 1;
		const size_t n_a = bipartition + 1;

		size_t finished = 0, rho_alpha = 0, max_beta = 0;
		// [0, bipartition] interval for U
		// [bipartition+1, n-1]  interval V
		auto D =
			sequence<uintE>(n, [&](size_t i) {
				return G.get_vertex(i).out_degree();
			});

		auto mask = sequence<std::tuple<bool, uintE>>(n_a, [&](size_t i) {
			return std::make_tuple<bool, uintE>((G.get_vertex(i).out_degree() < alpha)&(i<n_a), 0);
		});

		auto uDel = vertexSubsetData<uintE>(n_a, std::move(mask));

		auto cond_fu = [&D, &alpha](const uintE &u) { return D[u] >= alpha; };
		auto cond_fv = [&D, &max_beta](const uintE &v) { return D[v] > max_beta; };
		// instead of tracking whether a vertex is peeled or not using a boolean arr, we can just see whether its degree is above or below the cutoff

		auto clearZeroV = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE v = std::get<0>(p), edgesRemoved = std::get<1>(p);
			uintE new_deg = D[v] - edgesRemoved;
			D[v] = new_deg;
			if (new_deg == 0)
				return wrap(v, 0);
			return std::nullopt;
		};

		auto clearU = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE u = std::get<0>(p), edgesRemoved = std::get<1>(p);
			uintE new_deg = D[u] - edgesRemoved;
			D[u] = new_deg;
			if (new_deg < alpha)
			{
				if(max_beta>0)
					pbbslib::write_max(&BetaMax[u][alpha],max_beta);
				return wrap(u, 0);
			}
			return std::nullopt;
		};

		// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
		// (0,0) --> (a,0)
		// (0,0) --> (a-1,0) (blocked parallelism)
		// O(delta * n)
		while (!uDel.isEmpty())
		{
			vertexSubsetData<uintE> vDel = nghCount(G, uDel, cond_fv, clearZeroV, *(mem->em), no_dense);
			uDel = nghCount(G, vDel, cond_fu, clearU, *(mem->em), no_dense);
		}

		size_t vCount = 0;

		auto Dv =
			sequence<uintE>(n, [&](size_t i) {
				if (i <= bipartition || D[i] == 0)
					return std::numeric_limits<uintE>::max();
				return D[i];
			});

		auto bbuckets = make_vertex_buckets(n,Dv,mem->bkts,increasing,mem->total_buckets);
		// make num_buckets open buckets such that each vertex i is in D[i] bucket
		// note this i value is not real i value; realI = i+bipartition+1 or i+n_a

		// delayed_sequence (fake sequence that acts like sequence but doesn't allocate memory)
		// lambda wrapper
		// reduce_add(delayed_seq.slice())
		vCount = pbbslib::reduce_add(sequence<uintE>(n_b, [&](size_t i) {return D[i+n_a]>0;}));

		auto getVBuckets = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE v = std::get<0>(p), edgesRemoved = std::get<1>(p);
			uintE deg = D[v];
			uintE new_deg = std::max(deg - edgesRemoved, static_cast<uintE>(max_beta));
			D[v] = new_deg;
			return wrap(v, bbuckets.get_bucket(new_deg));
		};
		pt.stop();

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

			par_for(0, activeV.size(), [&](size_t i) {
				size_t index = activeV.vtx(i)-n_a;
				par_for(1, max_beta, [&](size_t j) {
					pbbslib::write_max(&AlphaMax[index][j],alpha);
				});
			});
			ft.start();
			vertexSubsetData deleteU = nghCount(G, activeV, cond_fu, clearU, *(mem->em), no_dense);
			// "deleteU" is a wrapper storing a sequence id of deleted vertices in U

			vertexSubsetData movedV = nghCount(G, deleteU, cond_fv, getVBuckets, *(mem->em), no_dense);
			// "movedV" is a wrapper storing a sequence of tuples like (id, newBucket)
			ft.stop();
			bt.start();
			bbuckets.update_buckets(movedV);
			bt.stop();
			rho_alpha++;
		}
		//bbuckets.del();
		return std::pair<size_t,size_t>(rho_alpha,max_beta);
	}

	template <class Graph>
	inline std::pair<size_t,size_t> PeelFixB(Graph &G, sequence<sequence<size_t>> &BetaMax, 
	sequence<sequence<size_t>> &AlphaMax, size_t beta,
	size_t bipartition = 2, PeelingMemory* mem = nullptr)
	{
		timer bt,ft,pt;
		pt.start();
		const size_t n = G.n;
		const size_t n_b = n - bipartition - 1;
		const size_t n_a = bipartition + 1;

		size_t finished = 0, rho_beta = 0, max_alpha = 0;

		auto D =
			sequence<uintE>(n, [&](size_t i) {
				return G.get_vertex(i).out_degree();
			});

		auto mask = sequence<std::tuple<bool, uintE>>(n, [&](size_t i) {
			if (i < n_a) return std::make_tuple<bool, uintE>(false, 0);
			return std::make_tuple<bool, uintE>(G.get_vertex(i).out_degree() < beta, 0);
		});
		auto vDel = vertexSubsetData<uintE>(n, std::move(mask));

		auto cond_fv = [&D, &beta](const uintE &v) { return D[v] >= beta; };
		auto cond_fu = [&D, &max_alpha](const uintE &u) { return D[u] > max_alpha; };

		// if the U list is empty
		auto clearZeroU = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE u = std::get<0>(p), edgesRemoved = std::get<1>(p);
			uintE new_deg = D[u] - edgesRemoved;
			D[u] = new_deg;
			if (new_deg == 0)
				return wrap(u, 0);
			return std::nullopt;
		};

		auto clearV = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE v = std::get<0>(p), edgesRemoved = std::get<1>(p);
			uintE new_deg = D[v] - edgesRemoved;
			D[v] = new_deg;
			if (new_deg < beta)
			{
				if(max_alpha>0)
					pbbslib::write_max(&AlphaMax[v-n_a][beta],max_alpha);
				return wrap(v, 0);
			}
			return std::nullopt;
		};

		// nghCount counts the # of neighbors
		while (!vDel.isEmpty())
		{
			vertexSubsetData<uintE> uDel = nghCount(G, vDel, cond_fu, clearZeroU, *(mem->em), no_dense);
			std::cout<<"debug size: "<<mem->em->size<<std::endl;
			vDel = nghCount(G, uDel, cond_fv, clearV, *(mem->em), no_dense);
		}

		size_t uCount = pbbslib::reduce_add(sequence<uintE>(n_a, [&](size_t i) {return (D[i]>0);}));

		auto Du =
			sequence<uintE>(n, [&](size_t i) {
				if (i > bipartition || D[i] == 0)
					return std::numeric_limits<uintE>::max();
				return D[i];
			});

		//auto abuckets = make_vertex_buckets(n,Du,increasing,num_buckets);
		auto abuckets = make_vertex_buckets(n,Du,mem->bkts,increasing,mem->total_buckets);
		// makes num_buckets open buckets
		// for each vertex [0, n_a-1], it puts it in bucket D[i]
		auto getUBuckets = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE u = std::get<0>(p), edgesRemoved = std::get<1>(p);
			uintE new_deg = std::max(D[u] - edgesRemoved, static_cast<uintE>(max_alpha));
			D[u] = new_deg;
			return wrap(u, abuckets.get_bucket(new_deg));
		};

		pt.stop();
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
			par_for(0, activeU.size(), [&](size_t i) {
				size_t index = activeU.vtx(i);
				par_for(1, max_alpha, [&](size_t j) {
					pbbslib::write_max(&BetaMax[index][j],beta);
				});
			});
			
			ft.start();
			vertexSubsetData deleteV = nghCount(G, activeU, cond_fv, clearV, *(mem->em), no_dense);
			vertexSubsetData movedU = nghCount(G, deleteV, cond_fu, getUBuckets, *(mem->em), no_dense);
			ft.stop();
			bt.start();
			abuckets.update_buckets(movedU);
			bt.stop();
			rho_beta++;
		}
		//abuckets.del();
		return std::pair<size_t,size_t>(rho_beta,max_alpha);
	}

} // namespace gbbs
