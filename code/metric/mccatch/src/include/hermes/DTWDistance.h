/**
* @file
*
* This file defines the Dynamic Time Warping distance.
*
* @version 1.0
* @author 011
* @date 09-20-2014
*/

#ifndef DTWDISTANCE_HPP
#define DTWDISTANCE_HPP

#include "DistanceFunction.h"
#include <cmath>

/**
* Class to obtain the Dynamic Time Warping Distance
*
* @brief DTW distance class.
* @author 011
* @version 1.0.
*/
template <class ObjectType>
class DTWDistance : public DistanceFunction <ObjectType>{

    public:
        DTWDistance();
        ~DTWDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "DTWDistance-inl.h"
#endif // DTWDISTANCE_HPP
