#!/usr/bin/python
import liblo
import time
import sys

addresses = [liblo.Address("192.168.0.5","2222")]

#int 'Number of arguments:', len(sys.argv), 'arguments.'
#print 'Argument List:', str(sys.argv)


r=0
g=0
b=0
colormax = 1
step = 1.0/256
delay = .001

while r < colormax :
    for address in addresses:
        liblo.send(address,'/2222',('f', r),('f', g),('f', b))
    r=r+step
    time.sleep(delay)

while True:
    while g < colormax :
        for address in addresses:
            liblo.send(address,'/2222',('f', r),('f', g),('f', b))
            
        r=r-step
        g=g+step
        time.sleep(delay)
        
    while b < colormax :
        for address in addresses:
            liblo.send(address,'/2222',('f', r),('f', g),('f', b))
            
        g=g-step
        b=b+step
        time.sleep(delay)
    while r < colormax :
        for address in addresses:
            liblo.send(address,'/2222',('f', r),('f', g),('f', b))
            
        b=b-step
        r=r+step
        time.sleep(delay)
