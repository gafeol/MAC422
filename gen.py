import random as rnd
import sys

n = int(sys.argv[1])
proc = 0
t0 = 0

f = open('in', 'w+')

while proc < n:
	t0 += rnd.randrange (0, 20)/10.
	dt = rnd.randrange (1, 30)/10.
	deadline = rnd.randrange (1, 3) * dt + t0
	f.write(str(t0) + " " + str(dt) + " " + str(deadline) + " process"+ str(proc) + "\n")
	proc += 1
