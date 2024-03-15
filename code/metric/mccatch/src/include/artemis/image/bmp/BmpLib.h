/**
* @file
*
* This file defines a Class to manipulate bitmaps images. 
* All methods required by ImageBase are implemented.
*
* @version 1.0
* @author 006
* @author 005
* @date 11-01-2010
*/

#ifndef BMPLIB_H
#define BMPLIB_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <artemis/image/ImageBase.h>

/**
*
* This Class provide resources to manipulate Bitmap Images.
* All methods required by ImageBase are implemented.
* This class can be used with the Extractors defined in the directory 
* Extractor.
*
*
* @brief Handler Class to BMP Image
* @author 006
* @see ImageBase
* @version 1.0
*/
class BMPImage : public Image{

    private:
        void loadPixelMatrix() throw (std::runtime_error);

    public:
        BMPImage();
        BMPImage(std::string filename);
        virtual ~BMPImage();

        void openImage(std::string filename) throw (std::runtime_error);
             
        void saveToFile(std::string filename) throw (std::runtime_error);
        
        BMPImage* clone();
};

#endif
