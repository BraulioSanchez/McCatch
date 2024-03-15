/**
* @file
*
* @version 1.0
* @author 006
* @date 10-29-2014
*/

#ifndef CANBERRADISTANCE_H
#define CANBERRADISTANCE_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

/**
* Class to obtain the Canberra Distance
*
* @brief Canberra distance class.
* @author 006
* @version 1.0.
*/
template <class ObjectType>
class CanberraDistance : public DistanceFunction <ObjectType>{

    public:

        CanberraDistance();
        virtual ~CanberraDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "CanberraDistance-inl.h"
#endif // CANBERRADISTANCE_H
