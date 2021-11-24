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
	sequence<std::tuple<uintE, uintE, uintE> > UP, VP;
	sequence<sequence<uintE> > queryTU, queryTV;
	BiCoreIndex(Graph& _G, size_t _bipartition, sequence<sequence<uintE> >& AlphaMax, sequence<sequence<uintE> >& BetaMax)
	: G(_G), n(_G.n)
	{
		n_a = _bipartition + 1;		// number of vertices in first partition
		n_b = n - _bipartition - 1; // number of vertices in second partition
		// AlphaMax is n_b by deg+1
		// BetaMax is n_a by deg+1
		// Let's consider BetaMax
		std::cout<<"creation starting"<<std::endl;
		auto buildU = [&](){ build_table(n_a, BetaMax, UP, queryTU); };
		auto buildV = [&](){ build_table(n_b, AlphaMax, VP, queryTV); }; // note that VP vtx needs offset
		par_do(buildU, buildV);
		// buildU();
	  	std::cout<<"done"<<std::endl;
	}

	sequence<uintE> query(uintE alpha, uintE beta){
		alpha--; beta--; // remember our query table is down by 1 index

		uintE rangeU = 0, rangeV = 0;
		uintE startCU, startCV;

		if(alpha<queryTU.size() && beta<queryTU[alpha].size()){
			uintE endC = 0;
			if(alpha < queryTU.size()-1) endC = queryTU[alpha+1][0]-1;
			else endC = UP.size()-1;
			startCU = queryTU[alpha][beta];
			rangeU = endC - startCU + 1;
		}

		if(beta<queryTV.size() && alpha<queryTV[beta].size()){
			uintE endC = 0;
			if(beta < queryTV.size()-1) endC = queryTV[beta+1][0]-1;
			else endC = VP.size()-1;
			startCV = queryTV[beta][alpha];
			rangeV = endC - startCV + 1;
		}

		sequence<uintE> vtxs(rangeU + rangeV);
		auto moveU = [&](){ 
			par_for(0, rangeU, [&](uintE id){
				vtxs[id] = std::get<2>(UP[id+startCU]);
			});
		};
		auto moveV = [&](){
			par_for(0, rangeV, [&](uintE id){
				vtxs[rangeU + id] = std::get<2>(VP[id+startCV]) + n_a;
			});
		};
		par_do(moveU, moveV);
		return vtxs;
	}

	void build_table(size_t nn, sequence<sequence<uintE> >& DegMax, sequence<std::tuple<uintE, uintE, uintE> >& allVtx, sequence<sequence<uintE> >& queryT){
		sequence<uintE> indices(nn, [&](uintE vtx){
			return DegMax[vtx].size()-1;
		});

		uintT total = pbbslib::scan_add_inplace(indices); // exclusive prefix sum
		allVtx = sequence<std::tuple<uintE, uintE, uintE> >(total);

		par_for(0, nn, [&](uintE vtx){
			uintT st = indices[vtx];
			par_for(1, DegMax[vtx].size(), [&](uintE deg1){
				uintE deg2 = DegMax[vtx][deg1];
				allVtx[st + deg1 - 1] = std::make_tuple(deg1, deg2, vtx);
			});
		});

	  	pbbslib::sample_sort_inplace(allVtx.slice(), std::less<std::tuple<uintE,uintE,uintE> >(), false);
	  	sequence<uintE> tptP(total);
	  	tptP[0] = 1;
	  	par_for(1, total, [&](uintE i){
	  		if(std::get<0>(allVtx[i]) != std::get<0>(allVtx[i-1]) || std::get<1>(allVtx[i]) != std::get<1>(allVtx[i-1])) tptP[i] = i; 
	  		else tptP[i] = 0;
	  	});
	  	sequence<uintE> TPT = pbbslib::filter(tptP, [](uintE idx){ return idx>0; });
	  	TPT[0] = 0;

	  	sequence<uintE> fptP(TPT.size());
	  	fptP[0] = 1; 
	  	par_for(1, TPT.size(), [&](uintE i){
	  		if(std::get<0>(allVtx[TPT[i]]) != std::get<0>(allVtx[TPT[i-1]])) fptP[i] = i;
	  		else fptP[i] = 0;
	  	});
	  	sequence<uintE> FPT = pbbslib::filter(fptP, [](uintE idx){ return idx>0; });
	  	FPT[0] = 0;

	  	// FPT is all set, its indices are deg1-1
	  	queryT = sequence<sequence<uintE> >(FPT.size(), [&](uintE i){
	  		uintE endC, startC = FPT[i];
	  		if(i < FPT.size()-1) endC = FPT[i+1]-1;
	  		else endC = TPT.size()-1;
	  		uintE maxDeg2 = std::get<1>(allVtx[TPT[endC]]);
	  		sequence<uintE> SPT(maxDeg2, std::numeric_limits<uintE>::max());
	  		par_for(startC, endC+1, [&](uintE j){
	  			uintE deg2 = std::get<1>(allVtx[TPT[j]]);
	  			SPT[deg2-1] = TPT[j]; // SPT is also deg2-1
	  		});
	  		pbbslib::scan_inplace(SPT.rslice(), pbbslib::minm<uintE>(), pbbslib::fl_scan_inclusive);
	  		return SPT;
	  	});
	}
};


}





