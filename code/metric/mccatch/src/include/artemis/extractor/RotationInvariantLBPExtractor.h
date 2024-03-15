#ifndef ROTATIONINVARIANTLBPEXTRACTOR_H
#define ROTATIONINVARIANTLBPEXTRACTOR_H

#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>

/**
 * This is a Template-Class. 
 *
 * <P> A Rotation Invariant Local Binary Pattern is a the frequency of all textures codes 
 * for the 8-neighbors pixel region in the Image of each color band. This
 * implementation creates a feature vector with 36 textures code.
 *
 * NOTE:
 * <ul>
 * <li>SignatureType must be an array object satisfying interface stObject
 * (e.g. stBasicArrayObject <double>);</li>
 * <li>normalizationRange is the maximum value of a bin in the normalization,
 * i.e., every bin is in the range [0, normalizationRange]. It defaults to 1.<br/>
 * The normalization in the range [0, 1] means that the sum of all bin values
 * is equal to 1. The normalization in the range [0, normalizationRange] first
 * normalize the bin values in the range [0, 1] and, thereafter, divides every
 * value is divided by the maximum value found, making the maximum equals to
 * normalizationRange.</li>
 * </ul>
 *
 * @brief Extractor of Rotation Invariant Local Binary Pattern (8-neighbors)
 * @author 014
 * @see BasicArrayObject
 * @version 1.0
 */
template< class SignatureType, class DataObjectType = Image >
class RotationInvariantLBPExtractor : public Extractor<SignatureType, DataObjectType> {
    private:
        u_int16_t numFeatures;

    protected:
        u_int16_t lookUp[256];
        static u_char rotate(uchar value);
        static u_char ror(uchar code);

    private:
        void createLookUp();

    public:
        RotationInvariantLBPExtractor(u_int16_t value = 36);
        virtual ~RotationInvariantLBPExtractor();
        u_int16_t getNumFeatures();
        void setNumFeatures(u_int16_t value);
        void generateSignature(const DataObjectType & image, SignatureType & sign);

};

#include "RotationInvariantLBPExtractor-inl.h"

#endif
