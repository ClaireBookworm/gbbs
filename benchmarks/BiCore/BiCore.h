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
#include <vector>
#include <unordered_set>

namespace gbbs
{
	// bipartition gives the last vertex id in first partition
	// size_t bipartition = P.getOptionLongValue("-bi", 2);

	template <class Graph>
	inline void BiCore(Graph &G, size_t num_buckets = 16, size_t bipartition = 2, size_t peel_core_alpha=0, size_t peel_core_beta=0)
	{
		std::cout << "begin" << std::endl;

		const size_t n = G.n; // number of vertices
		const size_t n_b = n - bipartition - 1; // number of vertices in first partition
		const size_t n_a = bipartition + 1; // number of vertices in second partition

		// AlphaMax[v][B]
		auto AlphaMax = sequence<sequence<size_t>>(n_b, [&G, &n_a](size_t i){ return sequence<size_t>(1+G.get_vertex(i+n_a).out_degree(),[](size_t i){return 0;}); });
		// BetaMax[u][A]
		auto BetaMax = sequence<sequence<size_t>>(n_a, [&G](size_t i){ return sequence<size_t>(1+G.get_vertex(i).out_degree(),[](size_t i){return 0;}); });

		if(peel_core_alpha!=0){
			PeelFixA(G, BetaMax, AlphaMax, peel_core_alpha, bipartition, num_buckets);
			std::cout << "complete PeelFixA" << std::endl;
			return;
		}else if(peel_core_beta!=0){
			PeelFixB(G, BetaMax, AlphaMax, peel_core_beta, bipartition, num_buckets);
			std::cout << "complete PeelFixB" << std::endl;
			return;
		}
		auto ret = KCore(G, num_buckets);
		const uintE delta = static_cast<size_t>(pbbslib::reduce_max(ret));
		// max unicor 

		auto msgA = pbbslib::new_array_no_init<std::tuple<size_t,size_t,float_t>>(delta+1);
		auto msgB = pbbslib::new_array_no_init<std::tuple<size_t,size_t,float_t>>(delta+1);
		auto timeA = sequence<double>(delta, 0.0);
		auto timeB = sequence<double>(delta, 0.0);
		auto tTimeA = sequence<double>(delta, 0.0);
		auto tTimeB = sequence<double>(delta, 0.0);

		// double slope = 1.5;
		// double thread_ratio = 1; //each worker gets assigned thread_ratio/num_workers() percent of depth
		// double avgSpan = (slope+1)/2*delta/num_workers()*thread_ratio;
		// double curSpan = 0;
		// std::vector<size_t> breakptrs;
		// breakptrs.push_back(0);
		// for(size_t i=1; i<=delta; i++){
		// 	curSpan += slope-(slope-1)/delta*i;
		// 	if(curSpan>=avgSpan*0.9){
		// 		curSpan = 0;
		// 		breakptrs.push_back(i);
		// 		std::cout<<"breakptr at "<<i<<std::endl;
		// 	}
		// }
		// if(breakptrs[breakptrs.size()-1]!=delta)
		// 	breakptrs.push_back(delta);
		//std::cout<<"delta "<<delta<<" size "<<breakptrs.size()<<std::endl;
		par_for(1, delta+1, 1, [&](size_t core){
			timer t_in; t_in.start();
			auto ret = PeelFixA(G, BetaMax, AlphaMax, core, bipartition, num_buckets);
			double preptime = std::get<1>(ret);
			timeA[core-1] = preptime;
			auto retA = std::get<0>(ret);
			msgA[core]=std::make_tuple(std::get<0>(retA),std::get<1>(retA),t_in.stop());
			tTimeA[core-1] = t_in.get_total();
		});

		par_for(1, delta+1, 1, [&](size_t core){
			timer t_in; t_in.start();
			auto ret = PeelFixB(G, BetaMax, AlphaMax, core, bipartition, num_buckets);
			double preptime = std::get<1>(ret);
			timeB[core-1] = preptime;
			auto retB = std::get<0>(ret);
			msgB[core]=std::make_tuple(std::get<0>(retB),std::get<1>(retB),t_in.stop());
			tTimeB[core-1] = t_in.get_total();
		});

		// par_for(1,breakptrs.size(),1,[&](size_t idx){
		// 	std::cout<<"running range "<<breakptrs[idx-1]+1<<" to "<<breakptrs[idx]<<std::endl;
		// 	timer t_in;
		// 	for(size_t core = breakptrs[idx-1]+1; core <= breakptrs[idx]; core++){
		// 		t_in.start();
		// 		auto ret = PeelFixA(G, BetaMax, AlphaMax, core, bipartition, num_buckets);
		// 		double preptime = std::get<1>(ret);
		// 		timeA[core-1] = preptime;
		// 		auto retA = std::get<0>(ret);
		// 		msgA[core]=std::make_tuple(std::get<0>(retA),std::get<1>(retA),t_in.stop());
		// 	}
		// 	t_in.reportTotal(std::string("range ")+std::to_string(breakptrs[idx-1]+1)+" to "+std::to_string(breakptrs[idx])+" runtime");
		// });

		// par_for(1,breakptrs.size(),1,[&](size_t idx){
		// 	std::cout<<"running range "<<breakptrs[idx-1]+1<<" to "<<breakptrs[idx]<<std::endl;
		// 	timer t_in;
		// 	for(size_t core = breakptrs[idx-1]+1; core <= breakptrs[idx]; core++){
		// 		t_in.start();
		// 		auto ret = PeelFixB(G, BetaMax, AlphaMax, core, bipartition, num_buckets);
		// 		double inittime = std::get<1>(ret);
		// 		timeB[core-1] = inittime;
		// 		auto retB = std::get<0>(ret);
		// 		msgB[core]=std::make_tuple(std::get<0>(retB),std::get<1>(retB),t_in.stop());
		// 	}
		// 	t_in.reportTotal(std::string("range ")+std::to_string(breakptrs[idx-1]+1)+" to "+std::to_string(breakptrs[idx])+" runtime");
		// });

		debug(for(size_t core=1; core<=delta; ++core) std::cout<<"coreA "<<core<<" "<<std::get<0>(msgA[core])<<" "<<std::get<1>(msgA[core])<<" "<<std::get<2>(msgA[core])<<'\n');
		debug(for(size_t core=1; core<=delta; ++core) std::cout<<"coreB "<<core<<" "<<std::get<0>(msgB[core])<<" "<<std::get<1>(msgB[core])<<" "<<std::get<2>(msgB[core])<<'\n');
		pbbslib::free_array(msgA);
		pbbslib::free_array(msgB);

		double peeltimeA = pbbslib::reduce_add(timeA);
		double peeltimeB = pbbslib::reduce_add(timeB);
		double totalTime = pbbslib::reduce_add(tTimeA) + pbbslib::reduce_add(tTimeB);
		debug(std::cout<< "peeltimeA: " << peeltimeA <<std::endl);
		debug(std::cout<< "peeltimeB: " << peeltimeB <<std::endl);
		debug(std::cout<< "totaltime: " << totalTime <<std::endl);
	}

	template <class Graph, class Apply>
	inline vertexSubsetData<uintE> nghCount(Graph &G, pbbslib::dyn_arr<uintE>& del, sequence<uintE>& D, size_t cutoff, Apply apply_f)
	{
		//everything less than cutoff is deleted
		std::unordered_set<uintE> eChange;
		for (uintE i = 0; i < del.size; i++){
			auto neighbors = G.get_vertex(del[i]).out_neighbors();
			// out_neighbors gives the id, degree, and neighbors in tuple form
			for (uintE j = 0; j < neighbors.degree; j++){
				uintE id = neighbors.get_neighbor(j);
				if(D[id]>=cutoff){
					eChange.insert(id);
					D[id]--;
				}
			}
		}
		pbbslib::dyn_arr<std::tuple<uintE,uintE> > changeArr(eChange.size());
		for(uintE id : eChange) {
			std::optional<std::tuple<uintE, uintE> > ret = apply_f(std::make_tuple(id, D[id]));
			if(ret) {changeArr.push_back(*apply_f(std::make_tuple(id, D[id])));}
		}
		return vertexSubsetData<uintE>(G.n, std::move(changeArr.to_seq()));
	}

	template <class Graph>
	inline pbbslib::dyn_arr<uintE> nghCount(Graph &G, pbbslib::dyn_arr<uintE>& del, sequence<uintE>& D, size_t cutoff){
		pbbslib::dyn_arr<uintE> delOther(del.size);
		for (uintE i = 0; i < del.size; i++){
			auto neighbors = G.get_vertex(del[i]).out_neighbors();
			for (uintE j = 0; j < neighbors.degree; j++){
				uintE id = neighbors.get_neighbor(j);
				if(D[id]>=cutoff){
					D[id]--;
					if(D[id]<cutoff) {delOther.resize(1); delOther.push_back(id);}
				}
			}
		}
		return delOther;
	}

	template <class Graph>
	inline std::pair<std::pair<size_t,size_t>,double> PeelFixA(Graph &G, sequence<sequence<size_t>> &BetaMax, 
	sequence<sequence<size_t>> &AlphaMax, size_t alpha,
	 size_t bipartition = 2, size_t num_buckets=16)
	{
		timer bt,ft,pt,it; // bt: begin time, ft: finish time, pt: processing time
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

		size_t initSize = pbbslib::reduce_add(sequence<uintE>(n_a, [&](size_t i) {return D[i]<alpha;}));

		pbbslib::dyn_arr<uintE> uDel(initSize);
		for(size_t i=0; i<n_a; i++)
			if(D[i]<alpha){uDel.push_back(i);}

		// instead of tracking whether a vertex is peeled or not using a boolean arr, we can just see whether its degree is above or below the cutoff
		// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
		ft.start();
		while (uDel.size>0){
			pbbslib::dyn_arr<uintE> vDel = nghCount(G, uDel, D, 1);
			uDel = nghCount(G, vDel, D, alpha);
		}
		ft.stop();

		pt.stop();

		auto vD =
			sequence<uintE>(n, [&](size_t i) {
				if (i <= bipartition || D[i] == 0)
					return std::numeric_limits<uintE>::max();
				return D[i];
			});

		it.start();
		auto bbuckets = make_vertex_buckets(n,vD,increasing,num_buckets);
		it.stop();
		// make num_buckets open buckets such that each vertex i is in D[i] bucket
		// note this i value is not real i value; realI = i+bipartition+1 or i+n_a
		size_t vCount = pbbslib::reduce_add(sequence<uintE>(n_b, [&](size_t i) {return D[i+n_a]>0;}));;
		//vCount = pbbslib::reduce_add(sequence<uintE>(n_b, [&](size_t i) {return D[i+n_a]>0;}));
		auto getVBuckets = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE> > {
			uintE v = std::get<0>(p), new_deg = std::get<1>(p);
			new_deg = std::max(new_deg, static_cast<uintE>(max_beta));
			D[v] = new_deg;
			uintE buck = bbuckets.get_bucket(new_deg);
			return wrap(v, buck);
		};

		auto updateBeta = [&](const uintE& u){
			pbbslib::write_max(&BetaMax[u][alpha],max_beta);
		};

		while (finished != vCount)
		{
			ft.start();
			bt.start();
			auto vbkt = bbuckets.next_bucket();
			bt.stop();
			max_beta = std::max(max_beta, vbkt.id);

			if (vbkt.id == 0)
				continue;
			pbbslib::dyn_arr<uintE> activeV(vbkt.identifiers.begin(), vbkt.identifiers.size(), vbkt.identifiers.size(), true);
			finished += activeV.size;
			for(uintE i=0; i<activeV.size; i++){
				size_t index = activeV[i]-n_a;
				for(uintE j=1; j<max_beta; j++)
					pbbslib::write_max(&AlphaMax[index][j],alpha);
			}
			// par_for(0, activeV.size, [&](size_t i) {
			// 	size_t index = activeV[i]-n_a;
			// 	par_for(1, max_beta, [&](size_t j) {
			// 		pbbslib::write_max(&AlphaMax[index][j],alpha);
			// 	});
			// });
			pbbslib::dyn_arr<uintE> deleteU = nghCount(G, activeV, D, alpha);
			for(size_t i=0; i<deleteU.size; i++) updateBeta(deleteU[i]);
			// "deleteU" is a wrapper storing a sequence id of deleted vertices in U
			vertexSubsetData<uintE> movedV = nghCount(G, deleteU, D, max_beta+1, getVBuckets);
			// "movedV" is a wrapper storing a sequence of tuples like (id, newBucket)
			ft.stop();
			bt.start();
			bbuckets.update_buckets(movedV);
			bt.stop();
			rho_alpha++;
		}
		it.start();
		bbuckets.del();
		it.stop();
		debug(pt.reportTotal("prep time"));
		debug(ft.reportTotal("nghCount time"));
		debug(bt.reportTotal("bucket time"));
		return std::make_pair(std::pair<size_t,size_t>(rho_alpha,max_beta),ft.get_total());
	}

	template <class Graph>
	inline std::pair<std::pair<size_t,size_t>,double> PeelFixB(Graph &G, sequence<sequence<size_t>> &BetaMax, 
	sequence<sequence<size_t>> &AlphaMax, size_t beta,
	size_t bipartition = 2, size_t num_buckets=16)
	{
		timer bt,ft,pt,it;
		pt.start();
		const size_t n = G.n;
		const size_t n_b = n - bipartition - 1;
		const size_t n_a = bipartition + 1;

		size_t finished = 0, rho_beta = 0, max_alpha = 0;
		auto D =
			sequence<uintE>(n, [&](size_t i) {
				return G.get_vertex(i).out_degree();
			});

		size_t initSize = pbbslib::reduce_add(sequence<uintE>(n_b, [&](size_t i) {return D[i+n_a]<beta;}));
		pbbslib::dyn_arr<uintE> vDel(initSize);
		for(size_t i=n_a; i<n; i++)
			if(D[i]<beta){ vDel.push_back(i); }

		// auto cond_fv = [&D, &beta](const uintE &v) { return D[v] >= beta; };
		// auto cond_fu = [&D, &max_alpha](const uintE &u) { return D[u] > max_alpha; };

		// // if the U list is empty
		// auto clearZeroU = [&](const std::tuple<uintE, uintE> &p)
		// 	-> const std::optional<std::tuple<uintE, uintE>> {
		// 	uintE u = std::get<0>(p), edgesRemoved = std::get<1>(p);
		// 	uintE new_deg = D[u] - edgesRemoved;
		// 	D[u] = new_deg;
		// 	if (new_deg == 0)
		// 		return wrap(u, 0);
		// 	return std::nullopt;
		// };

		// auto clearV = [&](const std::tuple<uintE, uintE> &p)
		// 	-> const std::optional<std::tuple<uintE, uintE> > {
		// 	uintE v = std::get<0>(p), edgesRemoved = std::get<1>(p);
		// 	uintE new_deg = D[v] - edgesRemoved;
		// 	D[v] = new_deg;
		// 	if (new_deg < beta)
		// 	{
		// 		if(max_alpha>0)
		// 			pbbslib::write_max(&AlphaMax[v-n_a][beta],max_alpha);
		// 		return wrap(v, 0);
		// 	}
		// 	return std::nullopt;
		// };

		// nghCount counts the # of neighbors
		ft.start();
		while (vDel.size>0){
			pbbslib::dyn_arr<uintE> uDel = nghCount(G, vDel, D, 1);
			vDel = nghCount(G, uDel, D, beta);
		}
		ft.stop();
		pt.stop();

		size_t uCount = pbbslib::reduce_add(sequence<uintE>(n_a, [&](size_t i) {return D[i]>0;}));

		auto Du =
			sequence<uintE>(n, [&](size_t i) {
				if (i > bipartition || D[i] == 0)
					return std::numeric_limits<uintE>::max();
				return D[i];
			});
		it.start();
		auto abuckets = make_vertex_buckets(n,Du,increasing,num_buckets);
		it.stop();
		// makes num_buckets open buckets
		// for each vertex [0, n_a-1], it puts it in bucket D[i]
		auto getUBuckets = [&](const std::tuple<uintE, uintE> &p)
			-> const std::optional<std::tuple<uintE, uintE>> {
			uintE u = std::get<0>(p), new_deg = std::get<1>(p);
			new_deg = std::max(new_deg, static_cast<uintE>(max_alpha));
			D[u] = new_deg;
			return wrap(u, abuckets.get_bucket(new_deg));
		};

		auto updateAlpha = [&](const uintE& v){
			pbbslib::write_max(&AlphaMax[v-n_a][beta],max_alpha);
		};

		while (finished != uCount)
		{
			ft.start();
			bt.start();
			auto ubkt = abuckets.next_bucket();
			bt.stop();
			max_alpha = std::max(max_alpha, ubkt.id);

			if (ubkt.id == 0)
				continue;
			pbbslib::dyn_arr<uintE> activeU(ubkt.identifiers.begin(), ubkt.identifiers.size(), ubkt.identifiers.size(), true);
			finished += activeU.size; // add to finished set
			for(uintE i=0; i<activeU.size; i++){
				size_t index = activeU[i];
				for(uintE j=1; j<max_alpha; j++)
					pbbslib::write_max(&BetaMax[index][j],beta);
			}
			// par_for(0, activeU.size, [&](size_t i) {
			// 	size_t index = activeU[i];
			// 	par_for(1, max_alpha, [&](size_t j) {
			// 		pbbslib::write_max(&BetaMax[index][j],beta);
			// 	});
			// });
			
			pbbslib::dyn_arr<uintE> deleteV = nghCount(G, activeU, D, beta);
			for(size_t i=0; i<deleteV.size; i++) updateAlpha(deleteV[i]);
			// "deleteV" is a wrapper storing a sequence id of deleted vertices in V
			vertexSubsetData<uintE> movedU = nghCount(G, deleteV, D, max_alpha+1, getUBuckets);

			ft.stop();
			bt.start();
			abuckets.update_buckets(movedU);
			bt.stop();
			rho_beta++;
		}
		it.start();
		abuckets.del();
		//em.del();
		it.stop();
		debug(pt.reportTotal("prep time"));
		return std::make_pair(std::pair<size_t,size_t>(rho_beta,max_alpha),ft.get_total());
	}

} // namespace gbbs
