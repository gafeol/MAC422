import sys
import os

escalonador = int(sys.argv[1])
nfiles =int(sys.argv[2])

flag = 0

if len(sys.argv) > 3:
	flag = int(sys.argv[3])

os.system("rm results/dl%d.txt" % escalonador)

for i in range(0, nfiles):
	print("Executando teste %d com escalonador %d" % (i, escalonador))
	os.system("./ep1 %d tests/in%d.txt tests/out%d_%d.txt %d >> results/dl%d.txt" % (escalonador, i, escalonador, i, flag, escalonador) )
