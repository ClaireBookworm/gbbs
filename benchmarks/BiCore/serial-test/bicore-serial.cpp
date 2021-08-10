#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

std::pair<int, int> getPairs(std::string fileName)
{
	std::ifstream fin(fileName);
	std::string graphType;
	fin >> graphType;
	int u, w;
	int sum = 0;

	auto p = new std::pair<int, int>[u + w];
	// std::pair<int, int> p = new std::pair[u + w];
	fin >> u >> w; // sizes of bipartite node groups
	int **all = new int *[u + w];
	int uGraph[u];
	for (int i = 0; i < u; i++)
	{
		fin >> uGraph[i];
		sum += uGraph[i] > 1 ? uGraph[i] : 1;
	}
	int wGraph[w];
	for (int i = 0; i < w; i++)
	{
		fin >> wGraph[i];
	}
	std::sort(uGraph, uGraph + u, std::greater<int>());
	std::sort(wGraph, wGraph + w, std::greater<int>());

	std::copy(wGraph, wGraph + w, std::copy(uGraph, uGraph + u, *all));

	// sorts correctly
	int pairCount = 0;
	for (int i = 0; i < u; i++)
	{
		// p[pairCount].first = uGraph[i];
		// int topVal = uGraph[i];
		// std::cout << "VALUE OF U: " << uGraph[i] << std::endl;
		for (int j = 0; j < w; j++)
		{
			if (wGraph[j] > 0)
			{
				p[pairCount].first = *all[i];
				p[pairCount].second = *all[u + j];
				// std::cout << "looking at w: " << wGraph[j] << std::endl;
				wGraph[j]--;
				uGraph[i]--;
				pairCount++;
				// continue;
			}
			else
			{
				continue;
			}
			if (uGraph[i] <= 0)
			{
				break;
			}
			// std::cout << "back at u: " << uGraph[i] << std::endl;
		}
		// std::cout << "END OF U: " << uGraph[i] << std::endl;
	}
	fin.close();
	// std::ofstream fout;
	// fout.open("test-graph.out"); // for some reason this keeps giving segmentation fault
	for (int i = 0; i < (sum); i++)
	{
		std::cout << p[i].first << ", " << p[i].second << std::endl;
	}
	return *p;
}

// use max alpha and beta
template <class Graph>
inline void BiCore(Graph &G, size_t bipartition = 2, size_t peel_core_alpha = 0, size_t peel_core_beta = 0)
{
	std::cout << "starting" << std::endl;
	const size_t n = G.n;					// # of vertices
	const size_t n_b = n - bipartition - 1; // number of vertices in first partition
	const size_t n_a = bipartition + 1;		// number of vertices in second partition

	// alphamax is max degree in first partition
	auto AlphaMax = sequence<sequence<size_t>>(n_b, [&G, &n_a](size_t i){ return sequence<size_t>(1+G.get_vertex(i+n_a).out_degree(),[](size_t i){return 0;}); });
	// BetaMax[u][A]
	auto BetaMax = sequence<sequence<size_t>>(n_a, [&G](size_t i){ return sequence<size_t>(1+G.get_vertex(i).out_degree(),[](size_t i){return 0;}); });

	for (v : G.vertices()) {
		if (v.get_degree < AlphaMax) {
			G.remove(v);
		}
	}
}

