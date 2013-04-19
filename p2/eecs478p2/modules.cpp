#include "circuit.h"

int Circuit::createADDModule(const string &input1, const string &input2,
        const string &cin,
        const string &output, const string &cout, unsigned int numBits)
{
    Node* node;
    string name;

    // create input nodes
    for (unsigned int i = 0; i < numBits; ++i)
    {
        stringstream sstr;
        sstr << i;

        name = input1 + "[" + sstr.str() + "]";
        node = createNode(name);

        name = input2 + "[" + sstr.str() + "]";
        node = createNode(name);
    }

    name = cin;
    node = createNode(name);

    // create output nodes
    for (unsigned int i = 0; i < numBits; ++i)
    {
        stringstream sstr;
        sstr << i;
        name = output + "[" + sstr.str() + "]";

        node = createNode(name);
    }

    name = cout;
    node = createNode(name);

    // Wire them.

    // create internal nodes, stores carries. Last one is buffered to carry-out.
    string internal("c_" + input1 + input2);
    for (unsigned int i = 0; i < numBits; ++i)
    {
        stringstream sstr;
        sstr << i;
        name = internal + "[" + sstr.str() + "]";

        node = createNode(name);
        node->type = INTERNAL;
    }


    // We start to build a ripple-carry adder.
    // I know it's slow, but it doesn't have to be fast.

    Node* nextCin = findNode(cin);
    assert(nextCin != NULL);

    for (unsigned int i = 0; i < numBits; i++)
    {
        // for i, we add i-th inputs, and produce i-th output with a carry out.
        stringstream sstr;
        sstr << i;
        string inname1 = input1 + "[" + sstr.str() + "]";
        string inname2 = input2 + "[" + sstr.str() + "]";
        string outname = output + "[" + sstr.str() + "]";
        string coutname = internal + "[" + sstr.str() + "]";

        Node* inNode1 = findNode(inname1);
        Node* inNode2 = findNode(inname2);
        Node* sumNode = findNode(outname);
        Node* coutNode = findNode(coutname);

        assert(inNode1 != NULL);
        assert(inNode2 != NULL);
        assert(sumNode != NULL);
        assert(coutNode != NULL);

        createFullAdder(inNode1, inNode2, nextCin, sumNode, coutNode);

        // set the carry in for the next step.
        nextCin = coutNode;
    }

    // Cout output is the last carry-out
    Node* coutNode = findNode(cout);
    assert(coutNode != NULL);
    createBUF1Node(nextCin, coutNode);

    return 0;
}

int Circuit::createSUBModule(const string &input1, const string &input2, const string &output, unsigned int numBits)
{
    Node* node;
    string name;

    // create input nodes
    for (unsigned int i = 0; i < numBits; ++i)
    {
        stringstream sstr;
        sstr << i;

        name = input1 + "[" + sstr.str() + "]";
        node = createNode(name);

        name = input2 + "[" + sstr.str() + "]";
        node = createNode(name);
    }

    node = createNode("ONE");
    createONENode(node);

    // create output nodes
    for (unsigned int i = 0; i < numBits; ++i)
    {
        stringstream sstr;
        sstr << i;
        name = output + "[" + sstr.str() + "]";

        node = createNode(name);
    }

    // create internal input inv_b by inverting 'b' or input2
    const string inv_name("inv_" + input2);
    for (unsigned int i = 0; i < numBits; ++i)
    {
        stringstream sstr;
        sstr << i;

        name = inv_name + "[" + sstr.str() + "]";
        node = createNode(name);
    }

    // Wire 'B's and 'InvB's.
    for (unsigned int i = 0; i < numBits; i++)
    {
        stringstream sstr;
        sstr << i;

        name = input2 + "[" + sstr.str() + "]";
        Node* original = findNode(name);
        assert(original != NULL);

        name = inv_name + "[" + sstr.str() + "]";
        Node* negated = findNode(name);
        assert(negated != NULL);

        createNOT1Node(original, negated);
    }

    // Wire them.
    string cout("not_used");
    createNode(cout);

    createADDModule(input1, inv_name, "ONE", output, cout, numBits);

    return 0;
}


int Circuit::createSHIFTModule(const string &input, const string &output, unsigned int numBits, unsigned int numShift)
{
    Node* node;
    // create input nodes
    for (unsigned int i = 0; i < numBits; ++i)
    {
        stringstream sstr;
        sstr << i;
        string name = input + "[" + sstr.str() + "]";

        node = createNode(name);
    }

    // create output nodes
    for (unsigned int i = 0; i < numBits+numShift; ++i)
    {
        stringstream sstr;
        sstr << i;
        string name = output + "[" + sstr.str() + "]";

        node = createNode(name);
    }

    // assign '0's to the least numShift bits
    Node* zeroNode = findNode("ZERO");
    if (zeroNode == NULL)
    {
        zeroNode = createNode("ZERO");
        createZERONode(zeroNode);
    }

    for (unsigned int i = 0; i < numShift; ++i)
    {
        stringstream sstr;
        sstr << i;
        string name = output + "[" + sstr.str() + "]";

        Node* outNode = findNode(name);
        assert(outNode != NULL);

        createBUF1Node(zeroNode, outNode);
    }

    // assign inputs to the remaining numBits bits
    for (unsigned int i = numShift; i < numBits+numShift; ++i)
    {
        string name;

        // find input node[i-numShift]
        stringstream inStr;
        inStr << i-numShift;
        name = input + "[" + inStr.str() + "]";
        Node* inNode = findNode(name);
        assert(inNode != NULL);

        // find output node[i]
        stringstream outStr;
        outStr << i;
        name = output + "[" + outStr.str() + "]";
        Node* outNode = findNode(name);
        assert(outNode != NULL);

        // assign
        createBUF1Node(inNode, outNode);
    }

    return 0;
}


int Circuit::arithmeticExtend(const string &input,
        unsigned int numBits, unsigned int extendBits)
{
    Node* node;
    string name;
    stringstream sstr;

    // create input and output nodes
    for (unsigned int i = 0; i < numBits + extendBits; ++i)
    {
        sstr.str("");
        sstr << i;
        name = input + "[" + sstr.str() + "]";

        node = createNode(name);
    }

    sstr.str("");
    sstr << numBits - 1;
    name = input + "[" + sstr.str() + "]";
    Node* msbNode = findNode(name);
    assert(msbNode != NULL);

    for (unsigned int i = numBits; i < numBits + extendBits; i++) {
        sstr.str("");
        sstr << i;
        name = input + "[" + sstr.str() + "]";

        node = findNode(name);
        assert(node != NULL);

        createBUF1Node(msbNode, node);
    }

    return 0;
}


int Circuit::createMINModule(const string &input1, const string &input2,
        const string &output, unsigned int numBits)
{

    Node* node;
    stringstream sstr;
    string name;

    // create input nodes
    for (unsigned int i = 0; i < numBits; ++i)
    {
        sstr.str("");
        sstr << i;
        name = input1 + "[" + sstr.str() + "]";
        node = createNode(name);

        name = input2 + "[" + sstr.str() + "]";
        node = createNode(name);
    }

    // create output nodes
    for (unsigned int i = 0; i < numBits; ++i)
    {
        sstr.str("");
        sstr << i;
        name = output + "[" + sstr.str() + "]";
        node = createNode(name);
    }

    // put two input values into n-bit subtractor
    const string comp("p_" + input1 + input2);
    createSUBModule(input1, input2, comp, numBits);

    // The MSB of the output from the subtractor is the select bit
    // If it's one, input2 is larger, and we should select input1
    // Otherwise, input1 is larger, and we should select input2
    sstr.str("");
    sstr << numBits - 1;
    name = comp + "[" + sstr.str() + "]";

    Node* select = findNode(name);
    assert(select != NULL);

    for (unsigned int i = 0; i < numBits; i++)
    {
        sstr.str("");
        sstr << i;

        name = input1 + "[" + sstr.str() + "]";
        Node* n1 = findNode(name);
        assert(n1 != NULL);

        name = input2 + "[" + sstr.str() + "]";
        Node* n2 = findNode(name);
        assert(n2 != NULL);

        name = output + "[" + sstr.str() + "]";
        Node* o = findNode(name);
        assert(o != NULL);

        createMUX2Node(select, n2, n1, o);
    }

    return 0;
}


int Circuit::createABSModule(const string &input, const string &output,
        unsigned int numBits)
{
    Node* node;
    stringstream sstr;
    string name;

    // create input nodes
    for (unsigned int i = 0; i < numBits; ++i)
    {
        sstr.str("");
        sstr << i;
        name = input + "[" + sstr.str() + "]";
        node = createNode(name);
    }

    // create output nodes
    for (unsigned int i = 0; i < numBits; ++i)
    {
        sstr.str("");
        sstr << i;
        name = output + "[" + sstr.str() + "]";
        node = createNode(name);
    }

    // create zeros nodes
    const string zeros("zeros");

    for (unsigned int i = 0; i < numBits; ++i)
    {
        sstr.str("");
        sstr << i;
        name = zeros + "[" + sstr.str() + "]";
        node = createNode(name);
        createZERONode(node);
    }

    // create two's complement to the input node
    const string comp("comp_" + input);

    createSUBModule(zeros, input, comp, numBits);

    // If MSB of the input is '0', we output as it is.
    // Otherwise, we output 2's complemented value.
    sstr.str("");
    sstr << numBits - 1;
    name = input + "[" + sstr.str() + "]";

    Node* select = findNode(name);
    assert(select != NULL);

    for (unsigned int i = 0; i < numBits; i++)
    {
        sstr.str("");
        sstr << i;

        name = input + "[" + sstr.str() + "]";
        Node* n = findNode(name);
        assert(n != NULL);

        name = comp + "[" + sstr.str() + "]";
        Node* c = findNode(name);
        assert(c != NULL);

        name = output + "[" + sstr.str() + "]";
        Node* o = findNode(name);
        assert(o != NULL);

        createMUX2Node(select, n, c, o);
    }

    return 0;
}



