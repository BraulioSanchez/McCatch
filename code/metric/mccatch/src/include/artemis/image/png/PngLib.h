/**
* @file
*
* This file defines a Class to manipulate png images. 
* All methods required by ImageBase are implemented.
*
* @version 1.0
*/

#ifndef PNGLIB_HPP
#define PNGLIB_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <artemis/image/ImageBase.h>

/**
* This is a base class to manipulate png files
*
* @brief Handler Class to BMP Images
* @author 006
* @see ImageBase
* @see pnglib.cpp
* @version 1.0
*/
class PNGImage : public Image{

    private:
        void loadPixelMatrix() throw (std::runtime_error);

    public:
        PNGImage();
        PNGImage(std::string filename);
        virtual ~PNGImage();

        void openImage(std::string filename) throw (std::runtime_error);
        void saveToFile(std::string filename) throw (std::runtime_error);
        
        PNGImage* clone();
};
#endif
