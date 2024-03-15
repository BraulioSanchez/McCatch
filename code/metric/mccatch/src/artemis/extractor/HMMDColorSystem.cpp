#include <artemis/extractor/HMMDColorSystem.h> 

HMMDColorSystem::HMMDColorSystem() {
}

HMMDColorSystem::~HMMDColorSystem() {
}

void HMMDColorSystem::toHMMD(Pixel p, u_int16_t *hue, u_int8_t *max, u_int8_t *min, u_int8_t *diff){
    //rgb to hmmd
    int32_t red, green, blue;

    red = static_cast<int32_t> (p.getRedPixelValue());
    green = static_cast<int32_t> (p.getGreenPixelValue());
    blue = static_cast<int32_t> (p.getBluePixelValue());

    double maxAux = std::max(std::max(red,green), std::max(green,blue));
    double minAux = std::min(std::min(red,green), std::min(green,blue));
    double hueAux = 0;
    double diffAux = (maxAux - minAux);

    if (diffAux == 0)
        hueAux = 0;
    else if ((red == maxAux) && ((green - blue) > 0))
        hueAux = 60*(green-blue)/(maxAux-minAux);
    else if ((red == maxAux) && ((green - blue) < 0))
        hueAux = 60*(green-blue)/(maxAux-minAux) + 360;
    else if (green == maxAux){
        hueAux = (double) (60*(2+(blue-red)/((maxAux)-(minAux))));}
    else if (blue == maxAux)
        hueAux = (double) (60*(4+(red-green)/(maxAux-minAux)));

    *hue = static_cast<u_int16_t> (hueAux);             //range [0,360]
    *max = static_cast<u_int8_t> (maxAux);              //range [0,255]
    *min = static_cast<u_int8_t> (minAux);              //range [0,255]
    *diff = static_cast<u_int8_t> (*max - *min);		//range [0,255]
}
