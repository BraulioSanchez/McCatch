/**
* @file
*
* This file defines the distance function Spearman's Rank Correlation.
*
* @version 1.0
* @author 011.
* @date 10-26-2014
*/

#ifndef SPEARMANDISTANCE_HPP
#define SPEARMANDISTANCE_HPP

#include "DistanceFunction.h"
#include <cmath>

/**
* Class to obtain the Spearman's Rank Correlacion
*
* @brief Spearman's Rank Correlacion class.
* @author 011.
* @version 1.0.
*/
template <class ObjectType>
class SpearmanDistance : public DistanceFunction <ObjectType>{

    public:

        SpearmanDistance();
        ~SpearmanDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "SpearmanDistance-inl.h"
#endif // SPEARMANDISTANCE_HPP
