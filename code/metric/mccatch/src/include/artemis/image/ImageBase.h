/**
* @file
*
* This file defines a base class to manipulate images.
*
* @version 1.0
*/
#ifndef IMAGEBASE_HPP
#define IMAGEBASE_HPP

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <cmath>

#include <artemis/image/Pixel.h>


/**
* This is a base class to manipulate images.
* Its structures are also used by the feature extractors provided by the
* Image Extractors library.
*
*
* @brief Generic class of Image.
* @author 006
* @author 005
* @author 004
* @version 1.0
* @see Pixel
*/
class Image {

    private:
        std::string filename;
        u_int32_t width; //lg x
        u_int32_t height; //alt
        u_int16_t nchannels;
        u_int32_t imageID;
        Pixel **pixel;
        u_int16_t bitsPerPixel;

    public:
        Image();
        Image(const Image & i);
        virtual ~Image();

        void setFilename(std::string filename);
        void setImageID(u_int32_t imageID);
        void setChannels(u_int16_t nchannels);
        void setPixel(u_int32_t x, u_int32_t y, const Pixel & pixel) throw (std::length_error, std::runtime_error);
        void setBitsPerPixel(u_int16_t bitsPerPixel);
        void setWidth(u_int32_t width);
        void setHeight(u_int32_t height);

        u_int32_t getSize() const;
        std::string getFilename() const;
        u_int32_t getWidth() const;
        u_int32_t getHeight() const;
        u_int32_t getImageID() const;
        u_int16_t getChannels() const;
        const Pixel & getPixel(u_int32_t x, u_int32_t y) const throw (std::length_error, std::runtime_error);
        u_int16_t getBitsPerPixel() const;

        void createPixelMatrix(u_int32_t width, u_int32_t height);
        void deletePixelMatrix();
        void toGrayScale();

        Image * clone() const;
        bool isEqual(const Image & i) const;
};

#endif

