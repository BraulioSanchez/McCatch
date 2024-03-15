//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------
#ifndef ncsjH
#define ncsjH

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
#include "name.cpp"

//---------------------------------------------------------------------------
// Class NCSJ
//---------------------------------------------------------------------------
/**
 * @version 1.0
 */
class NCSJ
{
public:
    /**
     * Default constructor.
     */
    NCSJ()
    {

    } // end NCSJ

    typedef stSlimTree<TName, TNameDistanceEvaluator> mySlimTree;

    void simJoin(mySlimTree *T, mySlimTree *_T, int neighborsCount[], double range)
    {
        u_int32_t i, j, numberOfEntries;
        double distance;
        stPage *currPageA, *currPageB;
        stSlimNode *currNode;
        TName p, q;

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
                                   distance, range, neighborsCount);
        } // end if

        delete currNode;
        currNode = 0;
        T->GetPageManager()->ReleasePage(currPageA);
        _T->GetPageManager()->ReleasePage(currPageB);
    } // end simJoin

private:
    void simJoinRecursive(u_int32_t pageIDA, double radiusA, mySlimTree *T,
                          u_int32_t pageIDB, double radiusB, mySlimTree *_T,
                          double centersDistance, double range, int neighborsCount[])
    {
        u_int32_t i, j, numberOfEntriesA, numberOfEntriesB;
        TName p, q;
        stPage *currPageA, *currPageB;
        stSlimNode *currNodeA, *currNodeB;
        double distance, minDistance, maxDiameter;

        maxDiameter = centersDistance + (radiusA + radiusB);
        if (maxDiameter <= range)
        {
            this->createGroup(pageIDA, T, pageIDB, _T, neighborsCount);
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
                        neighborsCount[p.GetOID() - 1]++;
                        // neighborsCount[q.GetOID() - 1]++; // if self-join
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
                                       distance, range, neighborsCount);
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
                                       distance, range, neighborsCount);
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
                                           distance, range, neighborsCount);
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

    void createGroup(u_int32_t pageID, mySlimTree *SlimTree, int neighborsCount[])
    {
        std::vector<TName> leafs;
        this->countLeafs(pageID, leafs, SlimTree);
        u_int32_t countLeafEntries = leafs.size();

        countLeafEntries--; // not allowed self similarity

        for (TName object : leafs)
            neighborsCount[object.GetOID() - 1] += countLeafEntries;
    } // end createGroup

    void createGroup(u_int32_t pageIDA, mySlimTree *T,
                     u_int32_t pageIDB, mySlimTree *_T,
                     int neighborsCount[])
    {
        std::unordered_set<long> leafsA;
        this->countLeafs(pageIDA, leafsA, T);
        u_int32_t countLeafEntriesA = leafsA.size();

        std::unordered_set<long> leafsB;
        this->countLeafs(pageIDB, leafsB, _T);
        u_int32_t countLeafEntriesB = leafsB.size();

        for (long OID : leafsA)
        {
            if (leafsB.contains(OID))
            {
                neighborsCount[OID - 1] += (countLeafEntriesB - 1);
            }
            else
            {
                neighborsCount[OID - 1] += countLeafEntriesB;
            }
        }
    } // end createGroup

    void countLeafs(u_int32_t pageID, std::vector<TName> &leafs, mySlimTree *SlimTree)
    {
        u_int32_t i, numberOfEntries;
        TName p;
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
                leafs.push_back(*p.Clone());
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

    void countLeafs(u_int32_t pageID, std::unordered_set<long> &leafs, mySlimTree *SlimTree)
    {
        u_int32_t i, numberOfEntries;
        TName p;
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
                leafs.insert(p.GetOID());
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

}; // end NCSJ

#endif // end ncsjH