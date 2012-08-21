/**
 * \file test_kdtree.cpp
 */

#include <iostream>

#include <Eigen/Core>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include "kdtree.h"

const long POINTSIZE = 2;
const long VECTORLENGTH = 100000;
const long KNNSIZE = 10;
typedef Eigen::Vector2f Point;
typedef std::vector<Point> PointContainer;

float euclidian(const Point& p1, const Point& p2)
{
  float dist = 0;
  for(unsigned long i = 0; i < std::min(p1.size(), p2.size()); ++i)
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
      point[j] = binded();
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
    ++it;
  }
  return result;
}

std::ostream& operator<<(std::ostream& stream, const Point& data)
{
  for(long it = 0; it != data.size(); ++it)
  {
    stream << data[it] << "\t";
  }
  stream << std::endl;
  return stream;
}

int main(int argc, char** argv)
{
  PointContainer data = generate(VECTORLENGTH);

  auto tree = Search::make_kdtree(data[0], &euclidian);
  Point minpoint;
  minpoint << -5.f, -5.f;
  Point maxpoint;
  maxpoint << -5.f, -5.f;
  tree.set_extent(minpoint, maxpoint);

  boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
  for(PointContainer::const_iterator it = data.begin(); it != data.end(); ++it)
  {
    tree.insert(*it);
  }
  std::cout << "Build time " << (boost::posix_time::microsec_clock::local_time() - time) << std::endl;

//  std::ofstream stream("dump.txt");
//  tree.dump(stream);

  Point zero(2, 0.f);
  time = boost::posix_time::microsec_clock::local_time();
  PointContainer result = knn(data, zero, KNNSIZE);
  std::cout << "Out time (linear) " << (boost::posix_time::microsec_clock::local_time() - time) << std::endl;

  time = boost::posix_time::microsec_clock::local_time();
  PointContainer result2 = tree.knn(zero, KNNSIZE);
  std::cout << "Out time (cover_tree) " << (boost::posix_time::microsec_clock::local_time() - time) << std::endl;

  bool boolean = true;
  for(int i = 0; i < std::min(result.size(), result2.size()); ++i)
  {
    for(int j = 0; j < POINTSIZE; ++j)
    {
      boolean = boolean && (result[i][j] == result2[i][j]);
    }
    std::cout << i << std::endl << result[i] << result2[i];
  }
  std::cout << "Result " << (boolean && (result.size() == result2.size())) << std::endl;
  return EXIT_SUCCESS;
}
