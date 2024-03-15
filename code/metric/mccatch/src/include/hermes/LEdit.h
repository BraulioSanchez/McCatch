/**
* @file
*
* This file defines the distance functions of the LEdit.
*
* @version 1.0
* @author 003.
* @author 006.
* @date 10-29-2014
*/

#ifndef LEDIT_H
#define LEDIT_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

/**
* Class to obtain the LEdit distance.
*
* @brief L-Edit class.
* @author 006.
* @version 1.0.
*/
template <class ObjectType = std::string >
class LEditDistance : public DistanceFunction <ObjectType> {

    public:

        LEditDistance();
        virtual ~LEditDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "LEdit-inl.h"
#endif // LEDIT_H
