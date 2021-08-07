#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

std::pair<int, int> getPairs(std::string fileName) {
	std::ifstream fin(fileName);
	std::string graphType;
	fin >> graphType;
	int u, w;
	int sum = 0;

	auto p = new std::pair<int, int>[u + w];
	// std::pair<int, int> p = new std::pair[u + w];
	fin >> u >> w;// sizes of bipartite node groups
	int** all = new int*[u+w];
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
		std::cout << "VALUE OF U: " << uGraph[i] << std::endl;
		for (int j = 0; j < w; j++) {
			if (wGraph[j] > 0)
			{
				p[pairCount].first = *all[i];
				p[pairCount].second = *all[u+j];
				// std::cout << "looking at w: " << wGraph[j] << std::endl;
				wGraph[j]--;
				uGraph[i]--;
				pairCount++;
				// continue;
			} else {
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

int main() {
	getPairs("test-graph.txt");
}
