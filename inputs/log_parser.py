import fileinput
import csv

linesA = [[]]
linesB = [[]]
rhoA = []
rhoB = []
timesA = []
timesB = []


filein = input("Log File Name (no extension): ")

for line in fileinput.input(files=filein):
	line = line[:-1]
	# lines.append(line)
	if ("total" in line):
		print("total:" + line)
		continue;
	print(line.split(' '))
	if line.split(' ')[0] == "coreA":
		linesA.append(line.split(' '))
		rhoA.append(line.split(' ')[2])
		timesA.append(line.split(' ')[4])
	elif line.split(' ')[0] == "coreB":
		linesB.append(line.split(' '))
		rhoB.append(line.split(' ')[2])
		timesB.append(line.split(' ')[4])
	else:
		"Error"
	# rho.append(line.split(' ')[2])
	# times.append(line.split(' ')[4])

graph_title = input("Enter Graph Title: ")

def write_results(file, xlabel, ylabel):
	x_label = input("X Label: ")
	y_label = input("Y Label: ")
	with open(file, mode='w') as origin:
		core_writer = csv.writer(origin, delimiter=',',quotechar='"', quoting=csv.QUOTE_MINIMAL)
		core_writer.writerow((x_label,y_label))
		for num in range(0, len(xlabel)):
			core_writer.writerow((xlabel[num],ylabel[num]))

# print(lines)

write_results(graph_title + "-A.csv", rhoA, timesA)
write_results(graph_title + "-B.csv", rhoB, timesB)