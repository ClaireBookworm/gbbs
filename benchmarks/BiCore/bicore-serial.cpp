#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

int main() {
	std::ifstream fin ("graph.txt");
	std::string graphType;
	fin >> graphType;
	int u, w;
	
	std::pair <int, int> p [u+w];
	fin >> u >> w; // sizes of bipartite node groups
	int uGraph [u];
	for (int i = 0; i < u; i++) {
		fin >> uGraph[i];
	}
	int wGraph [w];
		for (int i = 0; i < w; i++) {
		fin >> wGraph[i];
	}
	std::sort(uGraph, uGraph + u, std::greater<int>());
	std::sort(wGraph, wGraph + w, std::greater<int>());
	// sorts correctly 
	int pairCount = 0;
	for (int i = 0; i < (u); i++) {
		p[pairCount].first = uGraph[i];
		std::cout << "VALUE OF U: " << uGraph[i] << std::endl;
		for (int j = 0; j < w; j++) {
			std::cout << "looking at w: " << wGraph[j] << std::endl;
			if (wGraph[j] > 0) {
				p[pairCount].second = wGraph[j];
				// std::cout << "looking at w: " << wGraph[j] << std::endl;
				wGraph[j]--;
				uGraph[i]--;
				pairCount++;
				// continue;
			}
			if (uGraph[i] <= 0) {
				continue;
			}
			// std::cout << "back at u: " << uGraph[i] << std::endl;
		}
		std::cout << "END OF U: " << uGraph[i] << std::endl;
	}
	for(int i = 0; i < (u+w); i++) {
     std::cout << p[i].first << ", " << p[i].second << std::endl;
}
}
