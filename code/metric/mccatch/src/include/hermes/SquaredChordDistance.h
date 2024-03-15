/**
* @file
*
* This file implements all basic metric evaluators templates.
*
* @version 1.0
* @author 001
* @author 002
*/

#ifndef SQUAREDCHORDDISTANCE_H
#define SQUAREDCHORDDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

/**
* This class implements a generic metric evaluator that calculates the distance
* between objects using the Bray Curtis distance function.
**
* @author 006.
* @warning Both objects must have the same number of entries.
*/
template <class ObjectType>
class SquaredChordDistance: public DistanceFunction<ObjectType>{

    public:

        SquaredChordDistance();
        virtual ~SquaredChordDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "SquaredChordDistance-inl.h"
#endif // SQUAREDCHORDDISTANCE
