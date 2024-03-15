#include <artemis/extractor/SRGBColorSystem.h> 

/**
* Constructor
*/
SRGBColorSystem::SRGBColorSystem() {
}

/**
* Destructor.
*/
SRGBColorSystem::~SRGBColorSystem() {
}


/**
* Converts a pixel in RGB to R channel.
*/
double SRGBColorSystem::getROfSRGB(Pixel p){

    double aux;

    if (p.getRedPixelValue() < 10.0164)
        aux = (double)p.getRedPixelValue()/(255*12.92);
    else
        aux = pow(((((double)p.getRedPixelValue())/255)+0.055)/1.055,2.4);

	return aux;
}

/**
* Converts a pixel in RGB to G channel.
*/
double SRGBColorSystem::getGOfSRGB(Pixel p){

    double aux;

    if(p.getGreenPixelValue() < 10.0164)
        aux = (double)p.getGreenPixelValue()/(255*12.92);
    else
        aux = pow(((((double)p.getGreenPixelValue())/255)+0.055)/1.055,2.4);
	return aux;
}

/**
* Converts a pixel in RGB to B channel.
*/
double SRGBColorSystem::getBOfSRGB(Pixel p){

    double aux;

    if(p.getBluePixelValue() < 10.0164)
        aux = (double)p.getBluePixelValue()/(255*12.92);
    else
        aux = pow((((double)(p.getBluePixelValue())/255)+0.055)/1.055,2.4);
	return aux;
}
