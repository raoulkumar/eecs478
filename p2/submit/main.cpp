#include <iostream>

using namespace std;

#include "circuit.h"

void usage(const char* exename);


int main(int argc, char **argv)
{
    // default values
    unsigned int numBits = 0;
    string outFilename = "";

    Circuit c;

    // parsing inputs
    if (argc < 2)
    {
        usage(argv[0]);
    }

    if (argv[1] == string("-h") || argv[1] == string("-help"))
    {
        usage(argv[0]);
    }

    for (int i = 1; i < argc; ++i)
    {
        if (argv[1] == string("-h") || argv[1] == string("-help"))
        {
            usage(argv[0]);
        }
        else if (argv[i] == string("-add"))
        {
            if (i+2 < argc)
            {
                numBits = static_cast<unsigned>(atoi(argv[++i]));
                outFilename = argv[++i];

                if (!c.createADDModule("a", "b", "cin", "s", "cout", numBits))
                {
                    cout << "Adder module successfully created." << endl;

                    // your code here
                    c.setName("pyongjoo_adder");

                    c.setPIs("a", 0, numBits);
                    c.setPIs("b", 0, numBits);
                    c.setPI("cin");
                    c.setPOs("s", 0, numBits);
                    c.setPO("cout");

                    c.writeBLIF(outFilename);
                }
                else cout << "Problem creating adder module." << endl;
            }
            else
            {
                cout << "option -add requires two arguments" << endl;
                usage(argv[0]);
            }
        }
        else if (argv[i] == string("-sub"))
        {
            if (i+2 < argc)
            {
                numBits = static_cast<unsigned>(atoi(argv[++i]));
                outFilename = argv[++i];
                if (!c.createSUBModule("a", "b", "s", numBits))
                {
                    cout << "Subtractor module successfully created." << endl;

                    // your code here
                    c.setName("pyongjoo_subtractor");

                    c.setPIs("a", 0, numBits);
                    c.setPIs("b", 0, numBits);
                    c.setPOs("s", 0, numBits);

                    c.writeBLIF(outFilename);
                }
                else cout << "Problem creating substractor module." << endl;
            }
            else
            {
                cout << "option -sub requires two arguments" << endl;
                usage(argv[0]);
            }
        }
        else if (argv[i] == string("-shift"))
        {
            if (i+3 < argc)
            {
                numBits = static_cast<unsigned>(atoi(argv[++i]));
                unsigned int numShift = static_cast<unsigned>(atoi(argv[++i]));
                outFilename = argv[++i];
                if (!c.createSHIFTModule("orig", "out", numBits, numShift))
                {
                    cout << "Shifter module successfully created." << endl;
                    c.setName("shifter");
                    c.setPIs("orig", 0, numBits);
                    c.setPOs("out", 0, numBits+numShift);
                    //          c.print();   // optional
                    c.writeBLIF(outFilename);
                }
                else cout << "Problem creating shifter module." << endl;
            }
            else
            {
                cout << "option -shift requires three arguments" << endl;
                usage(argv[0]);
            }
        }
        else if (argv[i] == string("-absmin5x3y"))
        {
            if (i+1 < argc)
            {
                outFilename = argv[++i];
                if (!c.createABSMIN5X3YModule("x", "y", "z"))
                {
                    cout << "abs(min(5x, 3y)) module successfully created." << endl;
                    // your code here
                    c.setName("pyongjoo_absmin5x3y");
                    c.setPIs("x", 0, 16);
                    c.setPIs("y", 0, 16);
                    c.setPOs("z", 0, 19);
                    c.writeBLIF(outFilename);
                }
                else cout << "Problem creating abs(min(5x, 3y)) module." << endl;
            }
            else
            {
                cout << "option -absmin5x3y requires one argument" << endl;
                usage(argv[0]);
            }
        }
        else
        {
            cout << "unrecognized command" << argv[i] << endl;
            usage(argv[0]);
        }
    }

    return 0;
}

void usage(const char* exename)
{
    cout << "Usage: " << exename << " [options]" << endl;
    cout << endl;
    cout << "Available Options: " << endl;
    cout << "-h or -help                           Print this message and exit" << endl;
    cout << "-add <uint> <outFilename>             Save <uint>-bit adder module in BLIF format to <outFilename>" << endl;
    cout << "-sub <uint> <outFilename>             Save <uint>-bit subtractor module in BLIF format to <outFilename>" << endl;
    cout << "-shift <uint1> <uint2> <outFilename>  Save <uint1>-bit number shifted by <uint2> bits in BLIF format to <outFilename>" << endl;
    cout << "-absmin5x3y <outFilename>                    Save abs(min(5x, 3y)) module in BLIF format to <outFilename>" << endl;
    cout << endl;
    cout << "Example: " << exename << " -add 32 adder32.blif should create a 32-bit adder module in adder32.blif." << endl;
    cout << endl;

    exit(0);
}

