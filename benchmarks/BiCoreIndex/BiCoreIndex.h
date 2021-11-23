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
struct BiCoreIndex{
	Graph& G; 
	size_t n, n_a, n_b;
	sequence<sequence<uintE> > AlphaBetaQ;
	BiCoreIndex(Graph& _G, size_t _bipartition, sequence<sequence<uintE> >& AlphaMax, sequence<sequence<uintE> >& BetaMax)
	: G(_G)
	{
		const size_t n = G.n;					// # of vertices
		const size_t n_a = _bipartition + 1;		// number of vertices in first partition
		const size_t n_b = n - _bipartition - 1; // number of vertices in second partition
		// AlphaMax is n_b by deg+1
		// BetaMax is n_a by deg+1
		// Let's consider BetaMax
		std::cout<<"creation starting"<<std::endl;
		sequence<uintE> indices(n_a, [&](uintE vtx){
			return BetaMax[vtx].size()-1;
		});

		uintT total = pbbslib::scan_add_inplace(indices); // exclusive prefix sum
		sequence<std::tuple<uintE, uintE, uintE> > allVtx(total);

		std::cout<<"indices obtained"<<std::endl;

		par_for(0, n_a, [&](uintE vtx){
			uintT st = indices[vtx];
			par_for(1, BetaMax[vtx].size(), [&](uintE alpha){
				uintE beta = BetaMax[vtx][alpha];
				assert(beta<100000000);
				allVtx[st + alpha - 1] = std::make_tuple(alpha, beta, vtx);
			});
		});

		std::cout<<"allVtx created"<<std::endl;
	  	pbbslib::sample_sort_inplace(allVtx.slice(), std::less<std::tuple<uintE,uintE,uintE> >(), false);

	  	std::cout<<"allVtx sorted"<<std::endl;

	  	assert(total>0);

	  	sequence<uintE> tptP(total);
	  	tptP[0] = 1;
	  	par_for(1, total, [&](uintE i){
	  		if(std::get<0>(allVtx[i]) != std::get<0>(allVtx[i-1]) || std::get<1>(allVtx[i]) != std::get<1>(allVtx[i-1])) tptP[i] = i; 
	  		else tptP[i] = 0;
	  	});
	  	sequence<uintE> TPT = pbbslib::filter(tptP, [](uintE idx){ return idx>0; });
	  	TPT[0] = 0;

	  	std::cout<<"TPT created"<<std::endl;

	  	sequence<uintE> fptP(TPT.size());
	  	fptP[0] = 1; 
	  	par_for(1, TPT.size(), [&](uintE i){
	  		if(std::get<0>(allVtx[TPT[i]]) != std::get<0>(allVtx[TPT[i-1]])) fptP[i] = i;
	  		else fptP[i] = 0;
	  	});
	  	sequence<uintE> FPT = pbbslib::filter(fptP, [](uintE idx){ return idx>0; });
	  	FPT[0] = 0;

	  	std::cout<<"FPT created"<<std::endl;

	  	// FPT is all set, its indices are alpha-1
	  	AlphaBetaQ = sequence<sequence<uintE> >(FPT.size(), [&](uintE i){
	  		uintE endC, startC = FPT[i];
	  		if(i < FPT.size()-1) endC = FPT[i+1]-1;
	  		else endC = TPT.size()-1;
	  		uintE maxBeta = std::get<1>(allVtx[TPT[endC]]);
	  		sequence<uintE> SPT(maxBeta, std::numeric_limits<uintE>::max());
	  		par_for(startC, endC+1, [&](uintE j){
	  			uintE beta = std::get<1>(allVtx[TPT[j]]);
	  			SPT[beta-1] = TPT[j]; // SPT is also beta-1
	  		});
	  		pbbslib::scan_inplace(SPT.rslice(), pbbslib::minm<uintE>(), pbbslib::fl_scan_inclusive);
	  		return SPT;
	  	});
	  	std::cout<<"done"<<std::endl;
	}
};


}





