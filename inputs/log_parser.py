import fileinput
import csv
import os

print(os.getcwd())
filein = input("Log File Name (no extension): ")
for k in [1,2,4,8,16,30,60]:
	# linesA = [[]]
	# linesB = [[]]
	lines = [[]]
	# rhoA = []
	# rhoB = []
	# timesA = []
	# timesB = []
	rho = []
	times = []
	sum = 0
	count = 0
	deltaC = 0
	stopped = False
	filename1 = filein+"_adj_"+str(k)+"_threads.txt"
	filename2 = filein+"_"+str(k)+".error"
	filename3 = "../benchmarks/BiCore/"+filein+"_"+str(k)+".error"
	fin = None
	if os.path.isfile(filename1):
		fin = fileinput.input(files = filename1)
	elif os.path.isfile(filename2):
		fin = fileinput.input(files = filename2)
	elif os.path.isfile(filename3):
		fin = fileinput.input(files = filename3)
	else:
		print("no such file")
		exit(0)
	for line in fin:
		line = line[:-1]
		if "coreA" not in line and "coreB" not in line and "total" not in line and "time per iter" not in line:
			continue
		lines.append(line)
		if "coreA" in line and not stopped:
			deltaC+=1
		if "coreB" in line:
			stopped=True
		if ("total" in line):
			sum += float(line.split(' ')[1])
			count += 1
			continue
		if "time per iter" in line:
			sum += float(line.split(' ')[-1])
			print(sum)
			count += 1
			continue
		rho.append(line.split(' ')[2])
		times.append(line.split(' ')[4])
	print(k)
	print ("Rho max: "+str(max(rho)))
	print ("delta: "+str(deltaC))

	graph_title = filein+"-"+str(k)+"-rho-time"

	def write_results(file, xlabel, ylabel):
		x_label = "rho"
		y_label = "time"
		with open(file, mode='w') as origin:
			core_writer = csv.writer(origin, delimiter=',',quotechar='"', quoting=csv.QUOTE_MINIMAL)
			core_writer.writerow((x_label,y_label))
			for num in range(0, len(xlabel)):
				core_writer.writerow((xlabel[num],ylabel[num]))

	# print(lines)
	write_results(graph_title + ".csv", rho, times)
	print(sum)
	print ("avg time: "+str(sum / float(count)))
	# write_results(graph_title + "-A.csv", rhoA, timesA)
	# write_results(graph_title + "-B.csv", rhoB, timesB)