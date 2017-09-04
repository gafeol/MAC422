import random as rnd
import sys

n = int(sys.argv[1])
proc = 0
t0 = 0

f = open('in', 'w+')

while proc < n:
	t0 = rnd.randrange (0, 200)/10. ## processos comecam nos segundos de 0 a 19,1
	t0 = int(round(t0*10))/10.;
	dt = rnd.randrange (1, 50)/10. ## cada processo demora entre 0,1 e 4,9 segundos
	dt = int(round(dt*10))/10.
	deadline = rnd.randrange (10, 30)/10. * dt + t0
	deadline = int(round(deadline*10))/10.
	f.write(str(t0) + " " + str(dt) + " " + str(deadline) + " process"+ str(proc) + "\n")
	proc += 1
