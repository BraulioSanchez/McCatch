/**
* @file
*
* This file defines the Euclidean distance.
*
* @version 1.0
* @author 005
* @author 006
* @date 10-29-2014
*/

#ifndef EUCLIDEANDISTANCE_H
#define EUCLIDEANDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

/**
* Class to obtain the Euclidean (or geometric) Distance
*
* @brief L2 distance class.
* @author 006.
* @version 1.0.
*/
template <class ObjectType>
class EuclideanDistance : public DistanceFunction <ObjectType>{

    public:

        EuclideanDistance();
        virtual ~EuclideanDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "EuclideanDistance-inl.h"
#endif // EUCLIDEANDISTANCE_H
