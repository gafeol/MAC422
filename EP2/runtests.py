import sys
import os

if len(sys.argv) < 4:
	print("Wrong number of parameters in compilation")
	print("Usage:\npython runtests.py <tamanho da pista> <numero de ciclistas> <numero de voltas>  <OPTIONAL: flag for stderr prints>")
	exit(0);

tam_pista = int(sys.argv[1])
num_ciclistas =int(sys.argv[2])
num_voltas = int(sys.argv[3])

flag = ""

if len(sys.argv) > 4:
	flag = sys.argv[4]

os.system("rm dados.txt")
os.system("rm mem.txt")

os.system("make");

print("Teste com %d %d %d" % (tam_pista, num_ciclistas, num_voltas))

for i in range(0, 30):
	print("Executando teste %d" % i)
	with open("mem.txt", "a") as myfile:
	    myfile.write("Teste "+repr(i))

	os.system("./memusage.sh ./ep2 %d %d %d %s dum > out 2>> mem.txt"  % (tam_pista, num_ciclistas, num_voltas, flag) )

os.system("mv dados.txt dados_%d_%d_%d.txt" % (tam_pista, num_ciclistas, num_voltas))
os.system("mv mem.txt mem_%d_%d_%d.txt" % (tam_pista, num_ciclistas, num_voltas))
