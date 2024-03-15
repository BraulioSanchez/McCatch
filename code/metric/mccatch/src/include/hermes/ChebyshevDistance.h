/**
* @file
*
* This file defines the distance functions of the Minkwosky family.
*
* @version 1.0
* @author 005
* @author 006
* @date 10-29-2014
*/

#ifndef CHEBYSHEVDISTANCE_H
#define CHEBYSHEVDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

/**
* Class to obtain the Chebyshev distance or L infiniy.
*
* @brief L infinity class.
* @author 006.
* @version 1.0.
*/
template <class ObjectType>
class ChebyshevDistance : public DistanceFunction <ObjectType> {

    public:

        ChebyshevDistance();
        virtual ~ChebyshevDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};


#include "ChebyshevDistance-inl.h"
#endif // CHEBYSHEVDISTANCE_H
