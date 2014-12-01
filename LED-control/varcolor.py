import sys
import liblo

# this script controls all three lights in unison
# takes in a rgb value (from 0-1) as args

address3 = liblo.Address("192.168.0.3","2222")
address4 = liblo.Address("192.168.0.4","2222")
address5 = liblo.Address("192.168.0.5","2222")

#int 'Number of arguments:', len(sys.argv), 'arguments.'
#print 'Argument List:', str(sys.argv)

# rgb colors, from cmd line, range from 0 to 1.
r = float(sys.argv[1])
g = float(sys.argv[2])
b = float(sys.argv[3])

liblo.send(address3,'/light/color/set',('f', r),('f', g),('f', b))
liblo.send(address4,'/light/color/set',('f', r),('f', g),('f', b))
liblo.send(address5,'/light/color/set',('f', r),('f', g),('f', b))

