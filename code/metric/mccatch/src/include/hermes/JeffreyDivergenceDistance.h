/**
* @file
*
* This file defines the distance functions of the Minkwosky family.
*
* @see See the License.txt for details.
*
* @version 1.0
* @author 006.
* @date 10-29-2014
*/

#ifndef JEFFREYDIVERGENCEDISTANCE_H
#define JEFFREYDIVERGENCEDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

/**
* Class to obtain the Jeffrey Divergence Distance
*
* @brief Jeffrey Divergence distance class.
* @author 006.
* @version 1.0.
*/
template <class ObjectType>
class JeffreyDivergenceDistance : public DistanceFunction <ObjectType>{

    public:

        JeffreyDivergenceDistance();
        virtual ~JeffreyDivergenceDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "JeffreyDivergenceDistance-inl.h"
#endif // JEFFREYDIVERGENCEDISTANCE_H
