#include <artemis/extractor/YCrCbColorSystem.h> 

/**
* Constructor
*/
YCrCbColorSystem::YCrCbColorSystem() {
}

/**
* Destructor.
*/
YCrCbColorSystem::~YCrCbColorSystem() {
}

/**
* Converts a pixel in YCCrCb to gray scale.
*/
u_int8_t YCrCbColorSystem::toGrayScale(u_int16_t y, u_int16_t u, u_int16_t v){

    return y;
}

/**
* Converts a pixel in RGB to Y channel.
*/
u_int8_t YCrCbColorSystem::getYOfYCrCb(Pixel p){

    u_int8_t aux = 16 + ((65.738*p.getRedPixelValue()/256) + (129.057*p.getGreenPixelValue()/256) + (25.064*p.getBluePixelValue()/256));

    // limit value range from 0 to 255
    if(aux < 0) {
        aux = 0;
    }
    if(aux > 255) {
        aux = 255;
    }
    return aux;
}

/**
* Converts a pixel in RGB to Cr channel.
*/
u_int8_t YCrCbColorSystem::getCrOfYCrCb(Pixel p){

    u_int8_t aux = 128 + ((112.439*p.getRedPixelValue()/256) - (94.154*p.getGreenPixelValue()/256) - (18.285*p.getBluePixelValue()/256));
    // limit value range from 0 to 255
    if(aux < 0) {
        aux = 0;
    }
    if(aux > 255) {
        aux = 255;
    }
    return aux;
}

/**
* Converts a pixel in RGB to V channel.
*/
u_int8_t YCrCbColorSystem::getCbOfYCrCb(Pixel p){

    u_int8_t aux = 128 + (112*p.getRedPixelValue() - 93.786*p.getGreenPixelValue() + 18.214*p.getBluePixelValue());
    // limit value range from 0 to 255
    if(aux < 0) {
        aux = 0;
    }
    if(aux > 255) {
        aux = 255;
    }
    return aux;
}
