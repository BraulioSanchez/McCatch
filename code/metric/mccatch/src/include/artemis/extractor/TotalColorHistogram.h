#ifndef TOTALCOLORHISTOGRAM_HPP
#define TOTALCOLORHISTOGRAM_HPP

#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>

/**
 * This is a Template-Class. A template-parameter numFeatures is used
 * to define the lenght of the histogram.
 *
 * <P> A Total Clor Histogram is a simple sum of all pixel frequency
 * in the Image of each color band.
 *
 *
 * @brief Extractor of Normalized Histogram
 * @version 1.0
 */
template<class SignatureType, class DataObjectType = Image>
class TotalColorHistogramExtractor : public Extractor<SignatureType, DataObjectType> {

    private:
        u_int16_t numFeatures;

    public:

        TotalColorHistogramExtractor(u_int16_t numFeatures = 256);
        virtual ~TotalColorHistogramExtractor();

        u_int16_t getNumFeatures();

        void generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error, std::range_error);
};


#include "TotalColorHistogram-inl.h"
#endif
