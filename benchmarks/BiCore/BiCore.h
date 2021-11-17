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
inline std::pair<double, double> PeelFixA(Graph& G, sequence<uintE> D, uintE alpha, size_t n_a, size_t n_b);
template <class Graph>
inline std::pair<double, double> PeelFixB(Graph& G, sequence<uintE> D, uintE beta, size_t n_a, size_t n_b);

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

	auto timeA = sequence<double>(delta, 0.0);
	auto timeB = sequence<double>(delta, 0.0);

	auto peelA = [&](){
		par_for(1,delta+1,1,[&](size_t core){
			timer t_in; t_in.start();
			// use delayed_sequence here
			// start serial, improve with parallelism
			auto ret = PeelFixA(G, prepeel[core], core, n_a, n_b, num_buckets);
			t_in.stop();
			timeA[core-1] += t_in.get_total();
		});
	};

	auto peelB = [&](){
		par_for(1,delta+1,1,[&](size_t core){
			timer t_in; t_in.start(); 
			auto ret = PeelFixB(G, prepeel[core], core, n_a, n_b, num_buckets);
			t_in.stop();
			timeB[core-1] = t_in.get_total();
		});
	};

	par_do(peelA, peelB);
	//peelA();
	//peelB();
	double totalRuntime = 0;
	for(uintE i = 1; i <= delta; i++){
		std::cout<<"coreA "<<i<<" running time: "<<timeA[i-1]<<std::endl;
		totalRuntime += timeA[i-1];
	}
	for(uintE i = 1; i <= delta; i++){
		std::cout<<"coreB "<<i<<" running time: "<<timeB[i-1]<<std::endl;
		totalRuntime += timeB[i-1];
	}
	std::cout<<"ideal runtime with "<<num_workers()<<" threads: "<<totalRuntime/num_workers()<<std::endl;
	it.reportTotal("initialize time");
}

template <class Graph>
inline std::pair<double, double> PeelFixA(Graph& G, sequence<uintE> D, uintE alpha, size_t n_a, size_t n_b, size_t num_buckets)
{
	// allocation could bottleneck
	const size_t n = n_a + n_b;
	uintE rho_alpha = 0, max_beta = 0;

	auto Dv = sequence<uintE>(n, std::numeric_limits<uintE>::max(), false);
	for(uintE i=n_a; i<n; i++) if(D[i]>=alpha) Dv[i] = D[i];

	auto bbuckets = make_vertex_buckets(n,Dv,increasing,num_buckets,true); //maybe try sequentialize it
	auto getVBuckets = [&](const uintE& vtx, const uintE& deg)
		-> const std::optional<std::tuple<uintE, uintE> > {
		return wrap(vtx, bbuckets.get_bucket(deg));
	};
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0);
	std::vector<uintE> changeVtx(n, 0); // allocated outside of loop
	sequence<std::optional<std::tuple<uintE, uintE> > > moveV(n, true);
	size_t moveV_size = 0;
	size_t changeVtx_size = 0;
	// use a counter to track the number of changes and then use that to determine when to stop
	// don't .clear() the changeVtx vector, just reuse it
	uintE finished = 0;
	uintE vCount = 0;
	for(uintE i=n_a; i<n; i++) vCount+=D[i]>=alpha;
	while (finished != vCount)
	{
		iter++;
		auto bkt = bbuckets.next_bucket();
		max_beta = std::max((uintE)bkt.id, max_beta);
		finished += bkt.identifiers.size();
		for(uintE vi : bkt.identifiers){
			auto neighborsVi = G.get_vertex(vi).out_neighbors();
			for(uintE i = 0; i<neighborsVi.degree; i++){
				uintE ui = neighborsVi.get_neighbor(i);
				if(D[ui]-- == alpha){
					auto neighborsUi = G.get_vertex(ui).out_neighbors();
					for(uintE j = 0; j<neighborsUi.degree; j++){
						uintE vii = neighborsUi.get_neighbor(j); 
						if(D[vii] > max_beta){
							if(tracker[vii]!=iter){ // test par filter (figure out what par helps and what doesn't)
								changeVtx[changeVtx_size++] = vii;
								tracker[vii] = iter;
							}
							D[vii]--;
						}
					}
				}
			}
		}
		// use sequence --> alloc to max size (test on more graphs to see if resizing is expensive)
		for(size_t i = 0; i<changeVtx_size; i++){
			uintE vii = changeVtx[i];
			uintE deg = std::max(max_beta, D[vii]);
			Dv[vii] = deg; D[vii] = deg;
			auto ret = getVBuckets(vii, deg);
			if(ret) { moveV[moveV_size++] = ret; }
		}
		bbuckets.update_buckets_seq_arr(moveV, moveV_size);
		moveV_size = 0;
		changeVtx_size = 0; // clear gives compiler option to call destructor
		rho_alpha++;
	}
	std::cout<<"Alpha "<<alpha<<" "<<rho_alpha <<" "<<max_beta<<std::endl;
	return std::make_pair(0, 0);
}

template <class Graph>
inline std::pair<double, double> PeelFixB(Graph& G, sequence<uintE> D, uintE beta, size_t n_a, size_t n_b, size_t num_buckets)
{
	const size_t n = n_a + n_b;
	uintE rho_beta = 0, max_alpha = 0;

	auto Du = sequence<uintE>(n, std::numeric_limits<uintE>::max(), false);
	for(uintE i=0; i<n_a; i++) if(D[i]>=beta) Du[i]=D[i];

	auto abuckets = make_vertex_buckets(n,Du,increasing,num_buckets,true);
	auto getUBuckets = [&](const uintE& vtx, const uintE& deg)
		-> const std::optional<std::tuple<uintE, uintE> > {
		return wrap(vtx, abuckets.get_bucket(deg));
	};
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0); // tracks last time the degree changed
	std::vector<uintE> changeVtx(n, 0);
	sequence<std::optional<std::tuple<uintE, uintE> > > moveU(n, true);
	size_t moveU_size = 0;
	size_t changeVtx_size = 0;
	uintE finished = 0;
	uintE uCount = 0;
	for(uintE i=0; i<n_a; i++) uCount += D[i]>=beta;
	while (finished != uCount)
	{
		iter++;
		auto bkt = abuckets.next_bucket();
		max_alpha = std::max((uintE)bkt.id, max_alpha);
		finished += bkt.identifiers.size();
		for(uintE ui : bkt.identifiers){
			auto neighborsUi = G.get_vertex(ui).out_neighbors();
			for(uintE i = 0; i<neighborsUi.degree; i++){
				uintE vi = neighborsUi.get_neighbor(i);
				if(D[vi]-- == beta){
					auto neighborsVi = G.get_vertex(vi).out_neighbors();
					for(uintE j = 0; j<neighborsVi.degree; j++){
						uintE uii = neighborsVi.get_neighbor(j); 
						if(D[uii] > max_alpha){
							if(tracker[uii]!=iter){
								changeVtx[changeVtx_size++] = uii;
								tracker[uii] = iter;
							}
							D[uii]--;
						}
					}
				}
			}
		}
		for(size_t i = 0; i<changeVtx_size; i++){
			uintE uii = changeVtx[i];
			uintE deg = std::max(max_alpha, D[uii]);
			Du[uii] = deg; D[uii] = deg;
			auto ret = getUBuckets(uii, deg);
			if(ret) { moveU[moveU_size++] = ret; }
		}
		abuckets.update_buckets_seq_arr(moveU, moveU_size);
		changeVtx_size = 0;
		moveU_size = 0;
		rho_beta++;
	}
	std::cout<<"Beta "<<beta<<" "<<rho_beta <<" "<<max_alpha<<std::endl;
	return std::make_pair(0,0);
}

}

