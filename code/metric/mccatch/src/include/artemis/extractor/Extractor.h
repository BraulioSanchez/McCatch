/**
 * @file
 *
 * This file defines the basic interfaces for feature extractors.
 *
 * @version 1.0
 */
#ifndef EXTRACTOR_h
#define EXTRACTOR_h

#include <iostream>
#include <cstdlib>
#include <stdexcept>

/**
*
* This class is the basic interface for feature extractors. It depends on
* datatypes for handling source data (e.g. Image) and datatypes for
* storing the extracted signature (e.g. stBasicArrayObject).
*
* @brief Base class for any extractor.
* @author 004
* @author 005
* @author 006
* @version 1.0
*/
template< class SignatureType, class DataObjectType >
class Extractor {
    public:

        /**
        * Builds a new uninitialized instance of this class.
        *
        * @note This constructor is required to fullfil this interface.
        */
        Extractor() {
        }

        /**
        * Class destructor.
        */
        virtual ~Extractor() {
        }

        /**
        * This is a pure virtual method to trigger the signature generation of the
        * feature extractors in the library.
        *
        * @param dataObj The data object to be processed.
        * @param[out] sign The object to store the generated signature.
        */
        virtual void generateSignature(const DataObjectType & dataObj, SignatureType & sign) = 0;
};

#endif

