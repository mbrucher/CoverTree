/**
 * \file main.cpp
 */

#include <iostream>
#include <vector>

#include <boost/random/mersenne_twister.hpp>

#include "cover_tree.h"

float euclidian(const std::vector<float>& p1, const std::vector<float>& p2)
{
  float dist = 0;
  for(unsigned long i = 0; i < p1.size(); ++i)
  {
    dist += (p1[i] - p2[i]) * (p1[i] - p2[i]);
  }
  return dist;
}

int main(int argc, char** argv)
{
  CoverTree<float, std::vector<float>, float (*const)(const std::vector<float>&, const std::vector<float>&)> tree(&euclidian);

  return EXIT_SUCCESS;
}
