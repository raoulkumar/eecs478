'''
Test pattern generator for 16bit adder simulation
'''
import random
from bitstring import BitArray


def operation(input1, input2):
    return input1 + input2

def print_input(ib1, ib2):
    for idx, val in enumerate(ib1[::-1]):
        outfile.write('a[%d] %s\n' % (idx, val))
    for idx, val in enumerate(ib2[::-1]):
        outfile.write('b[%d] %s\n' % (idx, val))

def run():

    num_test = 1
    for i in range(num_test):
        input1 = random.randint(0, 2**16-1)
        input2 = random.randint(0, 2**16-1)
        output = operation(input1, input2)

        ib1 = BitArray(int=input1, length=20)
        ib2 = BitArray(int=input2, length=20)
        out = BitArray(int=output, length=20)

        ib1 = ib1.bin[-16:]
        ib2 = ib2.bin[-16:]
        out = out.bin[-17:]

        print ib1
        print ib2
        print out
        print

        print_input(ib1, ib2)


outfile = open('simtest_adder16.input', 'w')
run()
outfile.close()
