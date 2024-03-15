#include <artemis/extractor/XYZColorSystem.h> 

/**
* Constructor
*/
XYZColorSystem::XYZColorSystem() {
}

/**
* Destructor.
*/
XYZColorSystem::~XYZColorSystem() {
}

/**
* Converts a pixel in RGB to X channel.
*/
double XYZColorSystem::getXOfXYZ(Pixel p){

    SRGBColorSystem aux;
    return mXYZConversion[0][0]*aux.getROfSRGB(p)+mXYZConversion[0][1]*aux.getGOfSRGB(p)+mXYZConversion[0][2]*aux.getBOfSRGB(p);
}

/**
* Converts a pixel in RGB to Y channel.
*/
double XYZColorSystem::getYOfXYZ(Pixel p){

    SRGBColorSystem aux;
    return mXYZConversion[1][0]*aux.getROfSRGB(p)+mXYZConversion[1][1]*aux.getGOfSRGB(p)+mXYZConversion[1][2]*aux.getBOfSRGB(p);
}

/**
* Converts a pixel in RGB to Z channel.
*/
double XYZColorSystem::getZOfXYZ(Pixel p){

    SRGBColorSystem aux;
    return mXYZConversion[2][0]*aux.getROfSRGB(p)+mXYZConversion[2][1]*aux.getGOfSRGB(p)+mXYZConversion[2][2]*aux.getBOfSRGB(p);
}
