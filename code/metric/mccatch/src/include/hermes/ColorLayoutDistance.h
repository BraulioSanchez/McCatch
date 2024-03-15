/**
* @file
*
* This file defines the distance functions of the Minkwosky family.
*
* @version 1.0
* @author 009
* @author 006
* @date 07-29-2014
*/

#ifndef COLORLAYOUTDISTANCE_H
#define COLORLAYOUTDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

/**
* Class to obtain the Color Layout Distance
*
* @brief Color Layout distance class.
* @author 009.
* @author 006.
* @version 1.0.
*/
template <class ObjectType>
class ColorLayoutDistance : public DistanceFunction <ObjectType>{

    public:
        ColorLayoutDistance();
        virtual ~ColorLayoutDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "ColorLayoutDistance-inl.h"
#endif // COLORLAYOUTDISTANCE_H
