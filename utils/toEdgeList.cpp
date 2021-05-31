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

int main(){
    ifstream infile;
    string file_name = "flickr";
    infile.open("../inputs/"+file_name+"_konect");
    ofstream outfile;
    outfile.open("../inputs/"+file_name+"_edgelist");
    vector<int> edges;
    string skip1, skip2;
    int numEdge, numA, numB, first, second;
    infile.ignore(256, '\n');
    string temp;
    infile >> temp >> numEdge >> numA >> numB;

    for(int i=0;i<numEdge;i++){
        infile>>first>>second;
        first--;
        second--;
        second += numA;
        outfile << first <<" "<< second <<endl;
        outfile << second <<" "<< first <<endl;
    }
    infile.close();
    outfile.close();
    string command = "./snap_converter -i ../inputs/"+file_name+"_edgelist"+" -o "+"../inputs/"+file_name+"_adj";
    system(command.c_str());
}