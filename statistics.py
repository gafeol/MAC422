import sys
from math import sqrt

if len(sys.argv) < 2:
	print("Wrong number of parameters!");
	print("usage: python statistics.py <number of files>");
	exit(0);

nfiles = int(sys.argv[1]);

for arq in range(0, nfiles):
	entrada = open("dl"+str(arq+1)+".txt", 'r')
	saida = open("dl"+str(arq+1) + "_results.txt", 'w+')
	media1 = 0
	media2 = 0
	desvio1 = 0
	desvio2 = 0

	lines = entrada.readlines()
	n = len(lines)
	#calcula a media das mudancas de contexto e a dos deadlines
	for line in lines:
		a, b = map(int, line.split())
		media1 += a
		media2 += b

	media1 /= n
	media2 /= n

	#calcula o desvio padrao de cada parametro
	for line in lines:
		a, b = map(int, line.split())
		desvio1 += (a-media1)**2
		desvio2 += (b-media2)**2

	desvio1 = sqrt(desvio1/(n-1))
	desvio2 = sqrt(desvio2/(n-1))

	saida.write("mudancas de contexto: " + str(media1) + " " + str(desvio1) + '\n')
	saida.write("deadlines: " + str(media2) + " " + str(desvio2) + '\n')
	
	entrada.close()
	saida.close()