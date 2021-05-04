/*
convert bipartite graph input to adjancy graph input
For example
FROM
% bip unweighted
% 13 6 5
1 1 
1 2 
1 3 
2 1 
2 3 
3 4 
3 3 
4 1 
4 3 
5 5 
5 2 
6 1 
6 2 

TO
AdjacencyGraph
10
13
0
3
5
7
9
11
6
7
8
6
8
9
8
6
8
10
7
6
7
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    string temp;
    ifstream infile;
    ofstream outfile;
    int numVertexA, numVertexB, numEdges;
    int vertexOffset = -1;
    int vertexANumber;
    int prevVertexA = -1;
    int vertexBNumber;
    vector<int> offset;
    vector<int> edges;
    infile.open("input.txt");
    infile.ignore(256, '\n');
    outfile.open("adjData.txt");
    infile >> temp >> numEdges >> numVertexA >> numVertexB;
    for (int i=0; i<numEdges; i++) {
        infile >> vertexANumber >> vertexBNumber;
        if (vertexANumber != prevVertexA) {
            vertexOffset++;
            prevVertexA = vertexANumber;
            offset.push_back(vertexOffset);
        }
        else {
            vertexOffset++;
        }
        edges.push_back(vertexBNumber + numVertexA - 1);
    }
    cout << "AdjacencyGraph\n";
    outfile << "AdjacencyGraph\n";
    cout << numVertexA + numVertexB -1 << endl;
    outfile << numVertexA + numVertexB -1 << endl;
    cout << numEdges << endl;
    outfile << numEdges << endl;
    for (int i=0; i<offset.size(); i++) {
        cout << offset.at(i) << endl;
        outfile << offset.at(i) << endl;
    }
    for (int i=0; i<edges.size(); i++) {
        cout << edges.at(i) << endl;
        outfile << edges.at(i) << endl;
    }
    infile.close();
    outfile.close();
}