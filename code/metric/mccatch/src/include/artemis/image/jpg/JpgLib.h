/**
* @file
*
* This file defines a Class to manipulate jpg images. 
* All methods required by ImageBase are implemented.
*
* @version 1.0
* @author 005
* @author 006
* @date 11-01-2010
*/
#ifndef JPGLIB_HPP
#define JPGLIB_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <artemis/image/ImageBase.h>

/**
* Handler class to JPG Image.
*
* @brief Handler Class to JPG Images.
* @author 006.
* @see ImageBase
* @version 1.0
*/
class JPGImage : public Image{

    private:
        void loadPixelMatrix() throw (std::runtime_error);

    public:
        JPGImage();
        JPGImage(std::string filename);
        virtual ~JPGImage();
  
        void openImage(std::string filename) throw (std::runtime_error);

        void saveToFile(std::string filename) throw (std::runtime_error);
        
        JPGImage* clone();
};

#endif

