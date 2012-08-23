/**
 * \file kdtree.h
 */

#ifndef KDTREE_H_
#define KDTREE_H_

#include <map>

#include <boost/scoped_ptr.hpp>

namespace Search
{
  template<class ContainerType>
  struct Node
  {
    long dimension;
    ContainerType middle;
    ContainerType minpoint;
    ContainerType maxpoint;

    boost::scoped_ptr<Node<ContainerType> > left;
    boost::scoped_ptr<Node<ContainerType> > right;

    std::vector<ContainerType> children;

    template<class Distance, class GlobalContainer>
    void add_nodes(const ContainerType& point, const Distance& distance, GlobalContainer& container) const
    {
      if(left)
      {
        container.insert(std::make_pair(distance(point, left->middle), left.get()));
      }
      if(right)
      {
        container.insert(std::make_pair(distance(point, right->middle), right.get()));
      }
    }

    template<class Distance, class GlobalContainer>
    void add_children(const ContainerType& point, const Distance& distance, GlobalContainer& container) const
    {
      for (typename std::vector<ContainerType>::const_iterator it = children.begin(); it != children.end(); ++it)
      {
        container.insert(std::make_pair(distance(point, *it), *it));
      }
    }

    void add_point(const ContainerType& point)
    {
      children.push_back(point);
    }
  };

  template<class ContainerType, class Distance>
  class KDTree
  {
    typedef Node<ContainerType> MyNode;
    Distance distance;
    ContainerType minpoint;
    ContainerType maxpoint;

    boost::scoped_ptr<MyNode> root;

    bool test_distance_too_great(MyNode* node, const ContainerType& point, double max_dist) const
    {
      return (max_dist - distance(node->middle, node->maxpoint) < distance(point, node->middle));
    }

  public:
    KDTree(const Distance& distance) :
        distance(distance)
    {
    }

    void set_extent(const ContainerType& minpoint, const ContainerType& maxpoint)
    {
      this->minpoint = minpoint;
      this->maxpoint = maxpoint;
    }

    void insert(const ContainerType& point)
    {
      if (!root)
      {
        root.reset(new MyNode);
        root->middle = (minpoint + maxpoint) / 2;
        root->minpoint = minpoint;
        root->maxpoint = maxpoint;
      }
      root->add_point(point);
    }

    typedef std::multimap<double, MyNode*> NodeContainer;
    typedef std::multimap<double, ContainerType> MapContainer;
    std::vector<ContainerType> knn(const ContainerType& point, unsigned long k) const
    {
      std::vector<ContainerType> result;

      if (!root)
      {
        return result;
      }

      NodeContainer nodes;
      nodes.insert(std::make_pair(distance(point, root->middle), root.get()));

      MapContainer points;
      while (!nodes.empty())
      {
        if(points.size() > k)
        {
          typename MapContainer::iterator it = points.begin();
          std::advance(it, k);
          points.erase(it, points.end());
        }

        MyNode* current_node = nodes.begin()->second;
        nodes.erase(nodes.begin());

        if (points.size() > k && test_distance_too_great(current_node, point, points.rbegin()->first))
        {
          continue;
        }
        if(current_node->children.empty())
        {
          current_node->add_nodes(point, distance, nodes);
        }
        else
        {
          current_node->add_children(point, distance, points);
        }
      }

      for (auto it = points.begin(); it != points.end() && result.size() < k; ++it)
      {
        result.push_back(it->second);
      }

      return result;
    }
  };

  template<class ContainerType, class Distance>
  KDTree<ContainerType, Distance> make_kdtree(const ContainerType& container, const Distance& distance)
  {
    return KDTree<ContainerType, Distance>(distance);
  }
}

#endif /* KDTREE_H_ */
