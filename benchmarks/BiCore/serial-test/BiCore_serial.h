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
#include <list>

namespace gbbs{
// use max alpha and beta
inline double PeelFixA(std::vector<uintE>* adjG, size_t alpha, size_t n_a, size_t n_b);
inline double PeelFixB(std::vector<uintE>* adjG, size_t alpha, size_t n_a, size_t n_b);

struct Buckets{
	std::vector<uintE>* bkts;
	std::vector<uintE>& degs;
	uintE curDeg, n, ahead, st, et;
	Buckets(std::vector<uintE>& degs_, uintE startPos, uintE endPos)
	 : degs(degs_), st(startPos), et(endPos) 
	{
		uintE maxDeg = 0;
		uintE minDeg = 0;
		for(size_t i=startPos; i<endPos; i++) {
			maxDeg = std::max(degs[i], maxDeg);
			if(degs[i]>0) minDeg = std::min(degs[i], minDeg);
		}
		bkts = new std::vector<uintE>[maxDeg+1];
		ahead = 0;
		for(size_t i=startPos; i<endPos; i++)
			if(degs[i]>0){bkts[degs[i]].push_back(i); ahead++;}
		curDeg = minDeg;
		n = maxDeg;
	}

	inline void update(uintE idx, uintE newDeg){ bkts[newDeg].push_back(idx); }

	inline std::vector<uintE> next_bucket(){
		std::vector<uintE> filterBkt;
		while(filterBkt.size()==0){
			while(curDeg <= n && bkts[curDeg].size() == 0) curDeg++;
			std::vector<uintE>& bkt = bkts[curDeg];
			for(uintE i = bkt.size()-1; true; i--){
				if(degs[bkt[i]] == curDeg) filterBkt.push_back(bkt[i]);
				bkt.pop_back();
				if(i==0) break;
			}
		}
		ahead -= filterBkt.size();	
		return filterBkt;
	}

	inline bool empty(){
		assert(ahead>=0);
		return ahead == 0;
	}
};

template <class Graph>
inline std::vector<uintE>* make_graph(Graph& G){
	const size_t n = G.n;
	std::vector<uintE>* adjG = new std::vector<uintE>[n];
	for(size_t i=0; i<n; i++){
		auto neighbors = G.get_vertex(i).out_neighbors();
		for (size_t j = 0; j < neighbors.degree; j++){
			size_t other = neighbors.get_neighbor(j);
			adjG[i].push_back(other);
		}
	}
	return adjG;
}

template <class Graph>
inline void BiCore_serial(Graph &G, size_t num_buckets = 16, size_t bipartition = 2, size_t peel_core_alpha = 0, size_t peel_core_beta = 0)
{
	std::cout << "starting" << std::endl;
	const size_t n = G.n;					// # of vertices
	const size_t n_a = bipartition + 1;		// number of vertices in first partition
	const size_t n_b = n - bipartition - 1; // number of vertices in second partition

	auto ret = KCore(G, num_buckets);
	const size_t delta = static_cast<size_t>(pbbslib::reduce_max(ret));
	double pqt = 0;
	std::vector<uintE>* adjG = make_graph(G);
	std::cout<<"finished preprocessing"<<std::endl;
	for(size_t core = 1; core<=delta; core++){
		std::cout<<"running PeelFixA core "<<core<<std::endl;
		pqt += PeelFixA(adjG, core, n_a, n_b);
	}
	for(size_t core = 1; core<=delta; core++){
		std::cout<<"running PeelFixB core "<<core<<std::endl;
		pqt += PeelFixB(adjG, core, n_a, n_b);
	}
	std::cout<<"pq time "<<pqt<<std::endl;
	//mkt.reportTotal("make_graph time");
}

inline double PeelFixA(std::vector<uintE>* adjG, size_t alpha, size_t n_a, size_t n_b)
{
	const size_t n = n_a + n_b;
	timer pqt;
	timer pt;
	uintE rho_alpha = 0, max_beta = 0;

	pt.start();

	std::vector<uintE> D(n);
	for(size_t i=0; i<n; i++) D[i] = adjG[i].size();

	std::vector<uintE> uDel;
	for (size_t i = 0; i < n_a; i++)
		if (D[i] < alpha) uDel.push_back(i);
	// (alpha,0)
	// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
	while (uDel.size()>0)
	{
		std::vector<uintE> newUDel;
		for(uintE ui : uDel){
			for(uintE vi : adjG[ui]){
				if(D[vi]<1) continue;
				D[vi]--;
				if(D[vi]<1){
					for(uintE uii : adjG[vi]){ 
						if(D[uii]<alpha) continue;
						D[uii]--;
						if(D[uii]<alpha) newUDel.push_back(uii);
					}
				}
			}
		}
		uDel = std::move(newUDel);
	}

	pt.stop();

	pqt.start();

	Buckets bbuckets(D, n_a, n);
	pqt.stop();
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0);
	while (!bbuckets.empty())
	{
		iter++;
		pqt.start();
		std::vector<uintE> bkt = bbuckets.next_bucket();
		max_beta = std::max(max_beta, bbuckets.curDeg);
		pqt.stop();
		std::vector<uintE> changeVtx;
		for(uintE vi : bkt){
			for(uintE ui : adjG[vi]){
				if(D[ui]<alpha) continue;
				D[ui]--;
				if(D[ui]<alpha){
					for(uintE vii : adjG[ui]){ 
						if(D[vii]<=max_beta) continue;
						if(tracker[vii]!=iter){
							changeVtx.push_back(vii);
							tracker[vii] = iter;
						}
						D[vii]--;
					}
				}
			}
		}
		pqt.start();
		for(uintE vii : changeVtx)
			bbuckets.update(vii, D[vii]);
		pqt.stop();
		rho_alpha++;
	}
	std::cout<<rho_alpha << " "<<max_beta<<std::endl;
	return pqt.get_total();
}

inline double PeelFixB(std::vector<uintE>* adjG, size_t beta, size_t n_a, size_t n_b)
{
	const size_t n = n_a + n_b;
	timer pqt;
	timer pt;
	uintE rho_beta = 0, max_alpha = 0;

	pt.start();

	std::vector<uintE> D(n);
	for(size_t i=0; i<n; i++) D[i] = adjG[i].size();

	std::vector<uintE> vDel;
	for (size_t i = n_a; i < n; i++)
		if (D[i] < beta) vDel.push_back(i);
	// (alpha,0)
	// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
	while (vDel.size()>0)
	{
		std::vector<uintE> newVDel;
		for(uintE vi : vDel){
			for(uintE ui : adjG[vi]){
				if(D[ui]<1) continue;
				D[ui]--;
				if(D[ui]<1){
					for(uintE vii : adjG[ui]){ 
						if(D[vii]<beta) continue;
						D[vii]--;
						if(D[vii]<beta) newVDel.push_back(vii);
					}
				}
			}
		}
		vDel = std::move(newVDel);
	}

	pt.stop();

	pqt.start();

	Buckets abuckets(D, 0, n_a);
	pqt.stop();
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0);
	while (!abuckets.empty())
	{
		iter++;
		pqt.start();
		std::vector<uintE> bkt = abuckets.next_bucket();
		max_alpha = std::max(max_alpha, abuckets.curDeg);
		pqt.stop();
		std::vector<uintE> changeVtx;
		for(uintE ui : bkt){
			for(uintE vi : adjG[ui]){
				if(D[vi]<beta) continue;
				D[vi]--;
				if(D[vi]<beta){
					for(uintE uii : adjG[vi]){ 
						if(D[uii]<=max_alpha) continue;
						if(tracker[uii]!=iter){
							changeVtx.push_back(uii);
							tracker[uii] = iter;
						}
						D[uii]--;
					}
				}
			}
		}
		pqt.start();
		for(uintE uii : changeVtx)
			abuckets.update(uii, D[uii]);
		pqt.stop();
		rho_beta++;
	}
	std::cout<<rho_beta << " "<<max_alpha<<std::endl;
	return pqt.get_total();
}

}