#!/usr/bin/env python

def parse_input(lines):
  from collections import defaultdict
  data = defaultdict(list)
  level = 0
  current_level = 10000
  for line in lines:
    if line == "{":
      level = level + 1
    if line == "}":
      level = level - 1
    if line.startswith("Level"):
      current_level = int(line.split()[1][:-1])
    if line.startswith("Point"):
      data[current_level].append([float(el) for el in line.split()[1:]])
  return data

if __name__ == "__main__":
  from matplotlib import pyplot as plt
  import numpy as np
  
  f = open("dump.txt")
  data = parse_input(f)
  keys = data.keys()
  keys.sort()

  for key in keys:
    a = np.array(data[key])
    print a[:, 0]
    if key == 10000:
      key = 5
    plt.scatter(a[:, 0], a[:, 1], marker = "o", s = (15 * 2.**key)**2, alpha=.5)
    plt.hold(True)

  plt.show()
