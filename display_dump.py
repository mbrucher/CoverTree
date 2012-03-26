#!/usr/bin/env python

def parse_input(lines):
  from collections import defaultdict
  data = defaultdict(list)
  level = 0
  current_level = 10000
  parent = []
  for line in lines:
    if line == "{":
      level = level + 1
    if line == "}":
      level = level - 1
      parent = parent[:-1]
    if line.startswith("Level"):
      current_level = int(line.split()[1][:-1])
    if line.startswith("Point"):
      el = [float(el) for el in line.split()[1:]]
      data[current_level].append(el)
      parent.append(el)
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
      key = max(keys[:-1]) + 1
    plt.scatter(a[:, 0], a[:, 1], marker = "o", s = (16 * 2.**key)**2, alpha=.5)
    plt.hold(True)

  true_max = 2**max(keys[:-1])
  plt.xlim([-true_max, true_max])
  plt.ylim([-true_max, true_max])
  plt.show()
