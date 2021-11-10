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
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_set>

namespace gbbs{
// use max alpha and beta
template <class Graph>
inline std::pair<double, double> PeelFixA(Graph& G, sequence<uintE>& D, uintE alpha, size_t n_a, size_t n_b);
template <class Graph>
inline std::pair<double, double> PeelFixB(Graph& G, sequence<uintE>& D, uintE beta, size_t n_a, size_t n_b);

template <class Graph>
inline void initialClean(Graph &G, sequence<uintE>& D, std::vector<uintE>& del, uintE cutoff){
	while (!del.empty()){
		std::vector<uintE> newDel;
		for(uintE vi : del){
			auto neighborsVi = G.get_vertex(vi).out_neighbors();
			for(uintE i = 0; i<neighborsVi.degree; i++){
				uintE oi = neighborsVi.get_neighbor(i);
				if(D[oi]-- == cutoff) newDel.push_back(oi);
			}
		}
		del = std::move(newDel);
	}
}

template <class Graph>
inline void BiCore(Graph &G, size_t num_buckets = 16, size_t bipartition = 2, uintE peel_core_alpha = 0, uintE peel_core_beta = 0)
{
	std::cout << "starting" << std::endl;
	timer it; it.start();
	const size_t n = G.n;					// # of vertices
	const size_t n_a = bipartition + 1;		// number of vertices in first partition
	const size_t n_b = n - bipartition - 1; // number of vertices in second partition
	auto ret = KCore(G, num_buckets);
	const uintE delta = static_cast<uintE>(pbbslib::reduce_max(std::get<0>(ret)));
	// ret stores the largest core value for which each vertex exists
	// if we peel away (c,c)-core, we want to get rid of all vertices <= c
	// 
	sequence<uintT> edgeCount = std::move(std::get<1>(ret));
	std::vector<sequence<uintE> > prepeel = std::move(std::get<2>(ret));
	sequence<uintE> copyIdx(prepeel.size());
	size_t lastAvail = 0;
	for(size_t i=1; i<=delta; i++){
		if(i < prepeel.size() && prepeel[i].size() > 0) lastAvail = i;
		copyIdx[i] = lastAvail;
	}

	par_for(1,delta+1,1,[&](size_t idx){
		if(idx != copyIdx[idx]) prepeel[idx] = prepeel[copyIdx[idx]];
	});

	it.stop();
	std::cout<<"delta "<<delta<<" prepeel size "<<prepeel.size()<<" k-peel time "<<it.get_total()<<std::endl;
	it.start();

	sequence<double> workC = sequence<double>(delta, [&](size_t idx){
		return (double)(edgeCount[idx+1])/G.m;
	});

	for(uintE i=0; i<delta-1; i++) std::cout<<workC[i]<<" ";
	std::cout<<workC[delta-1]<<std::endl;

	std::vector<size_t> breakptrs;
	breakptrs.push_back(0);

	double thread_ratio = 1.0; //try messing with this
	double avgWork = pbbslib::reduce_add(workC) / num_workers() * thread_ratio;
	double curWork = 0.0;
	for(size_t i=1; i<=delta; i++){
		breakptrs.push_back(i);
		// curWork += workC[i-1];
		// if(curWork/avgWork >= 1.05){
		// 	curWork = 0.0;
		// 	breakptrs.push_back(i);
		// 	std::cout<<"breakptr at "<<i<<std::endl;
		// }
	}
	if(breakptrs[breakptrs.size()-1]!=delta)
		breakptrs.push_back(delta);

	std::cout<<"delta "<<delta<<" size "<<breakptrs.size()<<" k-peel time "<<it.get_total()<<std::endl;
	auto timeA = sequence<double>(delta, 0.0);
	auto timeB = sequence<double>(delta, 0.0);
	auto tTime = sequence<double>(delta, 0.0);
	// sequence<uintE>* prepeelA = new sequence<uintE>[breakptrs.size()];
	// par_for(1,breakptrs.size(),1,[&](size_t idx){
	// 	uintE minCore = breakptrs[idx-1]+1;
	// 	sequence<uintE> degA(n, [&](size_t i) { return G.get_vertex(i).out_degree(); });
	// 	size_t initSize = pbbslib::reduce_add(sequence<uintE>(n, [&](size_t i) {return degA[i]<minCore;}));
	// 	std::vector<uintE> DelA(initSize);
	// 	for(size_t i=0, j=0; i<n; i++) if(degA[i]<minCore) DelA[j++]=i;
	// 	initialClean(G, degA, DelA, minCore);
	// 	prepeelA[idx] = std::move(degA);
	// });

	// sequence<uintE>* prepeelB = new sequence<uintE>[breakptrs.size()];
	// par_for(1,breakptrs.size(),1,[&](size_t idx){
	// 	uintE minCore = breakptrs[idx-1]+1;
	// 	sequence<uintE> degB(n, [&](size_t i) { return G.get_vertex(i).out_degree(); });
	// 	size_t initSize = pbbslib::reduce_add(sequence<uintE>(n, [&](size_t i) {return degB[i]<minCore;}));
	// 	std::vector<uintE> DelB(initSize);
	// 	for(size_t i=0, j=0; i<n; i++) if(degB[i]<minCore) DelB[j++]=i;
	// 	initialClean(G, degB, DelB, minCore);
	// 	prepeelB[idx] = std::move(degB);
	// });
	it.stop();
	par_for(1,breakptrs.size(),1,[&](size_t idx){
		std::cout<<"running range "<<breakptrs[idx-1]+1<<" to "<<breakptrs[idx]<<std::endl;
		uintE minCore = breakptrs[idx-1]+1;
		for(uintE core = breakptrs[idx-1]+1; core <= breakptrs[idx]; core++){
			timer t_in; t_in.start();
			sequence<uintE> D = prepeel[minCore];
			// use delayed_sequence here
			// start serial, improve with parallelism
			size_t initSize = 0;
			for(uintE i=0; i<n; i++) if(D[i]<core && D[i]>=minCore) initSize++;
			std::vector<uintE> delA(initSize);
			for(size_t i = 0, j = 0; i<n; i++) if((D[i]<core) && (D[i]>=minCore)) delA[j++]=i; 
			initialClean(G, D, delA, core);
			auto ret = PeelFixA(G, D, core, n_a, n_b, num_buckets);
			timeA[core-1] = std::get<1>(ret);
			t_in.stop();
			tTime[core-1] += t_in.get_total();
		}
		for(uintE core = breakptrs[idx-1]+1; core <= breakptrs[idx]; core++){
			timer t_in; t_in.start();
			sequence<uintE> D = prepeel[minCore];
			size_t initSize = 0;
			for(uintE i=0; i<n; i++) if(D[i]<core && D[i]>=minCore) initSize++;
			std::vector<uintE> delB(initSize);
			for(size_t i = 0, j = 0; i<n; i++) if((D[i]<core) && (D[i]>=minCore)) delB[j++] = i;
			initialClean(G, D, delB, core);
			auto ret = PeelFixB(G, D, core, n_a, n_b, num_buckets);
			timeB[core-1] = std::get<1>(ret);
			t_in.stop();
			tTime[core-1] += t_in.get_total();
		}
		std::cout<<"range "<<breakptrs[idx-1]+1<<" to "<<breakptrs[idx]<<" finished"<<std::endl;
	});
	it.reportTotal("initialize time");
}

template <class Graph>
inline std::pair<double, double> PeelFixA(Graph& G, sequence<uintE>& D, uintE alpha, size_t n_a, size_t n_b, size_t num_buckets)
{
	const size_t n = n_a + n_b;
	timer pqt, pt;
	uintE rho_alpha = 0, max_beta = 0;

	pqt.start();
	auto Dv = sequence<uintE>::no_init(n);
	std::fill(Dv.begin(), Dv.end(), std::numeric_limits<uintE>::max());
	for(uintE i=n_a; i<n; i++) if(D[i]>=alpha) Dv[i] = D[i];

	auto bbuckets = make_vertex_buckets(n,Dv,increasing,num_buckets);
	auto getVBuckets = [&](const uintE& vtx, const uintE& deg)
		-> const std::optional<std::tuple<uintE, uintE> > {
		return wrap(vtx, bbuckets.get_bucket(deg));
	};
	pqt.stop();
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0);
	std::vector<uintE> changeVtx; // allocated outside of loop
	// use a counter to track the number of changes and then use that to determine when to stop
	// don't .clear() the changeVtx vector, just reuse it
	uintE finished = 0;

	uintE vCount = 0;
	for(uintE i=n_a; i<n; i++) vCount+=D[i]>=alpha;
	while (finished != vCount)
	{
		iter++;
		pqt.start();
		auto bkt = bbuckets.next_bucket();
		max_beta = std::max((uintE)bkt.id, max_beta);
		auto bktArr = bkt.identifiers;
		finished += bktArr.size();
		pqt.stop();
		for(uintE vi : bktArr){
			auto neighborsVi = G.get_vertex(vi).out_neighbors();
			for(uintE i = 0; i<neighborsVi.degree; i++){
				uintE ui = neighborsVi.get_neighbor(i);
				if(D[ui]-- == alpha){
					auto neighborsUi = G.get_vertex(ui).out_neighbors();
					for(uintE j = 0; j<neighborsUi.degree; j++){
						uintE vii = neighborsUi.get_neighbor(j); 
						if(D[vii] > max_beta){
							if(tracker[vii]!=iter){
								changeVtx.push_back(vii);
								tracker[vii] = iter;
							}
							D[vii]--;
						}
					}
				}
			}
		}
		pqt.start();
		pbbslib::dyn_arr<std::tuple<uintE, uintE> > moveV(changeVtx.size()/16);
		for(uintE vii : changeVtx){
			uintE deg = std::max(max_beta, D[vii]);
			Dv[vii] = deg; D[vii] = deg;
			auto ret = getVBuckets(vii, deg);
			if(ret) { moveV.resize(1); moveV.push_back(*ret); }
		}
		auto moveVBucket = vertexSubsetData<uintE>(n, moveV.to_seq());
		// sequence max size and then resize later 

		bbuckets.update_buckets(moveVBucket);
		changeVtx.clear();
		pqt.stop();
		rho_alpha++;
	}
	std::cout<<"Alpha "<<alpha<<" "<<rho_alpha <<" "<<max_beta<<std::endl;
	return std::make_pair(pqt.get_total(), pt.get_total());
}

template <class Graph>
inline std::pair<double, double> PeelFixB(Graph& G, sequence<uintE>& D, uintE beta, size_t n_a, size_t n_b, size_t num_buckets)
{
	const size_t n = n_a + n_b;
	timer pqt, pt;
	uintE rho_beta = 0, max_alpha = 0;

	pqt.start();
	auto Du = sequence<uintE>::no_init(n);
	std::fill(Du.begin(), Du.end(), std::numeric_limits<uintE>::max());
	for(uintE i=0; i<n_a; i++) if(D[i]>=beta) Du[i]=D[i];

	auto abuckets = make_vertex_buckets(n,Du,increasing,num_buckets);
	auto getUBuckets = [&](const uintE& vtx, const uintE& deg)
		-> const std::optional<std::tuple<uintE, uintE> > {
		return wrap(vtx, abuckets.get_bucket(deg));
	};
	pqt.stop();
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0); // tracks last time the degree changed
	std::vector<uintE> changeVtx;
	uintE finished = 0;
	uintE uCount = 0;
	for(uintE i=0; i<n_a; i++) uCount += D[i]>=beta;
	while (finished != uCount)
	{
		iter++;
		pqt.start();
		auto bkt = abuckets.next_bucket();
		max_alpha = std::max((uintE)bkt.id, max_alpha);
		auto bktArr = bkt.identifiers;
		finished += bktArr.size();
		pqt.stop();
		for(uintE ui : bktArr){
			auto neighborsUi = G.get_vertex(ui).out_neighbors();
			for(uintE i = 0; i<neighborsUi.degree; i++){
				uintE vi = neighborsUi.get_neighbor(i);
				if(D[vi]-- == beta){
					auto neighborsVi = G.get_vertex(vi).out_neighbors();
					for(uintE j = 0; j<neighborsVi.degree; j++){
						uintE uii = neighborsVi.get_neighbor(j); 
						if(D[uii] > max_alpha){
							if(tracker[uii]!=iter){
								changeVtx.push_back(uii);
								tracker[uii] = iter;
							}
							D[uii]--;
						}
					}
				}
			}
		}
		pqt.start();
		pbbslib::dyn_arr<std::tuple<uintE, uintE> > moveU(changeVtx.size()/16);//try changeVtx.size, try others
		for(uintE uii : changeVtx){ // could be paralleliezd
			uintE deg = std::max(max_alpha, D[uii]);
			Du[uii] = deg; D[uii] = deg;
			auto ret = getUBuckets(uii, deg);
			if(ret) { moveU.resize(1); moveU.push_back(*ret); }
		}
		auto moveUBucket = vertexSubsetData<uintE>(n, moveU.to_seq());
		abuckets.update_buckets(moveUBucket);
		changeVtx.clear();
		pqt.stop();
		rho_beta++;
	}
	std::cout<<"Beta "<<beta<<" "<<rho_beta <<" "<<max_alpha<<std::endl;
	return std::make_pair(pqt.get_total(), pt.get_total());
}

}

