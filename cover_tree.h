/**
* \file cover_tree.h
*/

#ifndef COVERTREE_H
#define COVERTREE_H

#include <iosfwd>
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
  static const long default_max_level = 0;
  long max_level;

  Distance distance;

  struct Node
  {
    Point data;
    typedef boost::ptr_list<Node> ChildrenLevelContainer;
    typedef std::map<long, ChildrenLevelContainer> ChildrenContainer;
    ChildrenContainer children;

    void add_child(const Point& new_data, long level)
    {
      std::auto_ptr<Node> node(new Node);
      node->data = new_data;
      children[level].push_back(node);
    }

    void dump(std::ostream& stream) const
    {
      stream << "Point " << data << "{" << std::endl;
      for(ChildrenContainer::const_iterator it = children.begin(); it != children.end(); ++it)
      {
        stream << "Level " << it->first << "{" << std::endl;
        for(ChildrenLevelContainer::const_iterator it_level = it->second.begin(); it_level != it->second.end(); ++it_level)
        {
          it_level->dump(stream);
        }
        stream << "}" << std::endl;
      }
      stream << "}" << std::endl;
    }
  };

  boost::scoped_ptr<Node> root;

  DataType find_min_dist(const Point& data, const std::set<Node*>& node_set) const
  {
    DataType dist = std::numeric_limits<DataType>::max();
    for(std::set<Node*>::const_iterator it = node_set.begin(); it != node_set.end(); ++it)
    {
      DataType current_dist = distance(data, (*it)->data);
      if(current_dist < dist)
      {
        dist = current_dist;
      }
    }
    return dist;
  }

  void populate_set_from_node(const Point& data, Node* node, std::set<Node*>& node_set, long level) const
  {
    DataType max_dist = std::pow(static_cast<DataType>(2), level);
    if(distance(data, node->data) < max_dist)
      node_set.insert(node);
    Node::ChildrenContainer::iterator it = node->children.find(level);
    if(it == node->children.end())
    {
      return;
    }
    for(Node::ChildrenLevelContainer::iterator it_level = it->second.begin(); it_level != it->second.end(); ++it_level)
    {
      if(distance(data, it_level->data) < max_dist)
      {
        node_set.insert(&*it_level);
      }
    }
  }

  bool insert(const Point& data, const std::set<Node*>& node_set, long level)
  {
    DataType dist = find_min_dist(data, node_set);
    if(dist > std::pow(static_cast<DataType>(2), level))
    {
      return false;
    }
    if(dist <= std::pow(static_cast<DataType>(2), level - 1))
    {
      std::set<Node*> new_node_set;
      for(std::set<Node*>::const_iterator it = node_set.begin(); it != node_set.end(); ++it)
      {
        populate_set_from_node(data, *it, new_node_set, level - 1);
      }
      if(insert(data, new_node_set, level - 1))
      {
        return true;
      }
    }
    (*node_set.begin())->add_child(data, level);
    return true;
  }

public:
  CoverTree(const Distance& distance)
    :max_level(default_max_level), distance(distance)
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
        ++max_level;
        insert(data);
      };
    }
  }

  void dump(std::ostream& stream) const
  {
    if(root)
    {
      root->dump(stream);
    }
  }
};

#endif
