/**
* @file
*
* This file implements the Normalized Histogram class.
*/


/**
* Generates a Normalized Histogram from the image provided.
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated normalized histogram.
* @return The resulting ExtReturnCode.
*/
template< class SignatureType, class DataObjectType>
void NormalizedHistogramExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error){

    u_int32_t idxPixelValue;
    double normalizer;
    double *featureVector = NULL;

    //Get the rate to normalize each pixel value
    normalizer = pow(2, (double) image.getBitsPerPixel()) / getHistogramSize();

    featureVector = new double[getHistogramSize()];
    for (size_t i = 0; i < getHistogramSize(); i++) {
        featureVector[i] = 0;
    }

    //Start extracting the histogram.
    //Read each pixel in the image, summing the color ocurrences.
    for (size_t x = 0; x < image.getWidth(); x++) {
        for (size_t y = 0; y < image.getHeight(); y++) {
            //Resize the pixel value according with the Histogram size.
            idxPixelValue = (u_int32_t) ((image.getPixel(x, y).getGrayPixelValue() / normalizer));
            if (idxPixelValue >= getHistogramSize()) {
                delete[] featureVector;
                throw std::runtime_error("Pixel out of bounds.");
            }
            featureVector[idxPixelValue]++;
        }
    }

    //Is the normalization range defined to [0, 1]?
    if (isNormalized()) {
        //Normalize the histogram in the range [0, 1], making the sum of all bin
        //values equals to 1.
        for (size_t i = 0; i < getHistogramSize(); i++) {
            featureVector[i] = featureVector[i] / image.getSize();
        }
    }

    //Populate the sign object with the features computed
    sign.resize(getHistogramSize(), 0.0);
    for (size_t i = 0; i < getHistogramSize(); i++) {
        sign[i] = featureVector[i];
    }

    delete[] featureVector;
}
