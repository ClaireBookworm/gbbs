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
#include <chrono>
#include <thread>
#include <stdlib.h>


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
		// max unicor 

		auto msgA = pbbslib::new_array_no_init<std::tuple<size_t,size_t,float_t> >(delta+1);
		auto msgB = pbbslib::new_array_no_init<std::tuple<size_t,size_t,float_t> >(delta+1);
		auto timeA = sequence<double>(delta, 0.0);
		auto timeB = sequence<double>(delta, 0.0);

		double slope = 1.5;
		double thread_ratio = 1; //each worker gets assigned thread_ratio/num_workers() percent of depth
		double avgSpan = (slope+1)/2*delta/num_workers()*thread_ratio;
		double curSpan = 0;
		std::vector<size_t> breakptrs;
		breakptrs.push_back(0);
		for(size_t i=1; i<=delta; i++){
			curSpan += slope-(slope-1)/delta*i;
			if(curSpan>=avgSpan*1){
				curSpan = 0;
				breakptrs.push_back(i);
				std::cout<<"breakptr at "<<i<<std::endl;
			}
		}
		if(breakptrs[breakptrs.size()-1]!=delta)
			breakptrs.push_back(delta);
		std::cout<<"delta "<<delta<<" size "<<breakptrs.size()<<std::endl;

		auto tTime = sequence<double>(breakptrs.size(), 0.0);
		auto pTime = sequence<double>(breakptrs.size(), 0.0);

		par_for(1,breakptrs.size(),1,[&](size_t idx){
			std::cout<<"running range "<<breakptrs[idx-1]+1<<" to "<<breakptrs[idx]<<std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			timer t_in, p_t; 
			t_in.start(); p_t.start();
			sequence<uintE> degA = sequence<uintE>(n, [&](size_t i) {
				return G.get_vertex(i).out_degree();
			});
			sequence<uintE> degB = sequence<uintE>(n, [&](size_t i) {
				return G.get_vertex(i).out_degree();
			});
			//uintE minCore = breakptrs[idx-1]+1;

			// size_t InitSize = pbbslib::reduce_add(sequence<uintE>(n_a, [&](size_t i) {return degA[i]<minCore;}));
			// pbbslib::dyn_arr<uintE> DelA(InitSize);
			// for(size_t i=0; i<n_a; i++) if(degA[i]<minCore) DelA.push_back(i);
			// initialClean(G, degA, DelA, minCore);

			// InitSize = pbbslib::reduce_add(sequence<uintE>(n_b, [&](size_t i) {return degB[i+n_a]<minCore;}));
			// pbbslib::dyn_arr<uintE> DelB(InitSize);
			// for(size_t i=n_a; i<n; i++) if(degB[i]<minCore) DelB.push_back(i);
			// initialClean(G, degB, DelB, minCore);
			uintE minCore = -10000000;
			p_t.stop();

			auto peelAllFixA = [&](){
			par_for(breakptrs[idx-1]+1, breakptrs[idx]+1, 1, [&](size_t core){
				sequence<uintE> D = sequence<uintE>(n, [&](size_t i) {
					return G.get_vertex(i).out_degree();
				});
				size_t initSize = pbbslib::reduce_add(sequence<uintE>(n_a, [&](size_t i) {return (D[i]<core) & (D[i]>=minCore);}));
				pbbslib::dyn_arr<uintE> delA(initSize);
				for(size_t i=0; i<n_a; i++) if((D[i]<core) && (D[i]>=minCore)){ delA.push_back(i); }
				initialClean(G, D, delA, core);

				auto ret = PeelFixA(G, BetaMax, AlphaMax, D, core, bipartition, num_buckets);
				double preptime = std::get<1>(ret);
				timeA[core-1] = preptime;
				auto retA = std::get<0>(ret);
				msgA[core]=std::make_tuple(std::get<0>(retA),std::get<1>(retA),0);
			});};
			auto peelAllFixB = [&](){
			par_for(breakptrs[idx-1]+1, breakptrs[idx]+1, 1, [&](size_t core){
				sequence<uintE> D = sequence<uintE>(n, [&](size_t i) {
					return G.get_vertex(i).out_degree();
				});
				size_t initSize = pbbslib::reduce_add(sequence<uintE>(n_b, [&](size_t i) {return (D[i+n_a]<core) & (D[i+n_a]>=minCore);}));
				pbbslib::dyn_arr<uintE> delB(initSize);
				for(size_t i=n_a; i<n; i++) if((D[i]<core) && (D[i]>=minCore)){ delB.push_back(i); }
				initialClean(G, D, delB, core);

				auto ret = PeelFixB(G, BetaMax, AlphaMax, D, core, bipartition, num_buckets);
				double inittime = std::get<1>(ret);
				timeB[core-1] = inittime;
				auto retB = std::get<0>(ret);
				msgB[core]=std::make_tuple(std::get<0>(retB),std::get<1>(retB),0);
			});};
			par_do(peelAllFixA,peelAllFixB);
			t_in.stop();
			tTime[idx] = t_in.get_total();
			pTime[idx] = p_t.get_total();
			std::cout<<"range "<<breakptrs[idx-1]+1<<" to "<<breakptrs[idx]<<" finished"<<std::endl;
		});

		// remember to aggregate AlphaMax, BetaMax

		debug(for(size_t core=1; core<=delta; ++core) std::cout<<"coreA "<<core<<" "<<std::get<0>(msgA[core])<<" "<<std::get<1>(msgA[core])<<" "<<std::get<2>(msgA[core])<<'\n');
		debug(for(size_t core=1; core<=delta; ++core) std::cout<<"coreB "<<core<<" "<<std::get<0>(msgB[core])<<" "<<std::get<1>(msgB[core])<<" "<<std::get<2>(msgB[core])<<'\n');
		pbbslib::free_array(msgA);
		pbbslib::free_array(msgB);

		double peeltimeA = pbbslib::reduce_add(timeA);
		double peeltimeB = pbbslib::reduce_add(timeB);
		double totalTime = pbbslib::reduce_add(tTime);
		double prepTime = pbbslib::reduce_add(pTime);
		debug(std::cout<< "peeltimeA: " << peeltimeA <<std::endl);
		debug(std::cout<< "peeltimeB: " << peeltimeB <<std::endl);
		debug(std::cout<< "initPeelTime: " << prepTime <<std::endl);
		debug(std::cout<< "totaltime: " << totalTime <<std::endl);
	}

	template <class Graph, class Apply>
	inline vertexSubsetData<uintE> nghCount(Graph &G, pbbslib::dyn_arr<uintE>& del, sequence<uintE>& D, size_t cutoff, Apply apply_f, size_t iter_id, uintE* peelAt)
	{
		//everything less than cutoff is deleted
		std::vector<uintE> eChange;
		for (uintE i = 0; i < del.size; i++){
			auto neighbors = G.get_vertex(del[i]).out_neighbors();
			uintE deg = neighbors.degree;
			// out_neighbors gives the id, degree, and neighbors in tuple form
			for (uintE j = 0; j < deg; j++){
				uintE id = neighbors.get_neighbor(j);
				if(D[id]>=cutoff){
					if(peelAt[id] < iter_id){
						eChange.push_back(id);
						peelAt[id] = iter_id;
					}
					D[id]--;
				}
			}
		}
		pbbslib::dyn_arr<std::tuple<uintE,uintE> > changeArr(eChange.size());
		for(uintE& id : eChange) {
			std::optional<std::tuple<uintE, uintE> > ret = apply_f(std::make_tuple(id, D[id]));
			if(ret) changeArr.push_back(*ret);
		}
		return vertexSubsetData<uintE>(G.n, std::move(changeArr.to_seq()));
	}

	template <class Graph>
	inline pbbslib::dyn_arr<uintE> nghCount(Graph &G, pbbslib::dyn_arr<uintE>& del, sequence<uintE>& D, size_t cutoff){
		pbbslib::dyn_arr<uintE> delOther(16);
		for (uintE i = 0; i < del.size; i++){
			auto neighbors = G.get_vertex(del[i]).out_neighbors();
			uintE deg = neighbors.degree;
			for (uintE j=0; j<deg; j++){
				uintE id = neighbors.get_neighbor(j);
				if(D[id]==cutoff){
					delOther.resize(1);
					delOther.push_back(id);
				}
				D[id]--;
			}
		}
		return delOther;
	}

	template <class Graph>
	inline void initialClean(Graph &G, sequence<uintE>& D, pbbslib::dyn_arr<uintE>& del, uintE cutoff){
		while (del.size>0){
			pbbslib::dyn_arr<uintE> oDel = nghCount(G, del, D, 1);
			del = nghCount(G, oDel, D, cutoff);
		}
	}

	template <class Graph>
	inline std::pair<std::pair<size_t,size_t>,double> PeelFixA(Graph &G, sequence<sequence<size_t>> &BetaMax, 
	sequence<sequence<size_t>> &AlphaMax, sequence<uintE>& D, size_t alpha,
	 size_t bipartition = 2, size_t num_buckets=16)
	{
		timer bt,ft,pt,it; // bt: begin time, ft: finish time, pt: processing time
		pt.start();

		const size_t n = G.n;
		const size_t n_b = n - bipartition - 1;
		const size_t n_a = bipartition + 1;

		size_t finished = 0, rho_alpha = 0, max_beta = 0;
		// [0, bipartition] interval for U
		// // [bipartition+1, n-1]  interval V
		// auto D =
		// 	sequence<uintE>(n, [&](size_t i) {
		// 		return G.get_vertex(i).out_degree();
		// 	});

		// size_t initSize = pbbslib::reduce_add(sequence<uintE>(n_a, [&](size_t i) {return D[i]<alpha;}));

		// pbbslib::dyn_arr<uintE> uDel(initSize);
		// for(size_t i=0; i<n_a; i++)
		// 	if(D[i]<alpha){uDel.push_back(i);}

		// // instead of tracking whether a vertex is peeled or not using a boolean arr, we can just see whether its degree is above or below the cutoff
		// // peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
		// //ft.start();
		// while (uDel.size>0){
		// 	pbbslib::dyn_arr<uintE> vDel = nghCount(G, uDel, D, 1);
		// 	uDel = nghCount(G, vDel, D, alpha);
		// }
		// //ft.stop();

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

		uintE* peelAt = (uintE*)std::calloc(G.n, sizeof(uintE));
		size_t iter_id = 0;

		while (finished != vCount)
		{
			bt.start();
			auto vbkt = bbuckets.next_bucket();
			bt.stop();
			ft.start();
			max_beta = std::max(max_beta, vbkt.id);

			if (vbkt.id == 0)
				continue;
			pbbslib::dyn_arr<uintE> activeV(vbkt.identifiers.begin(), vbkt.identifiers.size(), vbkt.identifiers.size(), true);
			finished += activeV.size;
			for(uintE i=0; i<activeV.size; i++)
				pbbslib::write_max(&AlphaMax[activeV[i]-n_a][max_beta-1],alpha);
			// par_for(0, activeV.size, [&](size_t i) {
			// 	size_t index = activeV[i]-n_a;
			// 	par_for(1, max_beta, [&](size_t j) {
			// 		pbbslib::write_max(&AlphaMax[index][j],alpha);
			// 	});
			// });

			// try graph pruning
			// try openmp
			pbbslib::dyn_arr<uintE> deleteU = nghCount(G, activeV, D, alpha);
			for(size_t i=0; i<deleteU.size; i++) updateBeta(deleteU[i]);
			// "deleteU" is a wrapper storing a sequence id of deleted vertices in U
			vertexSubsetData<uintE> movedV = nghCount(G, deleteU, D, max_beta+1, getVBuckets, ++iter_id, peelAt);
			// "movedV" is a wrapper storing a sequence of tuples like (id, newBucket)
			bt.start();
			bbuckets.update_buckets(movedV);
			bt.stop();
			ft.stop();
			rho_alpha++;
		}
		it.start();
		bbuckets.del();
		it.stop();
		delete[] peelAt;
		debug(pt.reportTotal("prep time"));
		debug(ft.reportTotal("nghCount time"));
		debug(bt.reportTotal("bucket time"));
		return std::make_pair(std::pair<size_t,size_t>(rho_alpha,max_beta),ft.get_total());
	}

	template <class Graph>
	inline std::pair<std::pair<size_t,size_t>,double> PeelFixB(Graph &G, sequence<sequence<size_t>> &BetaMax, 
	sequence<sequence<size_t>> &AlphaMax, sequence<uintE>& D, size_t beta,
	size_t bipartition = 2, size_t num_buckets=16)
	{
		timer bt,ft,pt,it;
		pt.start();
		const size_t n = G.n;
		const size_t n_b = n - bipartition - 1;
		const size_t n_a = bipartition + 1;

		size_t finished = 0, rho_beta = 0, max_alpha = 0;
		// auto D =
		// 	sequence<uintE>(n, [&](size_t i) {
		// 		return G.get_vertex(i).out_degree();
		// 	});

		// size_t initSize = pbbslib::reduce_add(sequence<uintE>(n_b, [&](size_t i) {return D[i+n_a]<beta;}));
		// pbbslib::dyn_arr<uintE> vDel(initSize);
		// for(size_t i=n_a; i<n; i++)
		// 	if(D[i]<beta){ vDel.push_back(i); }


		// // nghCount counts the # of neighbors
		// //ft.start();
		// while (vDel.size>0){
		// 	pbbslib::dyn_arr<uintE> uDel = nghCount(G, vDel, D, 1);
		// 	vDel = nghCount(G, uDel, D, beta);
		// }
		//ft.stop();
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

		uintE* peelAt = (uintE*)std::calloc(G.n, sizeof(uintE));
		size_t iter_id = 0;

		while (finished != uCount)
		{
			bt.start();
			auto ubkt = abuckets.next_bucket();
			bt.stop();
			ft.start();
			max_alpha = std::max(max_alpha, ubkt.id);

			if (ubkt.id == 0)
				continue;
			pbbslib::dyn_arr<uintE> activeU(ubkt.identifiers.begin(), ubkt.identifiers.size(), ubkt.identifiers.size(), true);
			finished += activeU.size; // add to finished set
			for(uintE i=0; i<activeU.size; i++)
				pbbslib::write_max(&BetaMax[activeU[i]][max_alpha-1],beta);
				// use prefix max later for j<max_alpha-1
			// par_for(0, activeU.size, [&](size_t i) {
			// 	size_t index = activeU[i];
			// 	par_for(1, max_alpha, [&](size_t j) {
			// 		pbbslib::write_max(&BetaMax[index][j],beta);
			// 	});
			// });
			pbbslib::dyn_arr<uintE> deleteV = nghCount(G, activeU, D, beta);
			for(size_t i=0; i<deleteV.size; i++) updateAlpha(deleteV[i]);
			// "deleteV" is a wrapper storing a sequence id of deleted vertices in V
			vertexSubsetData<uintE> movedU = nghCount(G, deleteV, D, max_alpha+1, getUBuckets, ++iter_id, peelAt);
			bt.start();
			abuckets.update_buckets(movedU);
			bt.stop();
			ft.stop();
			rho_beta++;
		}
		it.start();
		abuckets.del();
		delete[] peelAt;
		//em.del();
		it.stop();
		debug(pt.reportTotal("prep time"));
		return std::make_pair(std::pair<size_t,size_t>(rho_beta,max_alpha),ft.get_total());
	}

} // namespace gbbs
