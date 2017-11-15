import sys

if len(sys.argv) < 2:
	print("Wrong number of parameters!");
	print("usage: python gerador.py <numero de processos> <nome do arquivo de saida>");
	exit(0);

n = sys.argv[0]

nome = str(sys.argv[1])

saida = open(nome, 'w')

for i in range(0, n):

saida.close()
