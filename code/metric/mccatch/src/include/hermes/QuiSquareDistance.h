/**
* @QuiSquareDistance.hpp
*
* This file defines the distance functions
*
* @version 1.0
* @author 013
* @date 11-03-2014
*/

#ifndef QUISQUAREDISTANCE_H
#define QUISQUAREDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>

/**
* Class to obtain the Qui-Square distance. The chi-squared distance is useful when comparing histograms.
*
* @brief Qui-Square distance class.
* @author 13.
* @version 1.0.
*/
template <class ObjectType>
class QuiSquareDistance : public DistanceFunction<ObjectType> {

    public:

        QuiSquareDistance();
        ~QuiSquareDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error) ;
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "QuiSquareDistance-inl.h"
#endif // QUISQUAREDISTANCE_H
