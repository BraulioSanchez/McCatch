//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------
#ifndef csjH
#define csjH

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
#include <vector>
#include <cmath>
#include <algorithm>

#include <iostream>

// My objects
#include "graph.cpp"

//---------------------------------------------------------------------------
// Class CSJ
//---------------------------------------------------------------------------
/**
 * @version 1.0
 */
class CSJ
{
public:
    /**
     * Default constructor.
     */
    CSJ()
    {

    } // end CSJ

    CSJ(bool allowSelfSimilar)
    {
        this->allowSelfSimilar = allowSelfSimilar;
    }

    typedef stSlimTree<TGraph, TGraphDistanceEvaluator> mySlimTree;

    void simJoin(mySlimTree *T, mySlimTree *_T, double range, vector<unordered_set<TGraph* >> E)
    {
        u_int32_t i, j, numberOfEntries;
        double distance;
        stPage *currPageA, *currPageB;
        stSlimNode *currNode;
        TGraph p, q;

        // Evaluate the root node.
        if (T->GetRoot() && _T->GetRoot())
        {
            double radiusA = T->GetDistanceLimit() / 2.; // for ADIMENSIONAL data
            double radiusB = _T->GetDistanceLimit() / 2.;

            currPageA = T->GetPageManager()->GetPage(T->GetRoot());
            currNode = stSlimNode::CreateNode(currPageA);
            if (currNode->GetNodeType() == stSlimNode::LEAF)
            {
                stSlimLeafNode *leafNode = (stSlimLeafNode *)currNode;
                int idx = leafNode->GetRepresentativeEntry();
                p.Unserialize(leafNode->GetObject(idx), leafNode->GetObjectSize(idx));
            }
            else
            {
                stSlimIndexNode *indexNode = (stSlimIndexNode *)currNode;
                int idx = indexNode->GetRepresentativeEntry();
                p.Unserialize(indexNode->GetObject(idx), indexNode->GetObjectSize(idx));
            }

            currPageB = _T->GetPageManager()->GetPage(_T->GetRoot());
            currNode = stSlimNode::CreateNode(currPageB);
            if (currNode->GetNodeType() == stSlimNode::LEAF)
            {
                stSlimLeafNode *leafNode = (stSlimLeafNode *)currNode;
                int idx = leafNode->GetRepresentativeEntry();
                q.Unserialize(leafNode->GetObject(idx), leafNode->GetObjectSize(idx));
            }
            else
            {
                stSlimIndexNode *indexNode = (stSlimIndexNode *)currNode;
                int idx = indexNode->GetRepresentativeEntry();
                q.Unserialize(indexNode->GetObject(idx), indexNode->GetObjectSize(idx));
            }

            distance = T->GetMetricEvaluator()->GetDistance(p, q);
            this->simJoinRecursive(T->GetRoot(), radiusA, T,
                                   _T->GetRoot(), radiusB, _T,
                                   distance, range, E);
        } // end if

        delete currNode;
        currNode = 0;
        T->GetPageManager()->ReleasePage(currPageA);
        _T->GetPageManager()->ReleasePage(currPageB);
    } // end simJoin

private:
    bool allowSelfSimilar = true;
    int g = 10;

    void simJoinRecursive(u_int32_t pageIDA, double radiusA, mySlimTree *T,
                          u_int32_t pageIDB, double radiusB, mySlimTree *_T,
                          double centersDistance, double range,
                          vector<unordered_set<TGraph *>> E)
    {
        u_int32_t i, j, numberOfEntriesA, numberOfEntriesB;
        TGraph p, q;
        stPage *currPageA, *currPageB;
        stSlimNode *currNodeA, *currNodeB;
        double distance, minDistance, maxDiameter;

        maxDiameter = centersDistance + (radiusA + radiusB);
        if (maxDiameter <= range)
        {
            this->createGroup(pageIDA, T, pageIDB, _T, E);
            return;
        }

        minDistance = centersDistance - (radiusA + radiusB);
        if (minDistance > range)
            return;

        currPageA = T->GetPageManager()->GetPage(pageIDA);
        currNodeA = stSlimNode::CreateNode(currPageA);

        currPageB = _T->GetPageManager()->GetPage(pageIDB);
        currNodeB = stSlimNode::CreateNode(currPageB);

        if (currNodeA->GetNodeType() == stSlimNode::LEAF && currNodeB->GetNodeType() == stSlimNode::LEAF)
        {
            stSlimLeafNode *leafNodeA = (stSlimLeafNode *)currNodeA;
            numberOfEntriesA = leafNodeA->GetNumberOfEntries();
            stSlimLeafNode *leafNodeB = (stSlimLeafNode *)currNodeB;
            numberOfEntriesB = leafNodeB->GetNumberOfEntries();

            // For each entry...
            for (i = 0; i < numberOfEntriesA; i++)
            {
                // Rebuild the object
                p.Unserialize(leafNodeA->GetObject(i), leafNodeA->GetObjectSize(i));
                for (j = 0; j < numberOfEntriesB; j++)
                {
                    // Rebuild the object
                    q.Unserialize(leafNodeB->GetObject(j), leafNodeB->GetObjectSize(j));
                    if (p.GetOID() == q.GetOID())
                        continue;

                    // Evaluate distance
                    distance = T->GetMetricEvaluator()->GetDistance(p, q);
                    if (distance <= range)
                    {
                        // report results
                        mergeIntoPrevGroup(&p, &q, E);
                    } // end if
                }     // end for
            }         // end for
        }             // end if
        else if (currNodeA->GetNodeType() == stSlimNode::LEAF)
        {
            stSlimLeafNode *leafNodeA = (stSlimLeafNode *)currNodeA;
            stSlimIndexNode *indexNodeB = (stSlimIndexNode *)currNodeB;
            numberOfEntriesB = indexNodeB->GetNumberOfEntries();

            int id = leafNodeA->GetRepresentativeEntry();
            p.Unserialize(leafNodeA->GetObject(id), leafNodeA->GetObjectSize(id));

            // For each entry...
            for (j = 0; j < numberOfEntriesB; j++)
            {
                q.Unserialize(indexNodeB->GetObject(j), indexNodeB->GetObjectSize(j));
                distance = T->GetMetricEvaluator()->GetDistance(p, q);
                this->simJoinRecursive(currNodeA->GetPageID(), radiusA, T,
                                       indexNodeB->GetIndexEntry(j).PageID, indexNodeB->GetIndexEntry(j).Radius, _T,
                                       distance, range, E);
            } // end for
        }     // end else if
        else if (currNodeB->GetNodeType() == stSlimNode::LEAF)
        {
            stSlimIndexNode *indexNodeA = (stSlimIndexNode *)currNodeA;
            numberOfEntriesA = indexNodeA->GetNumberOfEntries();
            stSlimLeafNode *leafNodeB = (stSlimLeafNode *)currNodeB;

            int idx = leafNodeB->GetRepresentativeEntry();
            q.Unserialize(leafNodeB->GetObject(idx), leafNodeB->GetObjectSize(idx));

            // For each entry...
            for (i = 0; i < numberOfEntriesA; i++)
            {
                p.Unserialize(indexNodeA->GetObject(i), indexNodeA->GetObjectSize(i));
                distance = T->GetMetricEvaluator()->GetDistance(p, q);
                this->simJoinRecursive(indexNodeA->GetIndexEntry(i).PageID, indexNodeA->GetIndexEntry(i).Radius, T,
                                       currNodeB->GetPageID(), radiusB, _T,
                                       distance, range, E);
            } // end for
        }     // end else if
        else
        {
            stSlimIndexNode *indexNodeA = (stSlimIndexNode *)currNodeA;
            numberOfEntriesA = indexNodeA->GetNumberOfEntries();
            stSlimIndexNode *indexNodeB = (stSlimIndexNode *)currNodeB;
            numberOfEntriesB = indexNodeB->GetNumberOfEntries();

            // For each entry...
            for (i = 0; i < numberOfEntriesA; i++)
            {
                p.Unserialize(indexNodeA->GetObject(i), indexNodeA->GetObjectSize(i));
                for (j = 0; j < numberOfEntriesB; j++)
                {
                    q.Unserialize(indexNodeB->GetObject(j), indexNodeB->GetObjectSize(j));
                    distance = T->GetMetricEvaluator()->GetDistance(p, q);
                    this->simJoinRecursive(indexNodeA->GetIndexEntry(i).PageID, indexNodeA->GetIndexEntry(i).Radius, T,
                                           indexNodeB->GetIndexEntry(j).PageID, indexNodeB->GetIndexEntry(j).Radius, _T,
                                           distance, range, /*neighborsCount,*/ E);
                } // end for
            }     // end for
        }         // end else

        delete currNodeA;
        currNodeA = 0;
        delete currNodeB;
        currNodeB = 0;
        T->GetPageManager()->ReleasePage(currPageA);
        _T->GetPageManager()->ReleasePage(currPageB);
    } // end simJoinRecursive

    void mergeIntoPrevGroup(TGraph* p, TGraph* q, vector<unordered_set<TGraph *>> E)
    {
        int sizeGroups = E.size();
        int i = sizeGroups > g ? sizeGroups - g : 0;

        for(; i<sizeGroups; i++)
        {
            std::unordered_set<TGraph*> Ej = E[i];
            if(Ej.contains(p) || Ej.contains(q))
            {
                Ej.insert(p);
                Ej.insert(q);
                return;
            }
        }

        // create new group
        std::unordered_set<TGraph*> Ej;
        Ej.insert(p);
        Ej.insert(q);
        E.push_back(Ej);
    }

    void createGroup(u_int32_t pageIDA, mySlimTree *T,
                     u_int32_t pageIDB, mySlimTree *_T,
                     vector<unordered_set<TGraph *>> E)
    {
        std::unordered_set<TGraph*> leafs;
        this->countLeafs(pageIDA, leafs, T);
        this->countLeafs(pageIDB, leafs, _T);

        std::unordered_set<TGraph*> Ej;
        for (const auto &object : leafs)
        {
            Ej.insert(object);
        }
        E.push_back(Ej);
    } // end createGroup

    void countLeafs(u_int32_t pageID, std::unordered_set<TGraph*> &leafs, mySlimTree *SlimTree)
    {
        u_int32_t i, numberOfEntries;
        TGraph p;
        stPage *currPage;
        stSlimNode *currNode;

        currPage = SlimTree->GetPageManager()->GetPage(pageID);
        currNode = stSlimNode::CreateNode(currPage);

        if (currNode->GetNodeType() == stSlimNode::LEAF)
        {
            stSlimLeafNode *leafNode = (stSlimLeafNode *)currNode;
            numberOfEntries = leafNode->GetNumberOfEntries();
            // For each entry...
            for (i = 0; i < numberOfEntries; i++)
            {
                // Rebuild the object
                p.Unserialize(leafNode->GetObject(i), leafNode->GetObjectSize(i));
                leafs.insert(&p);
            } // end for
        }
        else
        {
            // Get Index node
            stSlimIndexNode *indexNode = (stSlimIndexNode *)currNode;
            numberOfEntries = indexNode->GetNumberOfEntries();
            // For each entry...
            for (i = 0; i < numberOfEntries; i++)
                this->countLeafs(indexNode->GetIndexEntry(i).PageID, leafs, SlimTree);
        } // end else

        delete currNode;
        currNode = 0;
        SlimTree->GetPageManager()->ReleasePage(currPage);
    } // end countLeafs

}; // end CSJ

#endif // end csjH