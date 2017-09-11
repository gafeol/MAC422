import sys
import os

if len(sys.argv) < 3:
	print("Wrong number of parameters in compilation")
	print("Usage:\npython runtests.py <escalonador> <numero de arquivos de teste> <OPTIONAL: flag for stderr prints>")
	exit(0);

escalonador = int(sys.argv[1])
nfiles =int(sys.argv[2])

flag = 0

if len(sys.argv) > 3:
	flag = int(sys.argv[3])

os.system("rm results/dl%d.txt" % escalonador)

os.system("make");

for i in range(0, nfiles):
	print("Executando teste %d com escalonador %d" % (i, escalonador))
	os.system("./ep1 %d tests/in%d.txt results/out%d_%d.txt %d >> results/ndl%d.txt" % (escalonador, i, escalonador, i, flag, escalonador) )
