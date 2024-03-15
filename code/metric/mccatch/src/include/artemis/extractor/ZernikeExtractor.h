#ifndef ZERNIKEEXTRACTOR_H
#define ZERNIKEEXTRACTOR_H

#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>
#include <cmath>
#include <vector>

/**
*
* @brief Extractor of Zernicke Moments
* @see Image hermes/util/BasicArrayObject
* @version 1.0
*/
template< class SignatureType, class DataObjectType = Image>
class ZernikeExtractor  : public Extractor<SignatureType, DataObjectType>{

    private:
        u_int8_t numMoments;

    public:
        ZernikeExtractor(u_int8_t numMoments = 10);
        ~ZernikeExtractor();

        void setNumMoments(u_int8_t numMoments);
        u_int8_t getNumMoments();

        double realZernikePolynom(char n, char m, double r2);
        std::pair< double, double> zernikePolynom(short n, short m, double x, double y);
        std::pair<double, double> zernikeMoment(Image image, short n, short m, short xc, short yc, double xscale, double yscale);
        std::vector< std::pair<double, double> > zernikeMoments(short n, Image image);

        virtual void generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error);
};

#include "ZernikeExtractor-inl.h"
#endif // ZERNIKEEXTRACTOR_H
