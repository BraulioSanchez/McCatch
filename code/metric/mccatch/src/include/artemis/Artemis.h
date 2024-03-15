/**
* @file
*
* This file include all files needed by artemis.
*
* @version 1.0
* @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
* @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
* @date 11-03-2010
*/

#ifndef ARTEMIS_H
#define ARTEMIS_H

#include <math.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cstring>


#include <artemis/extractor/Extractor.h>
#include <artemis/extractor/HaralickExtractor.h>
#include <artemis/extractor/HaralickFeature.h>
#include <artemis/extractor/MetricHistogram.h>
#include <artemis/extractor/TotalColorHistogram.h>
#include <artemis/extractor/NormalizedHistogram.h>
#include <artemis/extractor/Wavelets.h>
#include <artemis/extractor/ZernikeExtractor.h>
#include <artemis/extractor/TextureSpectrum.h>
#include <artemis/extractor/DiscreteCosineTransformation.h>
#include <artemis/extractor/YCrCbColorSystem.h>
#include <artemis/extractor/ColorLayout.h>
#include <artemis/extractor/ColorStructure.h>
#include <artemis/extractor/ScalableColor.h>
#include <artemis/extractor/EdgeHistogram.h>
#include <artemis/extractor/EdgeHistogramExtractor.h>
#include <artemis/extractor/SRGBColorSystem.h>
#include <artemis/extractor/XYZColorSystem.h>
#include <artemis/extractor/ColorTemperature.h>
#include <artemis/extractor/DaubechiesExtractor.h>
#include <artemis/extractor/HaarExtractor.h>
#include <artemis/extractor/TextureBrowsing.h>
#include <artemis/extractor/LocalBinaryPatternExtractor.h>
#include <artemis/extractor/BICHistogramExtractor.h>
#include <artemis/extractor/RotationInvariantLBPExtractor.h>

#include <artemis/image/Pixel.h>
#include <artemis/image/ImageBase.h>
#include <artemis/image/bmp/BmpLib.h>
//#include <artemis/image/dicom/DcmLib.h>
#include <artemis/image/jpg/JpgLib.h>
//#include <artemis/image/krl/FileHandler.h>
//#include <artemis/image/krl/KrlLib.h>
#include <artemis/image/png/PngLib.h>

#endif
