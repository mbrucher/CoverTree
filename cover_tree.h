/**
 * \file cover_tree.h
 */

#ifndef COVERTREE_H
#define COVERTREE_H

#include <map>
#include <set>

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

    void add_child(const Point& new_data, long level)
    {
      std::unique_ptr<Node> node(new Node);
      node->data = new_data;
      children[level].push_back(node);
    }
  };

  boost::scoped_ptr<Node> root;

  bool insert(const Point& data, const std::set<Node*>& node_set, long level)
  {
    return false;
  }

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
      std::set<Node*> node_set;
      node_set.insert(root.get());
      if(!insert(data, node_set, max_level))
      {
        throw std::runtime_error("No parent found");
      };
    }
  }
};

#endif
