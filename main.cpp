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
typedef std::vector<float> Point;
typedef std::vector<std::vector<float> > PointContainer;

float euclidian(const Point& p1, const Point& p2)
{
  float dist = 0;
  for(unsigned long i = 0; i < p1.size(); ++i)
  {
    dist += (p1[i] - p2[i]) * (p1[i] - p2[i]);
  }
  return std::sqrt(dist);
}

PointContainer generate(long size)
{
  PointContainer data;

  boost::mt19937 engine(static_cast<long unsigned int>(clock()));
  boost::normal_distribution<double> generator;
  boost::variate_generator<boost::mt19937, boost::normal_distribution<double> > binded(engine, generator);

  for(long i = 0; i < size; ++i)
  {
    Point point;
    for(long j = 0; j < POINTSIZE; ++j)
    {
      point.push_back(binded());
    }
    data.push_back(point);
  }

  return data;
}

std::ostream& operator<<(std::ostream& stream, const Point& data)
{
  for(Point::const_iterator it = data.begin(); it != data.end(); ++it)
  {
    stream << *it << "\t";
  }
  stream << std::endl;
  return stream;
}

int main(int argc, char** argv)
{
  CoverTree<float, std::vector<float>, float (*const)(const std::vector<float>&, const std::vector<float>&)> tree(&euclidian);

  PointContainer data = generate(10000);

  for(PointContainer::const_iterator it = data.begin(); it != data.end(); ++it)
  {
    tree.insert(*it);
  }

  tree.dump(std::cout);

  return EXIT_SUCCESS;
}
