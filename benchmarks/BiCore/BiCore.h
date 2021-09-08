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
	const size_t n = G.n;					// # of vertices
	const size_t n_a = bipartition + 1;		// number of vertices in first partition
	const size_t n_b = n - bipartition - 1; // number of vertices in second partition
	auto ret = KCore(G, num_buckets);
	const uintE delta = static_cast<uintE>(pbbslib::reduce_max(ret));
		
}

template <class Graph>
inline std::pair<double, double> PeelFixA(Graph& G, sequence<uintE> D, uintE alpha, size_t n_a, size_t n_b, size_t num_buckets)
{
	const size_t n = n_a + n_b;
	timer pqt, pt;
	uintE rho_alpha = 0, max_beta = 0;

	pqt.start();
	auto Dv = sequence<uintE>(n, [&](size_t i) {
		if (i < n_a || D[i] < alpha) return std::numeric_limits<uintE>::max();
		return D[i];
	});
	auto bbuckets = make_vertex_buckets(n,Dv,increasing,num_buckets);
	auto getVBuckets = [&](const uintE& vtx, const uintE& deg)
		-> const std::optional<std::tuple<uintE, uintE> > {
		return wrap(vtx, bbuckets.get_bucket(deg));
	};
	pqt.stop();
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0);
	std::vector<uintE> changeVtx;
	pbbslib::dyn_arr moveV(16);
	while (!bbuckets.empty())
	{
		iter++;
		pqt.start();
		auto bkt = bbuckets.next_bucket();
		max_beta = std::max(bkt.id, max_beta);
		auto bktArr = bkt.identifiers;
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
		for(uintE vii : changeVtx){
			uintE deg = std::max(max_beta, D[vii]);
			vD[vii] = deg; D[vii] = deg;
			auto ret = getVBuckets(vii, deg);
			if(ret) { moveV.resize(1); moveV.push_back(*ret); }
		}
		bbuckets.update_buckets(vertexSubsetData<uintE>(n, std::move(moveV.to_seq())));
		moveV.clear();
		changeVtx.clear();
		pqt.stop();
		rho_alpha++;
	}
	std::cout<<rho_alpha << " "<<max_beta<<std::endl;
	return std::make_pair(pqt.get_total(), pt.get_total());
}

template <class Graph>
inline std::pair<double, double> PeelFixB(Graph& G, sequence<uintE>& D, uintE beta, size_t n_a, size_t n_b, size_t num_buckets)
{
	const size_t n = n_a + n_b;
	timer pqt, pt;
	uintE rho_beta = 0, max_alpha = 0;

	pqt.start();
	auto Dv = sequence<uintE>(n, [&](size_t i) {
		if (i >= n_a || D[i] < beta) return std::numeric_limits<uintE>::max();
		return D[i];
	});
	auto abuckets = make_vertex_buckets(n,Dv,increasing,num_buckets);
	auto getUBuckets = [&](const uintE& vtx, const uintE& deg)
		-> const std::optional<std::tuple<uintE, uintE> > {
		return wrap(vtx, abuckets.get_bucket(deg));
	};
	pqt.stop();
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0);
	std::vector<uintE> changeVtx;
	pbbslib::dyn_arr moveU(16);
	while (!abuckets.empty())
	{
		iter++;
		pqt.start();
		auto bkt = abuckets.next_bucket();
		max_alpha = std::max(bkt.id, max_alpha);
		auto bktArr = bkt.identifiers;
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
		for(uintE uii : changeVtx){
			uintE deg = std::max(max_alpha, D[uii]);
			vD[uii] = deg; D[uii] = deg;
			auto ret = getUBuckets(uii, deg);
			if(ret) { moveU.resize(1); moveU.push_back(*ret); }
		}
		abuckets.update_buckets(vertexSubsetData<uintE>(n, std::move(moveU.to_seq())));
		moveU.clear();
		changeVtx.clear();
		pqt.stop();
		rho_beta++;
	}
	std::cout<<rho_beta << " "<<max_alpha<<std::endl;
	return std::make_pair(pqt.get_total(), pt.get_total());
}

}

