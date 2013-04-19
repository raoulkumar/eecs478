'''
BLIF file generator for n-bit Adder.
'''
import sys

# get argument
nbits = int(sys.argv[1])

# print header
print ".model py_adder" + str(nbits)
print

# print input
print ".inputs",

for i in range(nbits-1, -1, -1):
    print "a[%d]" % i,

for i in range(nbits-1, -1, -1):
    print "b[%d]" % i,

print "cin"
print

# print output
print ".outputs",

for i in range(nbits-1, -1, -1):
    print "s[%d]" % i,

print "cout"
print


# print name fields

def operator(i, j, cin):
    return i + j + cin

## first calculate bits that are turned on.
on_bits = [[] for i in range(nbits+1)]

for i in range(2**nbits):
    for j in range(2**nbits):
        for c in range(2):
            value = operator(i,j,c)

            for k in range(nbits+1):
                if value & (1 << k):
                    on_bits[k].append((i,j,c))

## output a name field for each output bit (for sum bits)
for i in range(nbits):
    print ".names",
    for j in range(nbits-1, -1, -1):
        print "a[%d]" % j,
    for j in range(nbits-1, -1, -1):
        print "b[%d]" % j,
    print "cin",
    print "s[%d]" % i

    for (a,b,c) in on_bits[i]:
        print "%s%s%s" % (('{0:0%db}' % nbits).format(a),
                ('{0:0%db}' % nbits).format(b), str(c)),
        print 1
    print

## output a name field for carry out bit
print ".names",
for j in range(nbits-1, -1, -1):
    print "a[%d]" % j,
for j in range(nbits-1, -1, -1):
    print "b[%d]" % j,
print "cin",
print "cout"

for (a,b,c) in on_bits[nbits]:
    print "%s%s%s" % (('{0:0%db}' % nbits).format(a),
            ('{0:0%db}' % nbits).format(b), str(c)),
    print 1
print


# print footer
print ".end"

