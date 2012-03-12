/**
 * \file cover_tree.h
 */

#ifndef COVERTREE_H
#define COVERTREE_H

/**
 * Cover tree class, templated with the inner type
 */
template<class DataType, class Point, class Distance>
class CoverTree
{
  Distance distance;
public:
  CoverTree(const Distance& distance)
    :distance(distance)
  {
  }
};

#endif
