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
struct Edge{
	size_t other;
	std::list<Edge>::iterator oiter;
	Edge(size_t o) : other(o) {}
};
struct Node{
	size_t idx;
	size_t deg;
	Node(size_t idx_, size_t deg_) : idx(idx_), deg(deg_) {}
	bool operator<(const Node& other) const{
		return deg < other.deg;
	}
	bool operator>(const Node& other) const{
		return deg > other.deg;
	}
};

inline double PeelFixA(std::vector<std::list<Edge> >& adjG, size_t alpha, size_t n_a, size_t n_b);
inline double PeelFixB(std::vector<std::list<Edge> >& adjG, size_t alpha, size_t n_a, size_t n_b);

template <class Graph>
inline std::vector<std::list<Edge> > make_graph(Graph& G){
	const size_t n = G.n;
	std::vector<std::list<Edge> > adjG(n);
	for(size_t i=0; i<n; i++){
		auto neighbors = G.get_vertex(i).out_neighbors();
		for (size_t j = 0; j < neighbors.degree; j++){
			size_t other = neighbors.get_neighbor(j);
			if(other<=i) continue; // don't support self-loop
			adjG[i].push_back(Edge(other));
			adjG[other].push_back(Edge(i));
			auto iEnd = adjG[i].end(); iEnd--;
			auto oEnd = adjG[other].end(); oEnd--;
			adjG[i].back().oiter = oEnd;
			adjG[other].back().oiter = iEnd;
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
	timer mkt;
	std::cout<<"finished preprocessing"<<std::endl;
	for(size_t core = 1; core<=delta; core++){
		std::cout<<"running PeelFixA core "<<core<<std::endl;
		mkt.start();
		std::vector<std::list<Edge> > adjG = make_graph(G);
		mkt.stop();
		pqt += PeelFixA(adjG, core, n_a, n_b);
	}
	for(size_t core = 1; core<=delta; core++){
		std::cout<<"running PeelFixB core "<<core<<std::endl;
		mkt.start();
		std::vector<std::list<Edge> > adjG = make_graph(G);
		mkt.stop();
		pqt += PeelFixB(adjG, core, n_a, n_b);
	}
	std::cout<<"pq time "<<pqt<<std::endl;
	mkt.reportTotal("make_graph time");
}

inline void nghCount(std::vector<std::list<Edge> >& adjG, size_t vtx, size_t cutoff, std::vector<size_t>& delList){
	//everything less than cutoff is deleted
	std::list<Edge>& neighbors = adjG[vtx];
	for (auto it = neighbors.begin(); it != neighbors.end(); it++){
		size_t id = it->other;
		if(adjG[id].size()==cutoff)
			delList.push_back(id);
		adjG[id].erase(it->oiter);
	}
	neighbors.clear();
}

inline std::unordered_set<size_t> nghCount(std::vector<std::list<Edge> >& adjG, std::vector<size_t>& del, size_t cutoff){
	//everything less than cutoff is deleted
	std::unordered_set<size_t> changeVtx;
	for(size_t vtx : del){
		std::list<Edge>& neighbors = adjG[vtx];
		for (auto it = neighbors.begin(); it != neighbors.end(); it++){
			size_t id = it->other;
			changeVtx.insert(id);
			adjG[id].erase(it->oiter);
		}
		neighbors.clear();
	}
	return changeVtx;
}

inline double PeelFixA(std::vector<std::list<Edge> >& adjG, size_t alpha, size_t n_a, size_t n_b)
{
	const size_t n = n_a + n_b;
	timer pqt;
	size_t rho_alpha = 0, max_beta = 0;

	std::vector<size_t> uDel;
	for (size_t i = 0; i < n_a; i++)
		if (adjG[i].size() < alpha)
			uDel.push_back(i);
	// (alpha,0)
	// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
	while (uDel.size()>0)
	{
		// everything less than cutoff is deleted
		//std::cout<<uDel.size()<<std::endl;
		std::vector<size_t> vDel;
		for(size_t del : uDel)
			nghCount(adjG, del, 1, vDel);
		uDel.clear();
		for(size_t del : vDel)
			nghCount(adjG, del, alpha, uDel);
	}

	size_t vCount = 0;
	std::vector<Node> remnants;
	for(size_t i=n_a; i<n; i++){
		if(adjG[i].size()>0){
			vCount++;
			remnants.push_back(Node(i,adjG[i].size()));
		}
	}
	pqt.start();
	using PQNode = std::priority_queue<Node, std::vector<Node>, std::greater<Node> >;
	PQNode pq(remnants.begin(), remnants.end());
	pqt.stop();
	while (!pq.empty())
	{
		pqt.start();
		Node node = pq.top();
		//std::cout<<node.idx<<" "<<node.deg<<" "<<pq.size()<<std::endl;
		pq.pop();
		pqt.stop();
		if(adjG[node.idx].size() != node.deg) continue; //old values
		max_beta = std::max(max_beta, node.deg);

		std::vector<size_t> deleteU;
		nghCount(adjG, node.idx, alpha, deleteU);
		for(Edge e : adjG[node.idx])
			if(adjG[e.other].size()>=alpha)
				pq.push(Node(e.other, adjG[e.other].size()));
		std::unordered_set<size_t> changeVtx = nghCount(adjG, deleteU, max_beta);
		for(size_t i : changeVtx)
			if(adjG[i].size()>0)
				pq.push(Node(i, adjG[i].size()));
		rho_alpha++;
	}
	std::cout<<rho_alpha << " "<<max_beta<<std::endl;
	return pqt.get_total();
}

inline double PeelFixB(std::vector<std::list<Edge> >& adjG, size_t beta, size_t n_a, size_t n_b)
{
	const size_t n = n_a + n_b;
	timer pqt;

	size_t rho_beta = 0, max_alpha = 0;

	std::vector<size_t> vDel;
	for (size_t i = n_a; i < n; i++)
		if (adjG[i].size() < beta)
			vDel.push_back(i);
	// (alpha,0)
	// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
	while (vDel.size()>0)
	{
		// everything less than cutoff is deleted
		std::vector<size_t> uDel;
		for(size_t del : vDel)
			nghCount(adjG, del, 1, uDel);
		vDel.clear();
		for(size_t del : uDel)
			nghCount(adjG, del, beta, vDel);
	}

	size_t uCount = 0;
	std::vector<Node> remnants;
	for(size_t i=0; i<n_a; i++){
		if(adjG[i].size()>0){
			uCount++;
			remnants.push_back(Node(i,adjG[i].size()));
		}
	}
	pqt.start();
	using PQNode = std::priority_queue<Node, std::vector<Node>, std::greater<Node> >;
	PQNode pq(remnants.begin(), remnants.end());
	pqt.stop();
	while (!pq.empty())
	{
		pqt.start();
		Node node = pq.top();
		//std::cout<<node.idx<<" "<<node.deg<<" "<<pq.size()<<std::endl;
		pq.pop();
		pqt.stop();
		if(adjG[node.idx].size() != node.deg) continue; //old values
		max_alpha = std::max(max_alpha, node.deg);

		std::vector<size_t> deleteV;
		nghCount(adjG, node.idx, beta, deleteV);
		for(Edge e : adjG[node.idx])
			if(adjG[e.other].size()>=beta)
				pq.push(Node(e.other, adjG[e.other].size()));
		std::unordered_set<size_t> changeVtx = nghCount(adjG, deleteV, max_alpha);
		for(size_t i : changeVtx)
			if(adjG[i].size()>0)
				pq.push(Node(i, adjG[i].size()));
		rho_beta++;
	}
	std::cout<<rho_beta << " "<<max_alpha<<std::endl;
	return pqt.get_total();
}

}