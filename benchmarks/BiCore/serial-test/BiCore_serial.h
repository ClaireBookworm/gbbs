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
template <class Graph>
inline std::pair<double, double> PeelFixA(Graph& G, std::vector<uintE>& Deg, size_t beta, size_t n_a, size_t n_b);
template <class Graph>
inline std::pair<double, double> PeelFixB(Graph& G, std::vector<uintE>& Deg, size_t beta, size_t n_a, size_t n_b);

struct Buckets{
	std::vector<uintE>* bkts;
	std::vector<uintE>& degs;
	uintE curDeg, n, ahead, curPos;
	Buckets(std::vector<uintE>& degs_, uintE startPos, uintE endPos)
	 : degs(degs_), curPos(0)
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

	inline uintE next_vtx(){
		if(curPos >= bkts[curDeg].size()){
			curDeg++;
			curPos = 0;
			while(curDeg <= n && bkts[curDeg].size() == 0) curDeg++;
		}
		std::vector<uintE>& bkt = bkts[curDeg];
		while(curPos < bkt.size() && degs[bkt[curPos]] != curDeg) curPos++;
		if(curPos < bkt.size()) {ahead--; return bkt[curPos++];}
		else return next_vtx();
	}

	inline std::vector<uintE> next_bucket(){
		std::vector<uintE> filterBkt; filterBkt.reserve(bkts[curDeg].size());
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
		return ahead == 0;
	}
};

template <class Graph>
inline Graph shrink_graph(Graph& G, const std::vector<uintE>& D, uintE n_a, uintE n_b, uintE cutoffA, uintE cutoffB){
	const size_t n = n_a + n_b;
	size_t nValid = n;
	std::vector<uintE> degs = D;
	for(uintE i = 0; i<n_a; i++) if(D[i]<cutoffA){ degs[i] = 0; nValid--; }
	for(uintE i = n_a; i<n; i++) if(D[i]<cutoffB){ degs[i] = 0; nValid--; }
	std::vector<uintT> offsets(n+1); offsets[0]=0;
	for(uintE i = 1; i<=n; i++) offsets[i] = offsets[i-1] + degs[i-1];
	const size_t m = offsets[n];
	vertex_data* v_data = pbbs::new_array_no_init<vertex_data>(n);
	for(uintE i = 0; i<n; i++) { v_data[i].offset = offsets[i]; v_data[i].degree = degs[i]; }
	uintE* edges = pbbs::new_array_no_init<uintE>(m);
	for(uintE i = 0; i<n; i++){
		if(i<n_a && D[i]<cutoffA) continue;
		if(i>=n_a && D[i]<cutoffB) continue;
		auto neighbors = G.get_vertex(i).out_neighbors();
		uintT offset = offsets[i];
		uintE idx = 0;
		for(uintE j = 0; j<neighbors.degree; j++){
			uintE neigh = neighbors.get_neighbor(j);
			if(neigh<n_a && D[neigh]<cutoffA) continue;
			if(neigh>=n_a && D[neigh]<cutoffB) continue;
			edges[idx+offset] = neigh; idx++;
		}
	}
	return Graph(
      v_data, n, nValid, m,
      [=](){
		pbbslib::free_array(v_data);
        pbbslib::free_array(edges);
      }, (std::tuple<uintE, pbbs::empty>*)edges);
}

inline uintE get_vertex_count(uintE vtx_c, std::vector<uintE> D, size_t n_a, size_t n_b, uintE cutoffA, uintE cutoffB){
	const size_t n = n_a + n_b;
	size_t new_n = vtx_c;
	for(uintE i = 0; i<n_a; i++) if(D[i]<cutoffA) new_n--;
	for(uintE i = n_a; i<n; i++) if(D[i]<cutoffB) new_n--;
	return new_n;
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
	double pt = 0;
	std::vector<uintE> DA(n);
	for(size_t i=0; i<n; i++) DA[i] = G.get_vertex(i).out_degree();
	std::vector<uintE> DB = DA;
	Graph GA = G.copy();
	Graph GB = G.copy();
	uintE vtx_countA = n;
	uintE vtx_countB = n;
	std::cout<<"finished preprocessing"<<std::endl;
	for(size_t core = 1; core<=delta; core++){
		std::cout<<"running PeelFixA core "<<core<<std::endl;
		auto ret = PeelFixA(GA, DA, core, n_a, n_b);
		pqt += ret.first;
		pt += ret.second;
	}
	for(size_t core = 1; core<=delta; core++){
		std::cout<<"running PeelFixB core "<<core<<std::endl;
		auto ret = PeelFixB(GB, DB, core, n_a, n_b);
		pqt += ret.first;
		pt += ret.second;
	}
	std::cout<<"pq time "<<pqt<<std::endl;
	std::cout<<"pt time "<<pt<<std::endl;
	//mkt.reportTotal("make_graph time");
}

template <class Graph>
inline void check(Graph& G, std::vector<uintE>& D, size_t n_a, size_t n_b, uintE cutoffA, uintE cutoffB){
	const size_t n = n_a + n_b;
	for(uintE i = 0; i<n; i++){
		if(i<n_a && D[i]<cutoffA) continue;
		if(i>=n_a && D[i]<cutoffB) continue;
		auto neighbors = G.get_vertex(i).out_neighbors();
		uintE idx = 0;
		for(uintE j = 0; j<neighbors.degree; j++){
			uintE neigh = neighbors.get_neighbor(j);
			if(neigh<n_a && D[neigh]<cutoffA) continue;
			if(neigh>=n_a && D[neigh]<cutoffB) continue;
			if(idx>=D[i])
				std::cout<<i<<" "<<idx<<" "<<D[i]<<std::endl;
			assert(idx<D[i]);idx++;
		}
	}
}

template <class Graph>
inline std::pair<double, double> PeelFixA(Graph& G, std::vector<uintE>& Deg, size_t alpha, size_t n_a, size_t n_b)
{
	const size_t n = n_a + n_b;
	timer pqt;
	timer pt;
	uintE rho_alpha = 0, max_beta = 0;
	uintE vtxCount = n;
	bool firstMove = true;
	pt.start();
	for(uintE i=0; i<n_a; i++) if(Deg[i]<alpha) vtxCount--;
	for(uintE i=n_a; i<n; i++) if(Deg[i]<1) vtxCount--;

	std::vector<uintE> uDel;
	for (size_t i = 0; i < n_a; i++)
		if (Deg[i] == alpha-1) uDel.push_back(i);
	// (alpha,0)
	// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
	while (uDel.size()>0)
	{
		std::vector<uintE> newUDel;
		for(uintE ui : uDel){
			vtxCount--;
			auto neighborsUi = G.get_vertex(ui).out_neighbors();
			for(uintE i = 0; i<neighborsUi.degree; i++){
				uintE vi = neighborsUi.get_neighbor(i);
				if(Deg[vi]<1) continue;
				Deg[vi]--;
				if(Deg[vi]<1){
					vtxCount--;
					auto neighborsVi = G.get_vertex(vi).out_neighbors();
					for(uintE j = 0; j<neighborsVi.degree; j++){
						uintE uii = neighborsVi.get_neighbor(j); 
						if(Deg[uii]<alpha) continue;
						Deg[uii]--;
						if(Deg[uii]<alpha) newUDel.push_back(uii);
					}
				}
			}
		}
		uDel = std::move(newUDel);
	}
	std::vector<uintE> D = Deg;
	Graph new_G;
	if(vtxCount*2 < G.nValid){
		firstMove=false;
		new_G = shrink_graph(G, D, n_a, n_b, alpha, 1);
		G = new_G;
		std::cout<<"compact at start "<<vtxCount<<std::endl;
	}else 
		new_G = std::move(G);

	pt.stop();

	pqt.start();

	Buckets bbuckets(D, n_a, n);
	pqt.stop();
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0);
	std::vector<uintE> changeVtx;
	while (!bbuckets.empty())
	{
		iter++;
		pqt.start();
		std::vector<uintE> bkt = bbuckets.next_bucket();
		if(bbuckets.curDeg > max_beta){
			if(vtxCount*3.5 < new_G.nValid && new_G.nValid*10 > n){
				if(firstMove){
					firstMove = false;
					G = std::move(new_G);
					new_G = shrink_graph(G, D, n_a, n_b, alpha, max_beta+1);
				}else
					new_G = shrink_graph(new_G, D, n_a, n_b, alpha, max_beta+1);
				uintE tempCount = 0;
				for(uintE i=0; i<n_a; i++) if(D[i]>=alpha) tempCount++;
				for(uintE i=n_a; i<n; i++) if(D[i]>max_beta) tempCount++;
				std::cout<<"compact "<<vtxCount<<" "<<new_G.nValid<<" "<<tempCount<<std::endl;
			}
			max_beta = bbuckets.curDeg;
		}
		pqt.stop();
		for(uintE vi : bkt){
			vtxCount--;
			D[vi]=0;
			auto neighborsVi = new_G.get_vertex(vi).out_neighbors();
			for(uintE i = 0; i<neighborsVi.degree; i++){
				uintE ui = neighborsVi.get_neighbor(i);
				if(D[ui]<alpha) continue;
				D[ui]--;
				if(D[ui]<alpha){
					D[ui]=0;
					vtxCount--;
					auto neighborsUi = new_G.get_vertex(ui).out_neighbors();
					for(uintE j = 0; j<neighborsUi.degree; j++){
						uintE vii = neighborsUi.get_neighbor(j); 
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
		changeVtx.clear();
		pqt.stop();
		rho_alpha++;
	}
	std::cout<<rho_alpha << " "<<max_beta<<std::endl;
	if(firstMove) G = std::move(new_G);
	return std::make_pair(pqt.get_total(), pt.get_total());
}

template <class Graph>
inline std::pair<double, double> PeelFixB(Graph& G, std::vector<uintE>& Deg, size_t beta, size_t n_a, size_t n_b)
{
	const size_t n = n_a + n_b;
	timer pqt;
	timer pt;
	uintE rho_beta = 0, max_alpha = 0;
	uintE vtxCount = n;
	bool firstMove = true;
	pt.start();

	for(uintE i=0; i<n_a; i++) if(Deg[i]<1) vtxCount--;
	for(uintE i=n_a; i<n; i++) if(Deg[i]<beta) vtxCount--;
	std::vector<uintE> vDel;
	for (uintE i = n_a; i < n; i++)
		if (Deg[i] == beta-1) vDel.push_back(i);

	while (vDel.size()>0)
	{
		std::vector<uintE> newVDel;
		for(uintE vi : vDel){
			vtxCount--;
			auto neighborsVi = G.get_vertex(vi).out_neighbors();
			for(uintE i = 0; i<neighborsVi.degree; i++){
				uintE ui = neighborsVi.get_neighbor(i);
				if(Deg[ui]<1) continue;
				Deg[ui]--;
				if(Deg[ui]<1){
					vtxCount--;
					auto neighborsUi = G.get_vertex(ui).out_neighbors();
					for(uintE j = 0; j<neighborsUi.degree; j++){
						uintE vii = neighborsUi.get_neighbor(j); 
						if(Deg[vii]<beta) continue;
						Deg[vii]--;
						if(Deg[vii]<beta) newVDel.push_back(vii);
					}
				}
			}
		}
		vDel = std::move(newVDel);
	}
	std::vector<uintE> D = Deg;
	Graph new_G;
	std::cout<<"initial count "<<vtxCount<<" "<<G.nValid<<std::endl;
	if(vtxCount*2 < G.nValid){
		firstMove=false;
		new_G = shrink_graph(G, D, n_a, n_b, 1, beta);
		G = new_G;
		std::cout<<"compact at start "<<vtxCount<<std::endl;
	}else 
		new_G = std::move(G);

	pt.stop();

	pqt.start();

	Buckets abuckets(D, 0, n_a);
	pqt.stop();
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0);
	std::vector<uintE> changeVtx; 
	while (!abuckets.empty())
	{
		iter++;
		pqt.start();
		std::vector<uintE> bkt = abuckets.next_bucket();
		if(abuckets.curDeg > max_alpha){
			if(vtxCount*3.5 < new_G.nValid && new_G.nValid*10 > n){
				if(firstMove){
					firstMove = false;
					G = std::move(new_G);
					new_G = shrink_graph(G, D, n_a, n_b, max_alpha+1, beta);
				}else
					new_G = shrink_graph(new_G, D, n_a, n_b, max_alpha+1, beta);
				std::cout<<"compact "<<vtxCount<<std::endl;
			}
			max_alpha = abuckets.curDeg;
		}
		pqt.stop();
		for(uintE ui : bkt){
			vtxCount--;
			auto neighborsUi = new_G.get_vertex(ui).out_neighbors();
			for(uintE i = 0; i<neighborsUi.degree; i++){
				uintE vi = neighborsUi.get_neighbor(i);
				if(D[vi]<beta) continue;
				D[vi]--;
				if(D[vi]<beta){
					vtxCount--;
					auto neighborsVi = new_G.get_vertex(vi).out_neighbors();
					for(uintE j = 0; j<neighborsVi.degree; j++){
						uintE uii = neighborsVi.get_neighbor(j);
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
		changeVtx.clear();
		pqt.stop();
		rho_beta++;
	}
	std::cout<<rho_beta << " "<<max_alpha<<std::endl;
	if(firstMove) G = std::move(new_G);
	return std::make_pair(pqt.get_total(), pt.get_total());
}

}
