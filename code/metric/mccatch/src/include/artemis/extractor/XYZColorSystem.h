/**
* @file
*
* This file contains the definition of the XYZColorSystem class.
*
* @version 1.0
*/
#ifndef XYZCOLORSYSTEM_HPP
#define XYZCOLORSYSTEM_HPP

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <artemis/image/Pixel.h>
#include <artemis/extractor/SRGBColorSystem.h>

/**
* @author 009
* @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
* @version 1.0.
*/
class XYZColorSystem {

    private:
        const double mXYZConversion[3][3] = {
            {0.4124,0.3576,0.1805},
            {0.2126,0.7152,0.0722},
            {0.0193,0.1192,0.9505}
        };

    public:
        XYZColorSystem();
        ~XYZColorSystem();

        double getXOfXYZ(Pixel p);
        double getYOfXYZ(Pixel p);
        double getZOfXYZ(Pixel p);

};

#endif
