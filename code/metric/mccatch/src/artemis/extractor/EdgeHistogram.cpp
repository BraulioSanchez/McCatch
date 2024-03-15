#include <artemis/extractor/EdgeHistogram.h> 

/**
* Constructor
*/
EdgeHistogram::EdgeHistogram() {
    setVerticalEdge(0.0);
    setHorizontalEdge(0.0);
    setDegree45Edge(0.0);
    setDegree135Edge(0.0);
    setNonDirectionalEdge(0.0);
    setNonEdge(0.0);
}

EdgeHistogram::EdgeHistogram(double vEdge, double hEdge, double d45Edge, double d135Edge, double nDirEdge, double nonEdge ) {
    setVerticalEdge(vEdge);
    setHorizontalEdge(hEdge);
    setDegree45Edge(d45Edge);
    setDegree135Edge(d135Edge);
    setNonDirectionalEdge(nDirEdge);
    setNonEdge(nonEdge);
}

/**
* Destructor.
*/
EdgeHistogram::~EdgeHistogram() {
}

void EdgeHistogram::setVerticalEdge(double value){
	verticalEdge = value;
}

void EdgeHistogram::setHorizontalEdge(double value){
	horizontalEdge = value;
}

void EdgeHistogram::setDegree45Edge(double value){
	degree45Edge = value;
}

void EdgeHistogram::setDegree135Edge(double value){
	degree135Edge = value;
}

void EdgeHistogram::setNonDirectionalEdge(double value){
	nonDirectionalEdge = value;
}

void EdgeHistogram::setNonEdge(double value){
	nonEdge = value;
}

double EdgeHistogram::getVerticalEdge(){
	return verticalEdge;
}

double EdgeHistogram::getHorizontalEdge(){
	return horizontalEdge;
}

double EdgeHistogram::getDegree45Edge(){
	return degree45Edge;
}

double EdgeHistogram::getDegree135Edge(){
	return degree135Edge;
}

double EdgeHistogram::getNonDirectionalEdge(){
	return nonDirectionalEdge;
}

double EdgeHistogram::getNonEdge(){
	return nonEdge;
}

std::vector<double> EdgeHistogram::serialize(){

    std::vector<double> answer;

    answer.push_back(getVerticalEdge());
    answer.push_back(getHorizontalEdge());
    answer.push_back(getDegree45Edge());
    answer.push_back(getDegree135Edge());
    answer.push_back(getNonDirectionalEdge());

    return answer;
}
