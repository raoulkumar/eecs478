#include "circuit.h"
#include <vector>
#include <assert.h>
#include <iostream>

using namespace std;


int main(int argc, char **argv)
{
    Circuit c("my_adder2.blif");

    vector<vector<string> > init_values;
    vector<vector<string> > out_values;

    // setup input values
    //.inputs a[0] a[1] b[0] b[1] cin 
    vector<string> a;
    a.push_back("a[0]");
    a.push_back("1");
    init_values.push_back(a);

    a.clear();
    a.push_back("a[1]");
    a.push_back("0");
    init_values.push_back(a);

    vector<string> b;
    b.push_back("b[0]");
    b.push_back("1");
    init_values.push_back(b);

    b.clear();
    b.push_back("b[1]");
    b.push_back("0");
    init_values.push_back(b);

    vector<string> cin;
    cin.push_back("cin");
    cin.push_back("1");
    init_values.push_back(cin);

    c.simulate(init_values, out_values);

    // print
    for (unsigned int i = 0; i < out_values.size(); i++) {
        vector<string> &pair = out_values[i];
        assert(pair.size() == 2);
        cout << pair[0] << " = " << pair[1] << ", ";
    }
    cout << endl;
}

