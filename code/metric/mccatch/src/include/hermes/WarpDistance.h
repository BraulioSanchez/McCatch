/**
* @file
*
* This file defines the distance functions of the Warp Distance for the Metric Histogram.
*
* @version 1.0
* @author 005
* @author 006
* @date 10-29-2014
*/

#ifndef WARPDISTANCE_H
#define WARPDISTANCE_H

#include "DistanceFunction.h"
#include <stdexcept>
/**
* Class to warp diferences between two metric histograms.
*
* @brief Warp distance between objects.
* @author 006.
* @version 1.0.
*/
template <class ObjectType>
class WarpDistance : public DistanceFunction <ObjectType>  {

    public:
        WarpDistance();
        virtual ~WarpDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "WarpDistance-inl.h"
#endif // WARPDISTANCE_H
