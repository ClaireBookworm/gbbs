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
	uintE other;
	std::list<Edge>::iterator oiter;
	Edge(uintE o, std::list<Edge>::iterator it) : other(o), oiter(it) {}
}
struct Node{
	uintE idx;
	uintE deg;
	Node(uintE idx_, uintE deg_) : idx(idx_), deg(deg_) {}
	bool operator<(const Node& other) const{
		return deg < other.deg;
	}
	bool operator>(const Node& other) const{
		return deg > other.deg;
	}
}
template <class Graph>
inline void BiCore_serial(Graph &G, size_t num_buckets = 16, size_t bipartition = 2, size_t peel_core_alpha = 0, size_t peel_core_beta = 0)
{
	std::cout << "starting" << std::endl;
	const size_t n = G.n;					// # of vertices
	const size_t n_a = bipartition + 1;		// number of vertices in first partition
	const size_t n_b = n - bipartition - 1; // number of vertices in second partition
	std::list<Edge>* adjG = new std::list<Edge>[n];
	for(size_t i=0; i<n; i++){
		auto neighbors = G.get_vertex(i).out_neighbors();
		for (size_t j = 0; j < neighbors.degree; j++){
			uintE other = neighbors.get_neighbor(j);
			if(other<=i) continue; // don't support self-loop
			auto oEnd = adjG[other].end();
			auto iEnd = adjG[i].end();
			adjG[i].push_back(Edge(other, oEnd));
			adjG[other].push_back(Edge(i, iEnd));
		}
	}

}

inline void nghCount(std::list<Edge>* adjG, uintE vtx, size_t cutoff, std::vector<uintE>& delList){
	//everything less than cutoff is deleted
	list<Edge>& neighbors = adjG[vtx];
	for (auto it = neighbors.begin(); it != neighbors.end(); it++){
		uintE id = it->other;
		if(adjG[id].size()==cutoff)
			delList.push_back(id);
		adjG[id].erase(it->oiter);
	}
	neighbors.clear();
}

inline unordered_set<uintE> nghCount(std::list<Edge>* adjG, std::vector<uintE>& del, size_t cutoff){
	//everything less than cutoff is deleted
	unordered_set<uintE> changeVtx;
	for(uintE vtx : del){
		list<Edge>& neighbors = adjG[vtx];
		for (auto it = neighbors.begin(); it != neighbors.end(); it++){
			uintE id = it->other;
			changeVtx.add(id);
			adjG[id].erase(it->oiter);
		}
		neighbors.clear();
	}
	return changeVtx;
}

inline std::pair<size_t, size_t> PeelFixA(std::list<Edge>* adjG, size_t alpha, size_t n_a, size_t n_b)
{
	const size_t n = n_a + n_b;

	size_t finished = 0, rho_alpha = 0, max_beta = 0;

	std::vector<uintE> uDel;
	for (size_t i = 0; i < n_a; i++)
		if (adjG[i].size() < alpha)
			uDel.push_back(i);
	// (alpha,0)
	// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
	while (uDel.size()>0)
	{
		// everything less than cutoff is deleted
		std::vector<uintE> vDel;
		for(uintE del : uDel)
			nghCount(adjG, del, 1, vDel);
		uDel.clear();
		for(uintE del : vDel)
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
	using PQNode = std::priority_queue<Node, std::vector<Node>, std::greater<Node> >;
	PQNode pq(remnants.begin(), remnants.end());
	while (finished != vCount)
	{
		Node node = pq.top();
		pq.pop();
		if(adjG[node.idx].size() != node.deg) continue; //old values
		max_beta = std::max(max_beta, node.deg);

		std::vector<uintE> deleteU;
		nghCount(adjG, node.idx, alpha, deleteU);
		for(Edge e : adjG[node.idx])
			pq.push(Node(e.other, adjG[e].size()));
		unordered_set<uintE> changeVtx = nghCount(adjG, deleteU, max_beta)
		for(uintE i : changeVtx)
			pq.push(Node(i, adjG[i].size()));
		rho_alpha++;
	}
	return std::pair<size_t, size_t>(rho_alpha, max_beta);
}
/*
  	uncompressed_neighbors<W> in_neighbors() {
    	return uncompressed_neighbors<W>(id, degree, neighbors); }
  	uncompressed_neighbors<W> out_neighbors() { return in_neighbors(); }
	*/
// The classes have two templates, W and C:
// W : the weight type of the underlying graph (pbbslib::empty if unweighted)
// C : the compression format used. See encodings/decoders.h.
}