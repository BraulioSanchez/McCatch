/**
* @file
*
* This file implements all basic metric evaluators templates.
*
* @version 1.0
*
* @author 001
* @author 002
*/

#ifndef BRAYCURTISDISTANCE_H
#define BRAYCURTISDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

/**
* This class implements a generic metric evaluator that calculates the distance
* between objects using the Bray Curtis distance function.
*
*
* @author 006
* @warning Both objects must have the same number of entries.
* @see DistanceFunction
*/
template <class ObjectType>
class BrayCurtisDistance: public DistanceFunction<ObjectType> {

    public:

        BrayCurtisDistance();
        virtual ~BrayCurtisDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "BrayCurtisDistance-inl.h"
#endif //BRAYCURTISDISTANCE_H
