/**
* @file
*
* This file implements all basic metric evaluators templates.
* Last modification 10-29-2014.
*
* @version 1.0
* @author 001
* @author 002
*/

#ifndef SUMPRODUCTDISTANCE_H
#define SUMPRODUCTDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>


template <class ObjectType>
class SumProductDistance : public DistanceFunction <ObjectType>{

    private:
        typedef DistanceFunction<ObjectType> myDistance;
        int numEval;
        std::vector <double> max;
        std::vector <double> weight;
        std::vector <myDistance *> Evaluator;

    public:
        SumProductDistance();
        virtual ~SumProductDistance();
        void clear();

        void addEval(myDistance *e, double w);
        double getPartialDistance(ObjectType &o1, ObjectType &o2, u_int32_t i);
        double getDistance(ObjectType &o1, ObjectType &o2);
        double GetDistance(ObjectType &o1, ObjectType &o2);
        double getMax(size_t i);
};

#include "SumProductDistance-inl.h"
#endif //SUMPRODUCTDISTANCE_H

