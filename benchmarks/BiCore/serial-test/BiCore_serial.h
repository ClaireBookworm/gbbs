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

struct Node{
	uintE idx;
	uintE deg;
	Node(size_t idx_, size_t deg_) : idx(idx_), deg(deg_) {}
	bool operator<(const Node& other) const{
		return deg < other.deg;
	}
	bool operator>(const Node& other) const{
		return deg > other.deg;
	}
};

struct Buckets{
	std::unordered_set<uintE>* bkts;
	Buckets(std::vector<uintE>& degs, uintE startPos){
		size_t maxDeg = 0;
		size_t minDeg = 0;
		for(size_t i=startPos; i<degs.size(); i++) {
			maxDeg = std::max(degs[i], maxDeg);
			minDeg = std::min(degs[i], minDeg);
		}
		bkts = new std::unordered_set<uintE>[maxDeg+1];
		for(size_t i=startPos; i<degs.size(); i++)
			bkts[degs[i]].insert(i);
	}
	inline void update(uintE idx, uintE oldDeg, uintE newDeg){
		bkts[oldDeg].erase(idx);
		bkts[newDeg].insert(idx);
	}

}

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

inline void nghCount(std::vector<uintE>* adjG, std::vector<uintE>& D, size_t vtx, size_t cutoff, std::vector<uintE>& delList){
	//everything less than cutoff is deleted
	std::vector<uintE>& neighbors = adjG[vtx];
	for (size_t i = 0; i<neighbors.size(); i++){
		uintE id = neighbors[i];
		if(D[id]==cutoff)
			delList.push_back(id);
		if(D[id]>=cutoff)
			D[id]--;
	}
	D[vtx] = 0;
}

inline std::vector<uintE> nghCount(std::vector<uintE>* adjG, std::vector<uintE>& D, std::vector<uintE>& del, std::vector<size_t>& tracker, size_t iter, size_t cutoff){
	//everything less than cutoff is deleted
	std::vector<uintE> changeVtx;
	for(uintE vtx : del){
		std::vector<uintE>& neighbors = adjG[vtx];
		for (size_t i = 0; i<neighbors.size(); i++){
			uintE id = neighbors[i];
			if(D[id]>=cutoff){
				if(tracker[id]<iter){
					changeVtx.push_back(id);
					tracker[id] = iter;
				}
				D[id]--;
			}
		}
		D[vtx] = 0;
	}
	return changeVtx;
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
		if (D[i] < alpha)
			uDel.push_back(i);
	// (alpha,0)
	// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
	while (uDel.size()>0)
	{
		// everything less than cutoff is deleted
		//std::cout<<uDel.size()<<std::endl;
		std::vector<uintE> vDel;
		for(size_t del : uDel)
			nghCount(adjG, D, del, 1, vDel);
		uDel.clear();
		for(size_t del : vDel)
			nghCount(adjG, D, del, alpha, uDel);
	}

	pt.stop();

	pqt.start();
	std::vector<Node> remnants;
	for(size_t i=n_a; i<n; i++) if(D[i]>0) remnants.push_back(Node(i,D[i]));

	using PQNode = std::priority_queue<Node, std::vector<Node>, std::greater<Node> >;
	PQNode pq(remnants.begin(), remnants.end());
	pqt.stop();
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0);
	while (!pq.empty())
	{
		pqt.start();
		Node node = pq.top();
		//std::cout<<node.idx<<" "<<node.deg<<" "<<pq.size()<<std::endl;
		pq.pop();
		pqt.stop();
		if(D[node.idx] != node.deg) continue; //old values
		max_beta = std::max(max_beta, node.deg);

		std::vector<uintE> deleteU;
		nghCount(adjG, D, node.idx, alpha, deleteU);
		std::vector<uintE> changeVtx = nghCount(adjG, D, deleteU, tracker, ++iter, max_beta);
		for(uintE i : changeVtx) pq.push(Node(i, std::max(max_beta,D[i])));
		rho_alpha++;
	}
	std::cout<<rho_alpha << " "<<max_beta<<std::endl;
	return pt.get_total();
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
	for (size_t i = n_a; i < n; i++) if (D[i] < beta) vDel.push_back(i);
	// (alpha,0)
	// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
	while (vDel.size()>0)
	{
		// everything less than cutoff is deleted
		std::vector<uintE> uDel;
		for(size_t del : vDel)
			nghCount(adjG, D, del, 1, uDel);
		vDel.clear();
		for(size_t del : uDel)
			nghCount(adjG, D, del, beta, vDel);
	}
	pt.stop();
	
	std::vector<Node> remnants;
	for(size_t i=0; i<n_a; i++) if(D[i]>0) remnants.push_back(Node(i,D[i]));

	pqt.start();
	using PQNode = std::priority_queue<Node, std::vector<Node>, std::greater<Node> >;
	PQNode pq(remnants.begin(), remnants.end());
	pqt.stop();
	size_t iter = 0;
	std::vector<size_t> tracker(n, 0);
	while (!pq.empty())
	{
		pqt.start();
		Node node = pq.top();
		//std::cout<<node.idx<<" "<<node.deg<<" "<<pq.size()<<std::endl;
		pq.pop();
		pqt.stop();
		if(D[node.idx] != node.deg) continue; //old values
		max_alpha = std::max(max_alpha, node.deg);

		std::vector<uintE> deleteV;
		nghCount(adjG, D, node.idx, beta, deleteV);
		std::vector<uintE> changeVtx = nghCount(adjG, D, deleteV, tracker, ++iter, max_alpha);
		for(uintE i : changeVtx) pq.push(Node(i, std::max(max_alpha,D[i])));
		rho_beta++;
	}
	std::cout<<rho_beta << " "<<max_alpha<<std::endl;
	return pt.get_total();
}

}