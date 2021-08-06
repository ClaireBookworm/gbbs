#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

int main()
{
	std::ifstream fin("test-graph.txt");
	std::string graphType;
	fin >> graphType;
	int u, w;
	int sum = 0;


	std::pair<int, int> p[u + w];
	fin >> u >> w; // sizes of bipartite node groups
	int all[u+w];
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

	std::copy(wGraph, wGraph + w, std::copy(uGraph, uGraph + u, all));

	// sorts correctly
	int pairCount = 0;
	for (int i = 0; i < u; i++)
	{
		// p[pairCount].first = uGraph[i];
		// int topVal = uGraph[i];
		std::cout << "VALUE OF U: " << uGraph[i] << std::endl;
		for (int j = 0; j < w; j++)
		{
			// p[pairCount].first = all[i];
			std::cout << "looking at w: " << wGraph[j] << std::endl;
			if (wGraph[j] > 0)
			{
				p[pairCount].first = all[i];
				p[pairCount].second = all[u+j];
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
	for (int i = 0; i < (sum); i++)
	{
		std::cout << p[i].first << ", " << p[i].second << std::endl;
	}
}
