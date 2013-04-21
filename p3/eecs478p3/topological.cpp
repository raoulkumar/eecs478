#include "circuit.h"
#include <vector>
#include <string>

using namespace std;


int main(int argc, char **argv)
{
    Circuit c("top_test.blif");

    vector<string> order;

    c.topologicalSort(order);

    for (unsigned int i = 0; i < order.size(); i++)
        cout << order[i] << " ";
    cout << endl;
}

