import sys

if len(sys.argv) < 2:
	print("Wrong number of parameters!");
	print("usage: python gerador.py <numero de processos> <nome do arquivo de saida>");
	exit(0);

n = sys.argv[0]

nome = str(sys.argv[1])

saida = open(nome, 'w')

saida.write("100 10 1 2\n")

for i in range(0, n):
	t0 = random.randint(
saida.close()
