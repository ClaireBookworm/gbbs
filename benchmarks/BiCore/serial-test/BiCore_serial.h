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
inline std::pair<double, double> PeelFixA(Graph& G, std::vector<uintE>& Deg, uintE alpha, size_t n_a, size_t n_b);
template <class Graph>
inline std::pair<double, double> PeelFixB(Graph& G, std::vector<uintE>& Deg, uintE beta, size_t n_a, size_t n_b);

struct Buckets{
	std::vector<uintE>* bkts;
	std::vector<uintE>& degs;
	uintE curDeg, n, ahead, curPos;
	Buckets(std::vector<uintE>& degs_, size_t startPos, size_t endPos, uintE cutoff)
	 : degs(degs_), curPos(0)
	{
		uintE maxDeg = 0;
		uintE minDeg = 0;
		ahead = 0;
		for(size_t i=startPos; i<endPos; i++) {
			maxDeg = std::max(degs[i], maxDeg);
			if(degs[i]>=cutoff){ minDeg = std::min(degs[i], minDeg); ahead++; }
		}
		bkts = new std::vector<uintE>[maxDeg+1];
		for(size_t i = minDeg; i<=maxDeg; i++) bkts[i].reserve((size_t)(ahead/(maxDeg-minDeg+1)));
		for(size_t i=startPos; i<endPos; i++)
			if(degs[i]>=cutoff) bkts[degs[i]].push_back(i);
		curDeg = minDeg;
		n = maxDeg;
	}

	inline void update(uintE idx, uintE newDeg){ bkts[newDeg].push_back(idx); }

	inline uintE next_bucket_deg(){
		while(curDeg <= n && bkts[curDeg].size() == 0) curDeg++;
		return curDeg;
	}

	inline bool empty(){
		return ahead == 0;
	}
};

template <class Graph>
inline Graph shrink_graph(Graph& G, const std::vector<uintE>& D, size_t n_a, size_t n_b, uintE cutoffA, uintE cutoffB){
	const size_t n = n_a + n_b;
	std::vector<uintE> degs = D;
	std::vector<uintE> oldDegs(n);
	for(size_t i = 0; i<n; i++) oldDegs[i] = G.v_data[i].degree; 
	for(size_t i = 0; i<n_a; i++) if(D[i]<cutoffA){ degs[i] = 0; }
	for(size_t i = n_a; i<n; i++) if(D[i]<cutoffB){ degs[i] = 0; }
	std::vector<uintT> offsets(n+1); offsets[0]=0;
	for(uintE i = 1; i<=n; i++) offsets[i] = offsets[i-1] + degs[i-1];
	const size_t m = offsets[n];
	vertex_data* v_data = pbbs::new_array_no_init<vertex_data>(n);
	for(uintE i = 0; i<n; i++) { v_data[i].offset = offsets[i]; v_data[i].degree = degs[i]; }
	uintE* edges = pbbs::new_array_no_init<uintE>(m);
	uintT idx = 0;
	for(uintE i = 0; i<n; i++){
		uintE oldDeg = oldDegs[i];
		uintT offset = offsets[i];
		if(degs[i] == 0){
			idx += oldDeg;
			continue;
		}
		for(uintE oj = 0; oj<oldDeg; oj++, idx++){
			uintE oid = std::get<0>(G.e0[idx]);
			edges[offset] = oid; offset+=degs[oid]>0;
		}
	}
	return Graph(
		v_data, n, m,
      [v_data, edges](){ pbbslib::free_array(v_data); pbbslib::free_array(edges); }, (std::tuple<uintE, pbbs::empty>*)edges);
}

template <class Graph>
inline void BiCore_serial(Graph &G, size_t num_buckets = 16, size_t bipartition = 2, uintE peel_core_alpha = 0, uintE peel_core_beta = 0)
{
	std::cout << "starting" << std::endl;
	const size_t n = G.n;					// # of vertices
	const size_t n_a = bipartition + 1;		// number of vertices in first partition
	const size_t n_b = n - bipartition - 1; // number of vertices in second partition
	auto ret = KCore(G, num_buckets);
	const uintE delta = static_cast<uintE>(pbbslib::reduce_max(ret));
	double pqt = 0;
	double pt = 0;
	std::vector<uintE> DA(n);
	for(size_t i=0; i<n; i++) DA[i] = G.get_vertex(i).out_degree();
	std::vector<uintE> DB = DA;
	std::cout<<"m "<<G.m<<" "<<G.n<<std::endl;
	Graph GA = G;
	std::cout<<"finished preprocessing"<<std::endl;
	for(uintE core = 1; core<=delta; core++){
		std::cout<<"running PeelFixA core "<<core<<std::endl;
		auto ret = PeelFixA(GA, DA, core, n_a, n_b);
		pqt += ret.first;
		pt += ret.second;
	}
	for(uintE core = 1; core<=delta; core++){
		std::cout<<"running PeelFixB core "<<core<<std::endl;
		auto ret = PeelFixB(G, DB, core, n_a, n_b);
		pqt += ret.first;
		pt += ret.second;
	}
	std::cout<<"pq time "<<pqt<<std::endl;
	std::cout<<"pt time "<<pt<<std::endl;
	//mkt.reportTotal("make_graph time");
}

template <class Graph>
inline std::pair<double, double> PeelFixA(Graph& G, std::vector<uintE>& Deg, uintE alpha, size_t n_a, size_t n_b)
{
	const size_t n = n_a + n_b;
	timer pqt, pt;
	uintE rho_alpha = 0, max_beta = 0;
	size_t edgeCount = 0;
	pt.start();

	std::vector<uintE> Del; Del.reserve(16);
	for (size_t i = 0; i < n; i++)
		if (Deg[i] == alpha-1) Del.push_back(i);
	// (alpha,0)
	// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
	while (!Del.empty())
	{
		std::vector<uintE> newDel;
		for(uintE vi : Del){
			auto neighborsVi = G.get_vertex(vi).out_neighbors();
			for(uintE i = 0; i<neighborsVi.degree; i++){
				uintE oi = neighborsVi.get_neighbor(i);
				if(Deg[oi]-- == alpha) newDel.push_back(oi);
			}
		}
		Del = std::move(newDel);
	}
	for(size_t i=0; i<n; i++) if(Deg[i]>=alpha) edgeCount+=Deg[i];
	std::vector<uintE> D = Deg;
	std::cout<<"cur state "<<edgeCount<<" "<<G.m<<std::endl;
	if(edgeCount*1.1 < G.m && G.m > 1000){
		G = shrink_graph(G, D, n_a, n_b, alpha, alpha);
		std::cout<<"compact at start "<<edgeCount<<std::endl;
	}

	pt.stop();

	pqt.start();

	Buckets bbuckets(D, n_a, n, alpha);
	pqt.stop();
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0);
	std::vector<uintE> changeVtx;
	while (!bbuckets.empty())
	{
		iter++;
		pqt.start();
		uintE curDeg = bbuckets.next_bucket_deg();
		max_beta = std::max(curDeg, max_beta);
		std::vector<uintE>& bkt = bbuckets.bkts[curDeg];
		pqt.stop();
		while(!bkt.empty()){
			uintE vi = bkt.back(); bkt.pop_back();
			if(D[vi] != curDeg) continue;
			bbuckets.ahead--;
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
			D[vii] = std::max(max_beta, D[vii]);
			bbuckets.update(vii, D[vii]);
		}
		changeVtx.clear();
		pqt.stop();
		rho_alpha++;
	}
	std::cout<<rho_alpha << " "<<max_beta<<std::endl;
	return std::make_pair(pqt.get_total(), pt.get_total());
}

template <class Graph>
inline std::pair<double, double> PeelFixB(Graph& G, std::vector<uintE>& Deg, uintE beta, size_t n_a, size_t n_b)
{
	const size_t n = n_a + n_b;
	timer pqt,pt;
	uintE rho_beta = 0, max_alpha = 0;
	size_t edgeCount = 0;
	pt.start();

	std::vector<uintE> Del; Del.reserve(16);
	for (uintE i = 0; i < n; i++)
		if (Deg[i] == beta-1) Del.push_back(i);

	while (!Del.empty())
	{
		std::vector<uintE> newDel;
		for(uintE vi : Del){
			auto neighborsVi = G.get_vertex(vi).out_neighbors();
			for(uintE i = 0; i<neighborsVi.degree; i++){
				uintE oi = neighborsVi.get_neighbor(i);
				if(Deg[oi]-- == beta) newDel.push_back(oi);
			}
		}
		Del = std::move(newDel);
	}
	for(size_t i=0; i<n; i++) if(Deg[i]>=beta) edgeCount+=Deg[i];
	std::vector<uintE> D = Deg;
	std::cout<<"cur state "<<edgeCount<<" "<<G.m<<std::endl;
	if(edgeCount*1.1 < G.m && G.m > 1000){
		G = shrink_graph(G, D, n_a, n_b, beta, beta);
		std::cout<<"compact at start "<<edgeCount<<std::endl;
	}

	pt.stop();

	pqt.start();

	Buckets abuckets(D, 0, n_a, beta);
	pqt.stop();
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0);
	std::vector<uintE> changeVtx;
	while (!abuckets.empty())
	{
		iter++;
		pqt.start();
		uintE curDeg = abuckets.next_bucket_deg();
		max_alpha = std::max(max_alpha, curDeg);
		std::vector<uintE>& bkt = abuckets.bkts[curDeg];
		pqt.stop();
		while(!bkt.empty()){
			uintE ui = bkt.back(); bkt.pop_back();
			if(D[ui] != curDeg) continue;
			abuckets.ahead--;
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
			D[uii] = std::max(D[uii], max_alpha);
			abuckets.update(uii, D[uii]);
		}
		changeVtx.clear();
		pqt.stop();
		rho_beta++;
	}
	std::cout<<rho_beta << " "<<max_alpha<<std::endl;
	return std::make_pair(pqt.get_total(), pt.get_total());
}

}
