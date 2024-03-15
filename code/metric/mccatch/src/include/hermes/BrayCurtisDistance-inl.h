/**
* Constructor.
*/
template <class ObjectType>
BrayCurtisDistance<ObjectType>::BrayCurtisDistance(){
}

/**
* Destructor.
*/
template <class ObjectType>
BrayCurtisDistance<ObjectType>::~BrayCurtisDistance(){}

/**
* @copydoc DistanceFunction::GetDistance()
*/
template <class ObjectType>
double BrayCurtisDistance<ObjectType>::GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    return getDistance(obj1, obj2);
}

/**
* @copydoc DistanceFunction::getDistance()
*/
template <class ObjectType>
double BrayCurtisDistance<ObjectType>::getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    if (obj1.size() != obj2.size()){
        throw std::length_error("The feature vectors do not have the same size.");
    }

    double d;
    double tmpNumerator;
    double tmpDenominator;

    d = 0;
    for (size_t i = 0; i < obj1.size(); i++){
        tmpNumerator   = fabs(obj1[i] -  obj2[i]);
        tmpDenominator = obj1[i]  +  obj2[i];

        if (!(tmpDenominator == 0.0)){
            d = d + (tmpNumerator/tmpDenominator);
        }
    }

    // Statistic support
    this->updateDistanceCount();

    return d;
}
