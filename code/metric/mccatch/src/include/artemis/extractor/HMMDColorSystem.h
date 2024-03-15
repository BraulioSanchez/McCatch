/**
* @file
*
* This file contains the definition of the YUVColorSystem class.
*
* @version 1.0
*/
#ifndef HMMDCOLORSYSTEM_H
#define HMMDCOLORSYSTEM_H

#include <artemis/image/Pixel.h>

/**
* @author 009
* @author 006
* @version 1.0.
*/
class HMMDColorSystem {

    public:
        HMMDColorSystem();
        ~HMMDColorSystem();

        void toHMMD(Pixel p, u_int16_t *hue, u_int8_t *max, u_int8_t *min, u_int8_t *diff);
};

#endif
