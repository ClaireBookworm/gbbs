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

// std::pair<int, int> getPairs(std::string fileName)
// {
	// 	std::ifstream fin(fileName);
	// 	std::string graphType;
	// 	fin >> graphType;
	// 	int u, w;
	// 	int sum = 0;

	// 	auto p = new std::pair<int, int>[u + w];
	// 	// std::pair<int, int> p = new std::pair[u + w];
	// 	fin >> u >> w; // sizes of bipartite node groups
	// 	int **all = new int *[u + w];
	// 	int uGraph[u];
	// 	for (int i = 0; i < u; i++)
	// 	{
	// 		fin >> uGraph[i];
	// 		sum += uGraph[i] > 1 ? uGraph[i] : 1;
	// 	}
	// 	int wGraph[w];
	// 	for (int i = 0; i < w; i++)
	// 	{
	// 		fin >> wGraph[i];
	// 	}
	// 	std::sort(uGraph, uGraph + u, std::greater<int>());
	// 	std::sort(wGraph, wGraph + w, std::greater<int>());

	// 	std::copy(wGraph, wGraph + w, std::copy(uGraph, uGraph + u, *all));

	// 	// sorts correctly
	// 	int pairCount = 0;
	// 	for (int i = 0; i < u; i++)
	// 	{
	// 		// p[pairCount].first = uGraph[i];
	// 		// int topVal = uGraph[i];
	// 		// std::cout << "VALUE OF U: " << uGraph[i] << std::endl;
	// 		for (int j = 0; j < w; j++)
	// 		{
	// 			if (wGraph[j] > 0)
	// 			{
	// 				p[pairCount].first = *all[i];
	// 				p[pairCount].second = *all[u + j];
	// 				// std::cout << "looking at w: " << wGraph[j] << std::endl;
	// 				wGraph[j]--;
	// 				uGraph[i]--;
	// 				pairCount++;
	// 				// continue;
	// 			}
	// 			else
	// 			{
	// 				continue;
	// 			}
	// 			if (uGraph[i] <= 0)
	// 			{
	// 				break;
	// 			}
	// 			// std::cout << "back at u: " << uGraph[i] << std::endl;
	// 		}
	// 		// std::cout << "END OF U: " << uGraph[i] << std::endl;
	// 	}
	// 	fin.close();
	// 	// std::ofstream fout;
	// 	// fout.open("test-graph.out"); // for some reason this keeps giving segmentation fault
	// 	for (int i = 0; i < (sum); i++)
	// 	{
	// 		std::cout << p[i].first << ", " << p[i].second << std::endl;
	// 	}
	// 	return *p;
// }
namespace gbbs{

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

// use max alpha and beta
template <class Graph> 
inline void BiCore_serial(Graph &G, size_t num_buckets = 16, size_t bipartition = 2, size_t peel_core_alpha = 0, size_t peel_core_beta = 0)
{
	std::cout << "starting" << std::endl;
	const size_t n = G.n;					// # of vertices
	const size_t n_a = bipartition + 1;		// number of vertices in first partition
	const size_t n_b = n - bipartition - 1; // number of vertices in second partition

	// alphamax is max degree in first partition
	// std::vector<size_t> AlphaMax (n_b, [&G, &n_a](size_t i){(1 + G.get_vertex(i + n_a).out_degree(), [](size_t i) { return 0; }); });
	auto degreeA = [&G, &n_a](size_t i){return (1 + G.get_vertex(i + n_a).out_degree()); };
	auto maxA = 0;
	for (int i = 0; i < n_a; i++) {
		maxA = maxA < degreeA(i) ? degreeA(i) : maxA;
	}
	std::vector<size_t> AlphaMax;
	AlphaMax.push_back(n_b);
	AlphaMax.push_back(maxA);
	// BetaMax[u][A
	auto degreeB = [&G](size_t i) { return (1 + G.get_vertex(i).out_degree()); };
	std::vector<size_t> BetaMax;
	int maxB = 0;
	BetaMax.push_back(n_a);
		for (int i = 0; i < n_a; i++) {
		maxB = maxB < degreeB(i) ? degreeB(i) : maxB;
	}
	BetaMax.push_back(maxB);


}

template <class Graph>
inline std::vector<uintE> nghCount(Graph &G, std::vector<uintE> del, std::vector<uintE> D, size_t cutoff)
{
	//everything less than cutoff is deleted
	std::unordered_set<uintE> delOther;
	for (size_t i = 0; i < del.size(); i++)
	{
		uintE vtx = del[i];
		auto D_n = G.get_vertex(vtx);
		// out_neighbors gives the id, degree, and neighbors in tuple form
		auto neighbors = D_n.out_neighbors();
		for (int j = 0; j < neighbors.degree; j++)
		{
			uintE id = neighbors.get_neighbor(j);
			if(D[id]>=cutoff){
				D[id]--;
				delOther.insert(id);
			}
		}
	}
	std::vector<uintE> delOtherF;
	for(uintE id : delOther)
		if(D[id]<cutoff) delOtherF.push_back(id);
	return delOtherF;
}

// template <class Graph>
// inline std::pair<std::pair<size_t, size_t>, double> PeelFixB(Graph &G, std::vector<size_t> &BetaMax, std::vector<size_t> &AlphaMax, size_t alpha, size_t bipartition = 2, size_t num_buckets  = 16) {

// }

template <class Graph>
inline std::pair<size_t, size_t> PeelFixA(Graph &G, std::vector<size_t> &BetaMax,std::vector<size_t> &AlphaMax, size_t alpha, size_t bipartition = 2, size_t num_buckets = 16)
{
	const size_t n = G.n;
	const size_t n_b = n - bipartition - 1;
	const size_t n_a = bipartition + 1;

	size_t finished = 0, rho_alpha = 0, max_beta = 0;
	// [0, bipartition] interval for U
	// [bipartition+1, n-1]  interval V
	std::vector<uintE> D;
	for (size_t i = 0; i < n; i++)
		D.push_back(G.get_vertex(i).out_degree());
	std::vector<uintE> uDel;
	for (size_t i = 0; i < n_a; i++)
		if (D[i] < alpha)
			uDel.push_back(i);
	// (alpha,0)
	// peels all vertices in U which are < alpha, and repeatedly peels vertices in V which has deg == 0
	while (uDel.size()>0)
	{
		// everything less than cutoff is deleted
		std::vector<uintE> vDel = nghCount(G, uDel, D, 1);
		uDel = nghCount(G, vDel, D, alpha);
	}

	size_t vCount = 0;

	std::vector<uintE> vD = (n, [&](size_t i)
							 {
								 if (i <= bipartition || D[i] == 0)
									 return std::numeric_limits<uintE>::max();
								 return D[i];
							 });
	vCount = pbbslib::reduce_add(std::vector<uintE>(n_b, [&](size_t i)
												 { return D[i + n_a] > 0; }));
	
	// using pq_graph = std::priority_queue<uintE, std::vector<uintE>, std::greater<uintE> >;
	using pq_graph = std::priority_queue<Node, std::vector<Node>, std::greater<Node> >;
	pq_graph pq(vD.begin(), vD.end());
	// while (finished != vCount)
	while (!pq.empty())
	{

		Node v = pq.top(); 
		auto vbkt = pq.top();
		// pq.pop();
		// std::cout << "pq.top: " << pq.top() << std::endl;
		max_beta = max_beta > vbkt.idx ? max_beta : vbkt.idx;
		if (uDel.at(0) == 0)
			continue;
		// auto activeV = vertexSubset(n, std::move(D.identifiers)); // container of vertices
		auto activeV = vertexSubset(n, std::move(D.at(0))); // container of vertices
		finished += activeV.size();

		for (size_t i = 0; i < activeV.size(); i++) {
			size_t index = activeV.vtx(i) - n_a;
				for(size_t j = 1; j < max_beta; j++){ 
					// AlphaMax.at(index)[j]=std::max(AlphaMax.at(index)[j],alpha); 
					AlphaMax.at(j)=std::max(AlphaMax.at(j),alpha); 
				}
		}
		vertexSubsetData deleteU = nghCount(G, activeV, D, alpha);
		// "deleteU" is a wrapper storing a sequence id of deleted vertices in U

		vertexSubsetData movedV = nghCount(G, deleteU, D, alpha);
		// "movedV" is a wrapper storing a sequence of tuples like (id, newBucket)
		// bbuckets.update_buckets(movedV);
		pq.pop();
		rho_alpha++;
	}
	// it.start();
	// bbuckets.del();
	// em.del();
	// it.stop();
	// debug(it.reportTotal("initialize time"));
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