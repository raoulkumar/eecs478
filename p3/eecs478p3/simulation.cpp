#include "circuit.h"

using namespace std;


int main(int argc, char **argv)
{
    Circuit c("my_and.blif");

    c.findNode("a")->setSimValue(ONE);
    c.findNode("b")->setSimValue(ONE);
    c.findNode("c")->simulate();

    cout << c.findNode("c")->getSimValue() << endl;
}

