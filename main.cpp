/**
 * \file main.cpp
 */

#include <iostream>
#include <vector>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <ctime>

#include "cover_tree.h"

const long POINTSIZE = 2;

float euclidian(const std::vector<float>& p1, const std::vector<float>& p2)
{
  float dist = 0;
  for(unsigned long i = 0; i < p1.size(); ++i)
  {
    dist += (p1[i] - p2[i]) * (p1[i] - p2[i]);
  }
  return dist;
}

std::vector<std::vector<float> > generate(long size)
{
  std::vector<std::vector<float> > data;

  boost::mt19937 engine(static_cast<long unsigned int>(clock()));
  boost::normal_distribution<double> generator;
  boost::variate_generator<boost::mt19937, boost::normal_distribution<double> > binded(engine, generator);

  for(long i = 0; i < size; ++i)
  {
    std::vector<float> point;
    for(long j = 0; j < POINTSIZE; ++j)
    {
      point.push_back(binded());
    }
    data.push_back(point);
  }

  return data;
}

int main(int argc, char** argv)
{
  CoverTree<float, std::vector<float>, float (*const)(const std::vector<float>&, const std::vector<float>&)> tree(&euclidian);

  return EXIT_SUCCESS;
}
