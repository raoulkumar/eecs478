'''
Test pattern generator for the function 'createABSMIN5X3YModule'.
'''
import random
from bitstring import BitArray

def operation(input1, input2):
    return abs(min(5*input1, 3*input2))

def run():
    num_test = 10
    for i in range(num_test):
        input1 = random.randint(-2**15, 2**15-1)
        input2 = random.randint(-2**15, 2**15-1)
        output = operation(input1, input2)

        ib1 = BitArray(int=input1, length=16)
        ib2 = BitArray(int=input2, length=16)
        out = BitArray(int=output, length=19)

        print ib1.bin
        print ib2.bin
        print out.bin
        print

run()

