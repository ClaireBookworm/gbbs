# converts the graph data files from http://konect.cc/networks/ into a file to be inputted into snap converter

# import file
import pandas

## file input
f = open("graphfile.txt", "r")
print(f.read())

rows = []
numbers = {} # dictionary for key value?

for row in f:
    rows += row

