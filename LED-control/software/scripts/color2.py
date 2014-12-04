#!/usr/bin/python
import liblo
import time
import sys

addresses = [liblo.Address("192.168.0.3","2222"), liblo.Address("192.168.0.4","2222"), liblo.Address("192.168.0.5","2222")]

#int 'Number of arguments:', len(sys.argv), 'arguments.'
#print 'Argument List:', str(sys.argv)


r=0
g=0
b=0
colormax = 1
step = 1.0/256
delay = 1


while True:
	time.sleep(delay)
	for address in addresses:
		g = 0
		r = 1
		liblo.send(address,'/2222',('f', r),('f', g),('f', b))
	time.sleep(delay)
	for address in addresses:
		r = 1
		g = 1
		liblo.send(address,'/2222',('f', r),('f', g),('f', b))
	time.sleep(delay)
	for address in addresses:
		r = 0
	        liblo.send(address,'/2222',('f', r),('f', g),('f', b))		        












 
  
