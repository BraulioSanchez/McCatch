/**
* @MorositaDistance.hpp
*
* This file defines the Morosita index for abundance data.
*
* @version 1.0
* @author 006
* @date 11-07-2014
*/

#include "DistanceFunction.h"
#include <cmath>

#ifndef MOROSITADISTANCE_H
#define MOROSITADISTANCE_H


/**
* Class to obtain the Morosita Index.
* It is useful to measure abundance data similarity.
*
* @brief Morosita Index Distance class.
* @author 006
* @version 1.0.
*/
template <class ObjectType>
class MorositaDistance : public DistanceFunction<ObjectType> {

    public:
        MorositaDistance();
        ~MorositaDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "MorositaDistance-inl.h"
#endif // MOROSITADISTANCE_H
