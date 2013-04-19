#include "circuit.h"

int Circuit::createABSMIN5X3YModule(const string &input1, const string &input2, const string &output)
{
    unsigned int ibits = 16;
    unsigned int obits = ibits + 3;

    string name;
    Node* node;
    stringstream sstr;

    // create primary inputs and outputs
    for (unsigned int i = 0; i < ibits; ++i)
    {
        sstr.str("");
        sstr << i;

        name = input1 + "[" + sstr.str() + "]";
        node = createNode(name);

        name = input2 + "[" + sstr.str() + "]";
        node = createNode(name);
    }

    for (unsigned int i = 0; i < obits; ++i)
    {
        stringstream sstr;
        sstr << i;

        name = output + "[" + sstr.str() + "]";
        node = createNode(name);
    }


    // 5x = (x << 2) + x

    // create (x << 2)
    const string x_shift("xs2");
    createSHIFTModule(input1, x_shift, ibits, 2);

    // arithmetic extend (x << 2) and x to 19 bits
    arithmeticExtend(x_shift, ibits + 2, 1);
    arithmeticExtend(input1, ibits, 3);

    // compute (x << 2) + x into 5x
    node = findNode("ZERO");
    if (node == NULL) {
        node = createNode("ZERO");
        createZERONode(node);
    }
    node = createNode("not_used");

    const string x_mult("5x");
    createADDModule(input1, x_shift, "ZERO", x_mult, "not_used", obits);


    // 3y = (y << 1) + y
    const string y_shift("ys1");
    createSHIFTModule(input2, y_shift, ibits, 1);

    // arithmetic extend (y << 1) and y to 19 bits
    arithmeticExtend(y_shift, ibits + 1, 2);
    arithmeticExtend(input2, ibits, 3);

    // compute (y << 1) + y into 3y
    node = findNode("ZERO");
    if (node == NULL) {
        node = createNode("ZERO");
        createZERONode(node);
    }
    node = createNode("not_used");

    const string y_mult("3y");
    createADDModule(input2, y_shift, "ZERO", y_mult, "not_used", obits);


    // Compute min(5x, 3y)
    const string min("min");
    createMINModule(x_mult, y_mult, min, obits);


    // Compute abs(min)
    createABSModule(min, output, obits);

    return 0;
}



