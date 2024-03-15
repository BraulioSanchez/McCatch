/**
* @file
*
* This file contains the structure to store histograms blocks.
*
* @version 1.0
* @author 009
* @author 006
* @date 05-08-2014
*/
#ifndef EDGEHISTOGRAM_HPP
#define EDGEHISTOGRAM_HPP

#include <iostream>
#include <cstdlib>
#include <vector>

/**
* @version 1.0.
*/
class EdgeHistogram {
    private:
		double verticalEdge;
		double horizontalEdge;
		double degree45Edge;
		double degree135Edge;
		double nonDirectionalEdge;
		double nonEdge;

    public:
        EdgeHistogram();
        EdgeHistogram(double vEdge, double hEdge, double d45Edge, double d135Edge, double nDirEdge, double nonEdge = 0.0);
        ~EdgeHistogram();

        void setVerticalEdge(double value);		
		void setHorizontalEdge(double value);
		void setDegree45Edge(double value);
		void setDegree135Edge(double value);
		void setNonDirectionalEdge(double value);
        void setNonEdge(double value);
		
		double getVerticalEdge();
		double getHorizontalEdge();
		double getDegree45Edge();
		double getDegree135Edge();
		double getNonDirectionalEdge();
        double getNonEdge();

        std::vector<double> serialize();
};

#endif
