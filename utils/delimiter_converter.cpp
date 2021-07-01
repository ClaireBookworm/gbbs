// converts delimiters from , to space
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
    string filename;
    cin>>filename;
    ifstream input("../inputs/"+filename);
    ofstream output;
    output.open("../inputs/"+filename+"_new");
    char c;
    while (input.get(c))
    {
        if(c==',')
            output << ' ';
        else
            output << c; 
    }
    output.close();
    return 0;
}