//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------
#ifndef microclusterH
#define microclusterH

// Metric Tree includes
#include <arboretum/stMetricTree.h>
#include <arboretum/stPlainDiskPageManager.h>
#include <arboretum/stDiskPageManager.h>
#include <arboretum/stMemoryPageManager.h>
#include <arboretum/stSlimTree.h>
#include <arboretum/stMetricTree.h>

#include <string.h>
#include <fstream>

#include <unordered_set>
#include <cmath>
#include <algorithm>

#include <iostream>

// My objects
#include "graph.cpp"

//---------------------------------------------------------------------------
// Class MicroCluster
//---------------------------------------------------------------------------
/**
 * @version 1.0
 */
class MicroCluster
{
public:
    /**
     * Default constructor.
     */
    MicroCluster()
    {
    } // end MicroCluster

    void addMember(TGraph *member)
    {
        members.insert(members.end(), member);
    }

    vector<TGraph *> getMembers() {
        return members;
    }

    int size()
    {
        return members.size();
    }

    void setScore(double s)
    {
        this->s = s;
    }

    double getScore()
    {
        return s;
    }

private:
    vector<TGraph *> members;

    double s;

}; // end MicroCluster

#endif // end microclusterH