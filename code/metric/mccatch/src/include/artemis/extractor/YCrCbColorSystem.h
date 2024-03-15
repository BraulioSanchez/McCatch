/**
* @file
*
* This file contains the definition of the YUVColorSystem class.
*
* @version 1.0
*/
#ifndef YCRCBCOLORSYSTEM_H
#define YCRCBCOLORSYSTEM_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <artemis/image/Pixel.h>

using namespace std;

/**
* @author 009
* @author 006
* @version 1.0.
*/
class YCrCbColorSystem {

    public:
        YCrCbColorSystem();
        ~YCrCbColorSystem();

        u_int8_t toGrayScale(u_int16_t y, u_int16_t u, u_int16_t v);
        u_int8_t getYOfYCrCb(Pixel p);
        u_int8_t getCrOfYCrCb(Pixel p);
        u_int8_t getCbOfYCrCb(Pixel p);
};

#endif //YCRCBCOLORSYSTEM_H
