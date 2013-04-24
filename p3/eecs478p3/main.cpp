#include "circuit.h"
#include <math.h>

#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

void usage(const char* exename);
void parseinput(const char* filename, vector<vector<string> > &inputs);
void printoutput(const vector<vector<string> > &outputs);

int main(int argc, char **argv)
{
  // parsing inputs
  if (argc < 2)
  {
    usage(argv[0]);
  }
  
  for (int i = 1; i < argc; ++i)
  {
    if (argv[i] == string("-h") || argv[i] == string("-help"))
    {
      usage(argv[0]);
    }
    
    else if (argv[i] == string("-topoSort"))
    {
      if (i + 1 < argc)
      {
        string inFilename = string(argv[++i]);
        Circuit c(inFilename);

        // your code here
        vector<string> order;

        c.topologicalSort(order);

        cout << "*** Topological order:" << endl;
        for (unsigned int i = 0; i < order.size(); i++)
            cout << order[i] << " ";
        cout << endl;
      }
      else
      {
        cout << "option -topoSort requires an additional argument." << endl;
        usage(argv[0]);
      }
    }
    else if (argv[i] == string("-simulate"))
    {
      if (i + 2 < argc)
      {
        string inFilename = string(argv[++i]);
        Circuit c(inFilename);
        string inputFile(argv[++i]);

        // your code here
        vector<vector<string> > init_values;
        vector<vector<string> > out_values;

        parseinput(inputFile.c_str(), init_values);
        c.simulate(init_values, out_values);
        printoutput(out_values);
      }
      else
      {
        cout << "option -simulate requires two additional arguments." << endl;
        usage(argv[0]);
      }
    }
    
  }
  
  return 0;
}

void usage(const char* exename)
{
  cout << "Usage: " << exename << " <options> " << endl;
  cout << "-h or -help                     prints out this help message. " << endl;
  cout << "-topoSort <inFile>              prints a topological ordering of the circuit in <inFile>. " << endl;
  cout << "-simulate <inFile> <inputs>     simulates the circuit in <inFile> with the inputs in <inputs>." << endl;
  cout << endl;
  
  exit(0);
}


void parseinput(const char* filename, vector<vector<string> > &inputs)
{
    ifstream infile(filename);

    string name, value;
    while (infile >> name >> value) {
        vector<string> pair;
        pair.push_back(name);
        pair.push_back(value);
        inputs.push_back(pair);
    }
}


void printoutput(const vector<vector<string> > &out_values)
{
    cout << "*** Outputs:" << endl;

    unsigned int i;
    for (i = 0; i < out_values.size()-1; i++) {
        const vector<string> &pair = out_values[i];
        assert(pair.size() == 2);
        cout << pair[0] << " = " << pair[1] << ", ";
    }

    // last one does not pring comma after
    const vector<string> &pair = out_values[i];
    cout << pair[0] << " = " << pair[1];

    cout << endl;
}


