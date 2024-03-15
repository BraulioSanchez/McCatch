/**
* @file
*
* This file defines a Class to open a single File from disk.
*
* @version 1.0
* @author 005
* @author 006
* @date 11-01-2010
*/
#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <stdexcept>

/**
* This Class is a Handler Class for files.
*
* @brief Handler Class for Files
* @author 005
* @author 006
* @version 1.0 
*/

class FileHandler {
      
    private :
        int size;
        int type;
		
    public :
        std::fstream file;
        static const int FILE_IN = 0;
        static const int FILE_OUT = 1;
        static const int FILE_OUT_APP = 2;
        static const int FILE_BINARY = 3;
		
	public:
	    FileHandler();
            FileHandler(std::string name, int type) throw (std::runtime_error);
            ~FileHandler();
		
            void setSize(int size);
		
            int getSize();
            char * getNextLine();
            char * getNextWord(char * &line);
        
            char * readFile(int size);
            char * readBuffer(char * &buffer, int numByte);

            void endianSwap(u_int16_t& x);
            void close();
       
            static bool fileExists( std::string fileName);
            bool endOfFile();
		
};

#endif
