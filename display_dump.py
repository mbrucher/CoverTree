#!/usr/bin/env python

def parse_input(lines):
  from collections import default_dict
  data = default_dict(list)
  level = 0
  current_level = 10000
  for line in lines:
    if line == "{":
	  level = level + 1
	if line == "}":
	  level = level - 
	if line.startswith("Level"):
	  current_level = int(line.split()[1][:-1])
	if line.startswith("Point"):
	  data[current_level].append(line.split()[1:])
	return data

if __name__ == "__main__":
  from matplotlib import pyplot
  
  f = open("dump.txt")
  data = parse_input(f)
  print data