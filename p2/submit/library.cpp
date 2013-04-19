#include "circuit.h"

// creates a constant '1' node
int Circuit::createONENode(Node* node)
{
    assert(node != NULL);

    node->clearTT();
    node->clearFanin();

    node->type = ONE_NODE;
    return 0;
}

// creates a constant '0' node
int Circuit::createZERONode(Node* node)
{
    assert(node != NULL);

    node->clearTT();
    node->clearFanin();

    node->type = ZERO_NODE;

    return 0;
}

// creates a 1-input BUFFER node
int Circuit::createBUF1Node(Node* input, Node* output)
{
    assert(input != NULL);
    assert(output != NULL);

    output->clearTT();
    output->clearFanin();

    output->addFanin(input);
    output->tt.setNumVars(1);

    output->tt.addEntry("1");

    return 0;
}

// creates a 1-input NOT node
int Circuit::createNOT1Node(Node* input, Node* output)
{
    assert(input != NULL);
    assert(output != NULL);

    output->clearTT();
    output->clearFanin();

    output->addFanin(input);
    output->tt.setNumVars(1);

    output->tt.addEntry("0");

    return 0;
}

// creates a 2-input OR node
int Circuit::createOR2Node(Node* input1, Node* input2, Node* output)
{
    assert(input1 != NULL);
    assert(input2 != NULL);
    assert(output != NULL);

    output->clearTT();
    output->clearFanin();

    output->addFanin(input1);
    output->addFanin(input2);
    output->tt.setNumVars(2);

    output->tt.addEntry("-1");
    output->tt.addEntry("1-");

    return 0;
}

// creates a 2-input AND node
int Circuit::createAND2Node(Node* input1, Node* input2, Node* output)
{
    assert(input1 != NULL);
    assert(input2 != NULL);
    assert(output != NULL);

    // after you implement this function,
    // change 'return -1' to 'return 0'

    output->clearTT();
    output->clearFanin();

    output->addFanin(input1);
    output->addFanin(input2);
    output->tt.setNumVars(2);

    output->tt.addEntry("11");

    return 0;
}

// creates a 3-input XOR node
int Circuit::createXOR3Node(Node* input1, Node* input2, Node* input3, Node* output)
{
    assert(input1 != NULL);
    assert(input2 != NULL);
    assert(input3 != NULL);
    assert(output != NULL);

    // after you implement this function,
    // change 'return -1' to 'return 0'

    output->clearTT();
    output->clearFanin();

    output->addFanin(input1);
    output->addFanin(input2);
    output->addFanin(input3);
    output->tt.setNumVars(3);

    output->tt.addEntry("100");
    output->tt.addEntry("010");
    output->tt.addEntry("001");
    output->tt.addEntry("111");

    return 0;
}

// creates a 4-input MUX node (with two select bits)
// ordering from msb to lsb is:
// select2 select1
// input4 input3 input2 input1
int Circuit::createMUX4Node(Node* select1, Node* select2,
        Node* input1, Node* input2, Node* input3, Node* input4, Node* output)
{
    assert(select1 != NULL);
    assert(select2 != NULL);
    assert(input1 != NULL);
    assert(input2 != NULL);
    assert(input3 != NULL);
    assert(input4 != NULL);
    assert(output != NULL);

    // after you implement this function,
    // change 'return -1' to 'return 0'

    output->clearTT();
    output->clearFanin();

    output->addFanin(input4);
    output->addFanin(input3);
    output->addFanin(input2);
    output->addFanin(input1);
    output->addFanin(select2);
    output->addFanin(select1);
    output->tt.setNumVars(6);

    output->tt.addEntry("1---11");
    output->tt.addEntry("-1--10");
    output->tt.addEntry("--1-01");
    output->tt.addEntry("---100");

    return 0;
}


// If select is 1, select input2; otherwise, select input1
int Circuit::createMUX2Node(Node* select, Node* input1, Node* input2, Node* output)
{
    assert(select != NULL);
    assert(input1 != NULL);
    assert(input2 != NULL);
    assert(output != NULL);

    output->clearTT();
    output->clearFanin();

    output->addFanin(input2);
    output->addFanin(input1);
    output->addFanin(select);
    output->tt.setNumVars(3);

    output->tt.addEntry("1-1");
    output->tt.addEntry("-10");

    return 0;
}


int Circuit::createFullAdder(Node* input1, Node* input2, Node* cin,
                Node* output, Node* cout)
{
    assert(input1 != NULL);
    assert(input2 != NULL);
    assert(cin != NULL);
    assert(output != NULL);
    assert(cout != NULL);

    output->clearTT();
    output->clearFanin();
    cout->clearTT();
    cout->clearFanin();

    // sum bit
    createXOR3Node(input1, input2, cin, output);

    // carry out bit
    cout->addFanin(input1);
    cout->addFanin(input2);
    cout->addFanin(cin);
    cout->tt.setNumVars(3);

    cout->tt.addEntry("011");
    cout->tt.addEntry("101");
    cout->tt.addEntry("110");
    cout->tt.addEntry("111");

    return 0;
}



