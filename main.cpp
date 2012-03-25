/**
 * \file main.cpp
 */

#include <ctime>
#include <fstream>
#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include "cover_tree.h"

const long POINTSIZE = 2;
const long VECTORLENGTH = 1000;
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

PointContainer knn(const PointContainer& container, const Point& data, int k)
{
  std::map<float, Point> point_map;
  for(PointContainer::const_iterator it = container.begin(); it != container.end(); ++it)
  {
    point_map[euclidian(data, *it)] = *it;
  }

  PointContainer result;
  std::map<float, Point>::const_iterator it = point_map.begin();
  for(int i = 0; i < k; ++i)
  {
    result.push_back(it->second);
  }
  return result;
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

  PointContainer data = generate(VECTORLENGTH);

  boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
  for(PointContainer::const_iterator it = data.begin(); it != data.end(); ++it)
  {
    tree.insert(*it);
  }
  std::cout << "Build time " << (boost::posix_time::microsec_clock::local_time() - time) << std::endl;

  /*  std::ofstream stream("dump.txt");
  tree.dump(stream);*/

  Point zero(2, 0.f);
  time = boost::posix_time::microsec_clock::local_time();
  PointContainer result = knn(data, zero, 10);
  std::cout << "Out time " << (boost::posix_time::microsec_clock::local_time() - time) << std::endl;

  return EXIT_SUCCESS;
}
