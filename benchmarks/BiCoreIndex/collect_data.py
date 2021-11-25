import os
from collections import OrderedDict

dicc = {}
dicq = {}

for file in os.listdir():
	if ".txt" not in file: continue
	if "orkut" in file : continue
	name = file.split("_")[0]
	num = int(file.split("_")[1])
	with open(file, 'r') as f:
		lines = f.readlines()
	qlines = [line for line in lines if "Query" in line]
	qnums = [float(line[line.index("Query")+12 : -1]) for line in qlines]
	assert(len(qnums)==3)
	clines = [line for line in lines if "Construction" in line]
	cnums = [float(line[line.index("Construction")+19 : -1]) for line in clines]
	assert(len(cnums)==3)
	if name not in dicc:
		dicc[name] = {}
	if name not in dicq:
		dicq[name] = {}
	dicc[name][num] = sum(cnums)/3
	dicq[name][num] = sum(qnums)/3


print("construction times:")

for key, val in dicc.items():
	print(f'{key}: ')
	print(" ".join([str(val[i]) for i in sorted(val)]))

print("query times:")
for key, val in dicq.items():
	print(f'{key}: ')
	print(" ".join([str(val[i]) for i in sorted(val)]))


