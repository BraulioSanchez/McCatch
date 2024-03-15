/**
* @file
*
* This file defines the distance functions of the Minkwosky family.
*
* @version 1.0
* @author 003
* @author 006
* @date 10-29-2014
*/

#ifndef MANHATTANDISTANCE_H
#define MANHATTANDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

/**
* Class to obtain the Manhattan (or L1) Distance
*
* @brief L1 distance class.
* @author 006.
* @version 1.0.
*/
template <class ObjectType>
class ManhattanDistance : public DistanceFunction<ObjectType> {

    public:
        ManhattanDistance();
        ~ManhattanDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};


#include "ManhattanDistance-inl.h"
#endif // MANHATTANDISTANCE_H
