/**
 * \file cover_tree.h
 */

#ifndef COVERTREE_H
#define COVERTREE_H

#include <map>

#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_list.hpp>

/**
 * Cover tree class, templated with the inner type
 */
template<class DataType, class Point, class Distance>
class CoverTree
{
  static const long max_level = 10;

  Distance distance;

  struct Node
  {
    Point data;
    std::map<long, boost::ptr_list<Node> > children;

    bool insert(const Point& data, long level)
    {
      return false;
    }
  };

  boost::scoped_ptr<Node> root;

public:
  CoverTree(const Distance& distance)
    :distance(distance)
  {
  }

  void insert(const Point& data)
  {
    if(!root)
    {
      root.reset(new Node);
      root->data = data;
    }
    else
    {
      if(!root->insert(data, max_level))
      {
        throw std::runtime_error("No parent found");
      };
    }
  }
};

#endif
