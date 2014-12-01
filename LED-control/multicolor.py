import sys
import liblo

# this script allows for individual light control
# args: node name (3-5), rgb value

node = "192.168.0." + sys.argv[1]
address = liblo.Address(node,"2222")

#int 'Number of arguments:', len(sys.argv), 'arguments.'
#print 'Argument List:', str(sys.argv)

# rgb colors, from cmd line, range from 0 to 1.
r = float(sys.argv[2])
g = float(sys.argv[3])
b = float(sys.argv[4])

liblo.send(address,'/light/color/set',('f', r),('f', g),('f', b))


#int 'Number of arguments:', len(sys.argv), 'arguments.'
#print 'Argument List:', str(sys.argv)

# rgb colors, from cmd line, range from 0 to 1.
r = float(sys.argv[2])
g = float(sys.argv[3])
b = float(sys.argv[4])

liblo.send(address,'/light/color/set',('f', r),('f', g),('f', b))

