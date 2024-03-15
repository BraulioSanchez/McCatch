/**
* @file
*
* This file defines a Class to extract a Texture Spectrum.
*
* @version 1.0
*/
#ifndef TEXTURESPECTRUM_HPP
#define TEXTURESPECTRUM_HPP

#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>
#include <cfloat>

/**
 * @brief Extractor of Texture Spectrum
 * @version 1.0
 */
template< class SignatureType, class DataObjectType = Image >
class TextureSpectrumExtractor : public Extractor<SignatureType, DataObjectType> {

    private:
        u_int16_t numFeatures;
        u_int8_t deltaRange;

    private:
        u_int16_t getSameValueNumber(u_int16_t elem1, u_int16_t elem2, u_int16_t elem3);
        void getOrientations(u_int16_t neighborValues[], u_int16_t orientations[]);

    public:
        static const int MAX_LOOKUP_NTU = 6561;

    public:
        TextureSpectrumExtractor(u_int8_t deltaRange = 0, u_int16_t numFeatures = 8);
        virtual ~TextureSpectrumExtractor();

        void setNumFeatures(u_int16_t numFeatures);
        void setDeltaRange(u_int8_t deltaRange);

        u_int16_t getNumFeatures();
        u_int8_t getDeltaRange();

        virtual void generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error);
};

//include implementation (template class)
#include "TextureSpectrum-inl.h"
#endif

