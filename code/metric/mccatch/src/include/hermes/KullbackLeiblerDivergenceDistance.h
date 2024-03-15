/**
* @file This file contains the Kullback-Leibler divergence implementation.
*
* @see Please, check the License.txt for code usage.
*
* @version 1.0
* @author 012.
* @author 006.
* @date 03-09-2014
*/

#ifndef KULLBACKLEIBLERDIVERGENCEDISTANCE_H
#define KULLBACKLEIBLERDIVERGENCEDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

/**
* Class to obtain the Kullback-Leibler Divergence Distance
* D (X||Y) = sum( Xi * ln ( Xi / Yi ) )
*
* @brief Kullback-Leibler Divergence Distance class.
* @author 012.
* @author 006.
* @date 10-13-2014
* @version 1.0.
*/
template <class ObjectType>
class KullbackLeiblerDivergenceDistance : public DistanceFunction <ObjectType>{

    public:

        KullbackLeiblerDivergenceDistance();
        virtual ~KullbackLeiblerDivergenceDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "KullbackLeiblerDivergenceDistance-inl.h"
#endif // KULLBACKLEIBLERDIVERGENCEDISTANCE_H
