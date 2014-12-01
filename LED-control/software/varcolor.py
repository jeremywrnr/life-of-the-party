import sys
import liblo

address = liblo.Address("192.168.0.3","2222")

#int 'Number of arguments:', len(sys.argv), 'arguments.'
#print 'Argument List:', str(sys.argv)

# rgb colors, from cmd line, range from 0 to 1.
r = float(sys.argv[1])
g = float(sys.argv[2])
b = float(sys.argv[3])

liblo.send(address,'/light/color/set',('f', r),('f', g),('f', b))

