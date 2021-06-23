import fileinput
import csv

lines = [[]]
rho = []
times = []

filein = input("Log File Name (no extension): ")

for line in fileinput.input(files=filein):
	line = line[:-1]
	# lines.append(line)
	if ("total" in line):
		print("total:" + line)
		continue;
	
	lines.append(line.split(' '))
	print(line.split(' '))
	rho.append(line.split(' ')[2])
	times.append(line.split(' ')[4])

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

write_results(graph_title + ".csv", rho, times)