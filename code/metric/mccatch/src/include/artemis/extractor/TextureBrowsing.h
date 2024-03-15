/**
* @file
*
* This file defines a Class to extract a Texture Browsing of the image.
*
* @version 1.0
* @date 20-10-2014
*/
#ifndef TEXTUREBROWSING_H
#define TEXTUREBROWSING_H

#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>

#include <cmath>
#include <vector>

template< class SignatureType, class DataObjectType = Image>
class TextureBrowsingExtractor : public Extractor<SignatureType, DataObjectType> {

    private:
        u_int16_t sigmaS[4] = {25,50,75,100};
        u_int16_t thetaK[6] = {0,30,60,90,120,150};
        u_int16_t kernelSize = 21;
        u_int16_t sigmaSize = 4;
        u_int16_t thetaSize = 6;
        double threshold = 0.03;

    private:
        std::vector<std::vector<double> > mkKernel(u_int16_t ks, double sig, double th, double lm, double ps);
        std::vector<std::vector<double> > convolution(const DataObjectType &image, std::vector<std::vector<double> > kernel);
        double calculateMean(std::vector<std::vector<double> > matrix);
        double calculateDeviation(std::vector<std::vector<double> > matrix, double mean);
        void calculateConstrastandGama(std::vector<std::vector<double> > matrix, double *gama, double *verticalConstrast, double *horizontalContrast, std::vector<std::pair<double,double> > *coordinates);
        std::vector<double> calculatePBC2to5(std::vector<double> results, std::vector<u_int16_t> clusteringPoints);
        double candidateClassification(std::vector<double> results, std::vector<u_int16_t> clusteringPoints);
        std::vector<u_int16_t> agglomerativeClustering(std::vector<std::pair<double, double> > coordinates);

    public:
        TextureBrowsingExtractor(u_int16_t sigmaSize = 4, u_int16_t thetaSize = 6);
        virtual ~TextureBrowsingExtractor();

        u_int16_t getKernelSize();

        void setSigmaSize(u_int16_t value);
        void setThetaSize(u_int16_t value);
        void setThreshold(double value);

        void setKernelSize(u_int16_t value);
        double getThreshold();
        u_int16_t getSigmaSize();
        u_int16_t getThetaSize();

        virtual void generateSignature(const DataObjectType &image, SignatureType &sign);
};


#include "TextureBrowsing-inl.h"
#endif
