#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <stack>
#include <queue>
#include <set>

using namespace std;

#include "node.h"

typedef map<string,Node*>::iterator mapIter;
class Circuit
{
  private:
    map<string,Node*> nodeMap;
    string name;
    
  public:
    // Added by me.
    // Perform simulation of the circuit, given the input values as specified in
    // the vector init_values. The values for the input nodes that are not
    // specified in the input vector are undetermined.
    // The simulated results are pushed bach to out_values. The user of this
    // method must create the output vector, and pass it to the function. The
    // method does not perform any cleaning beforing inserting values into the
    // vector, so caution may be needed.
    void simulate(vector<vector<string> > &init_values, vector<vector<string> > &out_values);

    // nodes names that are topologically sorted are pushed bach into the vector
    // 'order'. The method does NOT perform any cleaning before inserting the
    // names.
    void topologicalSort(vector<string> &order);
    void topologicalVisit(set<Node*> &nodes, Node* n, vector<string> &order);


    // constructors
    Circuit() {};
    
    Circuit(const string &filename)
    { if (readBLIF(filename)) cout << "error in reading " << filename << endl; }
    
    // destructor
    ~Circuit();
    
    // get number of nodes in the circuit
    unsigned getNumNodes() { return nodeMap.size(); }
    
    // get and set the name of the circuit
    string getName() { return name; }
    int setName(const string &modelName) { name = modelName; return 0; }
    
    // get primary inputs and outputs
    vector<Node*> getPIs();
    vector<Node*> getPOs();
    
    // sets the node of <string> as primary input and output
    int setPI(const string &input);
    int setPO(const string &output);
    
    // set (assign) nodes in specific format as primary inputs and outputs from [startBit,endBit)
    // for a given string <string> and numBits, format is <string>[startBit] ... <string[endBit-1]
    int setPIs(const string &input, unsigned int startBit, unsigned int endBit);
    int setPOs(const string &output, unsigned int startBit, unsigned int endBit);
    
    // check to see if a node is there
    Node* findNode(const string &nodeName);
    
    // create an empty node - will return NULL if node already exists
    Node* createNode(const string &nodeName);
    
    // prints out the contents of the circuit
    int print();
    
    // reads in circuit from BLIF format
    int readBLIF(const string &filename);
    
    // writes circuit in BLIF format
    int writeBLIF(const string &filename);
    
    // erases the contents of the entire circuit
    int clear();
    

    
};

#endif

