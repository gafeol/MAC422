import sys
import os

escalonador = int(sys.argv[1])
nfiles = int(sys.argv[2])

for i in range(1, nfiles):
	print("Executando teste %d com escalonador %d" % (i, escalonador))
	os.system("./ep1 %d tests/in%d.txt tests/out%d_%d.txt > results/dl%d_%d.txt" % (escalonador, i, i, escalonador, i, escalonador) )
