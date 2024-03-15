/**
 * @file
 *
 * This file defines the extractor for Daubechies and Haar Wavelects Extractor.
 *
 * @version 1.0
 */

#ifndef DAUBECHIESEXTRACTOR_h
#define DAUBECHIESEXTRACTOR_h

#include <vector>
#include <cmath>
#include <cstdlib>
#include <cfloat>

#include <artemis/extractor/Wavelets.h>
#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>


/**
* This is a Template-Class. A template-parameter levels defines the levels
* utilized on the Wavelet. The template parameter vectorComposition defines
* a type of signature what will be utilized.
*
* @brief Implementation of Daubechies Wavelet Extractor
* @author 005
* @author 006
* @see ImageBase
* @version 1.0
*/
template< class SignatureType, class DataObjectType = Image >
class DaubechiesExtractor : public Extractor<SignatureType, DataObjectType> , public Wavelets <SignatureType, DataObjectType>{
    public:
        DaubechiesExtractor();
        virtual ~DaubechiesExtractor();

        virtual void generateSignature(const Image & img, SignatureType & sign) throw (std::runtime_error) ;
};

#include "DaubechiesExtractor-inl.h"
#endif //DAUBECHIES
