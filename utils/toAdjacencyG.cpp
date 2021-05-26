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
11
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
// Driver function to sort the vector elements
// by second element of pairs
bool sortbysec(const pair<int, int> &a,
               const pair<int, int> &b)
{
    return (a.second < b.second);
}

int main()
{
    string temp;
    ifstream infile;
    ofstream outfile;
    int numVertexA, numVertexB, numEdges;
    int vertexOffset = -1;
    int vertexANumber;
    int prevVertexA = -1;
    int prevVertexB = -1;
    int vertexBNumber;
    vector<int> offset;
    vector<int> offsetB;
    vector<int> edges;
    vector<int> edgesB;
    vector<pair<int, int> > edgePair;
    infile.open("../inputs/record_konect");
    infile.ignore(256, '\n');
    outfile.open("../inputs/record_adj");
    infile >> temp >> numEdges >> numVertexA >> numVertexB;
    for (int i = 0; i < numEdges; i++)
    {
        infile >> vertexANumber >> vertexBNumber;
        edgePair.push_back(make_pair(vertexANumber, vertexBNumber));
        if (vertexANumber != prevVertexA)
        {
            vertexOffset++;
            prevVertexA = vertexANumber;
            offset.push_back(vertexOffset);
        }
        else
        {
            vertexOffset++;
        }
        edges.push_back(vertexBNumber + numVertexA - 1);
    }
    sort(edgePair.begin(), edgePair.end(), sortbysec);
    for (int i = 0; i < numEdges; i++)
    {
        vertexBNumber = edgePair.at(i).second;
        if (vertexBNumber != prevVertexB)
        {
            vertexOffset++;
            prevVertexB = vertexBNumber;
            offset.push_back(vertexOffset);
        }
        else
        {
            vertexOffset++;
        }
        edges.push_back(edgePair.at(i).first - 1);
    }
    cout << "AdjacencyGraph\n";
    outfile << "AdjacencyGraph\n";
    cout << numVertexA + numVertexB << endl;
    outfile << numVertexA + numVertexB << endl;
    cout << numEdges * 2 << endl;
    outfile << numEdges * 2 << endl;
    for (int i = 0; i < offset.size(); i++)
    {
        cout << offset.at(i) << endl;
        outfile << offset.at(i) << endl;
    }
    for (int i = 0; i < edges.size(); i++)
    {
        cout << edges.at(i) << endl;
        outfile << edges.at(i) << endl;
    }
    infile.close();
    outfile.close();
}