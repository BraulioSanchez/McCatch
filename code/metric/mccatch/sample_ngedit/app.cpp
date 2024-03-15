//---------------------------------------------------------------------------
// app.cpp - Implementation of the application.
//
//---------------------------------------------------------------------------
#ifndef appH
#define appH

#include <chrono>

// Metric Tree includes
#include <arboretum/stMetricTree.h>
#include <arboretum/stPlainDiskPageManager.h>
#include <arboretum/stDiskPageManager.h>
#include <arboretum/stMemoryPageManager.h>
#include <arboretum/stSlimTree.h>
#include <arboretum/stMetricTree.h>

// #include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <tuple>
#include <cmath>
#include <algorithm>

#pragma hdrstop
#pragma package(smart_init)

// My objects
#include "graph.cpp"
#include "oplot.cpp"
#include "ncsj.cpp"
#include "csj.cpp"
#include "microcluster.cpp"

//---------------------------------------------------------------------------
// class TApp
//---------------------------------------------------------------------------
class TApp
{
public:
   /**
    * This is the type of the Slim-Tree defined by TGraph and
    * TNameDistanceEvaluator.
    */
   typedef stSlimTree<TGraph, TGraphDistanceEvaluator> mySlimTree;

   /**
    * Creates a new instance of this class.
    */
   TApp()
   {
   } // end TApp

   /**
    * Init the application:
    */

   void Init(int argc, char *argv[])
   {
      if (argc < 2 || argc > 6)
      {
         cerr << "Invalid number of params!\n";
         exit(1);
      }
      if (argc == 2)
      {
         this->fileDataset = argv[1];
      }
      if (argc == 3)
      {
         this->fileDataset = argv[1];
         this->a = stoi(argv[2]);
      }
      if (argc == 4)
      {
         this->fileDataset = argv[1];
         this->a = stoi(argv[2]);
         this->b = stod(argv[3]);
      }
      if (argc == 5)
      {
         this->fileDataset = argv[1];
         this->a = stoi(argv[2]);
         this->b = stod(argv[3]);
         this->c = stod(argv[4]);
      }
      if (argc == 6)
      {
         this->fileDataset = argv[1];
         this->a = stoi(argv[2]);
         this->b = stod(argv[3]);
         this->c = stod(argv[4]);
         this->output = stoi(argv[5]);
      }

      // cout << "[Metric McCatch]\n";
      // cout << "Hyperparameters readed:\n";
      // cout << "fileDataset = [" << fileDataset << "]\n";
      // cout << "a = [" << a << "]\n";
      // cout << "b = [" << b << "]\n";
      // cout << "c = [" << c << "]\n";
      // cout << "output = [" << output << "]\n";
   }

   /**
    * Runs the application.
    */
   void Run()
   {
      // BEGIN: Define the neighborhood radii
      vector<TGraph *> P = loadDataset(fileDataset); // Lets load the tree with a lot values from the file
      mySlimTree *T = buildTree(P);
      double *R = calculateRadii(T);
      // END: Define the neighborhood radii

      // BEGIN: Build the 'Oracle' plot
      OPlot *O = buildOPlot(P, T, R);
      // END: Build the 'Oracle' plot

      // BEGIN: Spot the microclusters
      vector<MicroCluster *> M = spotMCs(P, O, R);
      // END: Spot the microclusters

      // BEGIN: Compute the anomaly scores
      double *W = scoreMCs(P, M, O, R);
      // END: Compute the anomaly scores

      // if (this->output == 0 || this->output == 1)
      // { // output == 0, full output
      //    for (int j = 0; j < M.size(); j++)
      //    {
      //       for (int i = 0; i < M[j]->size(); i++)
      //       {
      //          cout << "mj" << j << "," << M[j]->getMembers()[i]->GetOID() << "," << M[j]->getScore() << endl;
      //       }
      //    }
      //    if (this->output == 1)
      //    { // only microclusters output
      //       exit(0);
      //    }
      // }

      // // output == 2, only score per point
      // cout << "oid,#edges,score,gt" << endl;
      // for (int i = 0; i < P.size(); i++)
      // {
      //    cout << P[i]->GetOID() << "," << P[i]->GetEdges().size() << "," << W[i] << "," << y_true[i] << endl;
      // }

      // Release resources
      done(T);
   } // end Run

private:
   /**
    * Input file
    */
   char *fileDataset;
   /**
    * a
    */
   int a = 15;

   /**
    * b
    */
   double b = 0.1;

   /**
    * c
    */
   double c = 0.1;

   /**
    * Output type to choose.
    */
   int output = 0;

   /**
    * y_true, to compute the AUROC
    */
   vector<double> y_true;

   vector<TGraph *> loadDataset(char *fileDataset)
   {
      ifstream infile(fileDataset);
      string line;
      vector<string> fields;
      vector<TGraph *> P;

      while (getline(infile, line))
      {
         long oid = stol(line.substr(0, line.find('\t')));

         int y = stoi(line.substr(line.find_last_of('\t') + 1));
         y_true.push_back(y);

         // Splitting the string by ';'
         vector<string> v;
         istringstream iss(line.substr(line.find('\t') + 1));
         string token;
         while (getline(iss, token, ';')) {
            v.push_back(token);
         }

         // Splitting each substring by ','
         vector<vector<int>> edges;
         for (auto str : v) {
            int comma_pos = str.find(',');
            int first = stoi(str.substr(0, comma_pos));
            int second = stoi(str.substr(comma_pos + 1));
            edges.emplace_back(vector<int>{first, second});
         }

         P.push_back(new TGraph(oid, edges));

         v.clear();
         edges.clear();
      }

      infile.close();

      return P;
   }

   mySlimTree *buildTree(vector<TGraph *> P)
   {
      int treePageSize = 4096;

      // create for Slim-Tree
      mySlimTree *T = new mySlimTree(new stMemoryPageManager(treePageSize));
      T->SetSplitMethod(stSlimTree<TGraph, TGraphDistanceEvaluator>::smSPANNINGTREE);
      T->SetChooseMethod(stSlimTree<TGraph, TGraphDistanceEvaluator>::cmMINDIST);

      for (TGraph *object : P)
         T->Add(object);
      T->Optimize();

      return T;
   } // end buildTree

   mySlimTree *buildTree(unordered_set<TGraph *> P)
   {
      int treePageSize = 4096;

      // create for Slim-Tree
      mySlimTree *T = new mySlimTree(new stMemoryPageManager(treePageSize));
      T->SetSplitMethod(stSlimTree<TGraph, TGraphDistanceEvaluator>::smSPANNINGTREE);
      T->SetChooseMethod(stSlimTree<TGraph, TGraphDistanceEvaluator>::cmMINDIST);

      for (const auto &object : P)
         T->Add(object);
      T->Optimize();

      return T;
   } // end buildTree

   double *calculateRadii(mySlimTree *T)
   {
      double *R = new double[a + 1];
      double l = T->GetDistanceLimit();
      for (int i = 0; i < a + 1; i++)
         R[i] = l / pow(2, i);

      return R;
   } // end calculateRadii

   OPlot *buildOPlot(vector<TGraph *> P, mySlimTree *T, double *R)
   {
      int n = P.size();
      c = ceil(n * c);

      OPlot *O = new OPlot(n);

      // BEGIN: Count the neighbors
      int **q = new int *[n];
      for (int i = 0; i < n; i++)
         q[i] = new int[a + 1];
      int *qe = new int[n];
      bool *enoughNeighbors = new bool[n];
      for (int i = 0; i < n; i++)
         enoughNeighbors[i] = false;

      vector<TGraph *> ids;
      for (TGraph *object : P)
         ids.insert(ids.end(), object);

      NCSJ *ncsj = new NCSJ();

      // create for Slim-Tree
      int treePageSize = 4096;
      mySlimTree *_T = buildTree(P);

      // from small to large radius
      for (int e = a; e >= 0; e--)
      { // begin FOR
         for (int i = 0; i < n; i++)
            qe[i] = 0;
         if (!ids.empty())
         {
            if (ids.size() != n)
               ncsj->simJoin(T, _T, qe, R[e]); // simJoin
            else
               ncsj->simJoin(_T, _T, qe, R[e]); // selfJoin
         }

         if (!ids.empty())
         {
            ids.clear();
            for (TGraph *object : P)
            {
               int i = (object->GetOID() - 1);
               {
                  if (qe[i] > c)
                     enoughNeighbors[i] = true;
                  if (!enoughNeighbors[i])
                     ids.insert(ids.end(), object);
                  q[i][e] = qe[i]; // stores the neighbors count
               }
            }
            if (ids.size() != n)
            {
               delete _T;
               _T = new mySlimTree(new stMemoryPageManager(treePageSize));
               this->loadTree(ids, _T);
               _T->Optimize();
            }
         } // end if
      }    // end FOR
      for (int i = 0; i < n; i++)
         q[i][0] = n; // completes the neighbors count
      // END: Count the neighbors

      // BEGIN: Find the plateaus
      double **slopes = new double *[n];
      for (int i = 0; i < n; i++)
         slopes[i] = new double[a];

      reverse(R, R + (a + 1)); // reverse radii

      for (TGraph *object : P)
      {
         int i = (object->GetOID() - 1);

         reverse(q[i], q[i] + (a + 1)); // reverse qi

         // computes slopes by each point
         slopes[i][0] = (log(q[i][1]) - log(q[i][0])) //
                        / (log(R[1]) - log(R[0]));
         int endFirstPlateau = 0;
         int excused = a;
         for (int e = 1; e < a; e++)
         { // begin FOR
            if (q[i][e] > (int)c)
            { // avoid excused zone
               excused = e - 1;
               break;
            }
            if (q[i][e] < 2)
               endFirstPlateau = e; // end of first plateau

            slopes[i][e] = (log(q[i][e + 1]) - log(q[i][e])) //
                           / (log(R[e + 1]) - log(R[e]));
         } // end FOR

         double xi = R[endFirstPlateau];
         double yi = 0.;

         // look for middle plateau
         for (int e = endFirstPlateau; e < excused;)
         { // begin FOR
            if (slopes[i][e] <= b && q[i][e + 1] < (int)c)
            { // begin IF
               int _e = e + 1;
               for (; _e < a;)
               { // begin FOR
                  if (slopes[i][_e] <= b && q[i][_e + 1] < (int)c)
                     _e++;
                  else
                     break;
               } // end FOR

               if (_e < a)
                  yi = R[_e] - R[e]; // have middle plateau
               else
                  cout << (i + 1) << " does not have middle plateau!" << endl;

               e = _e;
            } // end IF
            else
               e++;
         } // end FOR

         // BEGIN: Build the 'Oracle' plot
         O->GetX()[i] = xi;
         O->GetY()[i] = yi;
         // END: Build the 'Oracle' plot
      }
      // END: Find the plateaus
      for (int i = 0; i < n; i++)
         delete[] q[i];
      delete[] q;
      delete[] qe;
      delete[] enoughNeighbors;
      delete ncsj;
      for (int i = 0; i < n; i++)
         delete[] slopes[i];
      delete[] slopes;
      freeContainer(ids);
      done(_T);
      return O;
   }

   vector<MicroCluster *> spotMCs(vector<TGraph *> P, OPlot *O, double *R)
   {
      // BEGIN: Compute the Cutoff d
      // Build the Histogram of 1NN Distances H
      int *H = new int[a + 1];
      for (int i = 0; i < (a + 1); i++)
         H[i] = 0;
      for (TGraph *object : P)
      {
         int i = (object->GetOID() - 1);
         auto itr = find(R, R + (a + 1), O->GetX()[i]);
         int e = distance(R, itr);
         H[e]++;
      }

      // Data-driven computation of d
      int _e = 0;
      for (int j = 0; j < (a + 1); j++)
      {
         if (H[_e] < H[j])
            _e = j;
      }
      double H_[(a + 1) - _e];
      for (int j = 0; j < (a + 1) - _e; j++)
         H_[j] = H[j + _e];
      int __e = MDL(H_, sizeof(H_) / sizeof(H_[0]));
      double d = R[__e + _e];
      // END: Compute the Cutoff d

      // BEGIN: Gel the outliers into microclusters
      unordered_set<TGraph *> A;
      // Gel nonsingleton microclusters
      unordered_set<TGraph *> M_;
      double upx = -numeric_limits<double>::max();
      int e = 0;
      for (TGraph *object : P)
      {
         int i = (object->GetOID() - 1);
         if (O->GetX()[i] >= d || O->GetY()[i] >= d) // All outliers
            A.insert(object);
         if (O->GetY()[i] >= d)
         { // Large Group 1NN D.
            M_.insert(object);
            if (upx < O->GetX()[i]) // Largest 1NN Dist.
               upx = O->GetX()[i];
         }
      }
      auto itr = find(R, R + (a + 1), upx);
      e = distance(R, itr);

      vector<MicroCluster *> M;
      if (!M_.empty())
      {
         // create for Slim-Tree
         mySlimTree *T = buildTree(M_);

         vector<unordered_set<TGraph *>> E;
         // CSJ *csj = new CSJ();
         CSJ *csj = new CSJ(true);
         csj->simJoin(T, T, R[e + 1], E); // Find neighbors
         for (int i = 0; i < E.size(); i++)
         { // Build neighborhood graph
            unordered_set<TGraph *> Gi = E[i];
            for (int j = i + 1; j < E.size(); j++)
            {
               unordered_set<TGraph *> Gj = E[j];
               for (const auto &object : Gj)
               {
                  if (Gi.contains(object))
                  { // Find components
                     for (const auto &object_ : Gj)
                        Gi.insert(object_);
                     E.erase(E.begin() + j);
                     j--;
                     break;
                  }
               }
               freeContainer(Gj);
            }
            freeContainer(Gi);
         }
         for (unordered_set<TGraph *> Gj : E)
         {
            MicroCluster *Mj = new MicroCluster();
            for (TGraph *object : Gj)
               Mj->addMember(object);
            M.insert(M.end(), Mj);
         }

         delete csj;
         done(T);
      }

      // Gel singleton microclusters
      for (TGraph *object : A)
      {
         if (!M_.contains(object))
         {
            MicroCluster *Mj = new MicroCluster();
            Mj->addMember(object);
            M.insert(M.end(), Mj);
         }
      }
      // END: Gel the outliers into microclusters
      delete[] H;
      freeContainer(A);
      freeContainer(M_);
      return M;
   }

   double *scoreMCs(vector<TGraph *> P, vector<MicroCluster *> M, OPlot *O, double *R)
   {
      int n = P.size();
      tuple<int, int> tuple_ = distinctCharsAndLongestWord(P);
      int t_1 = get<0>(tuple_); // # distinct chars.
      int t_2 = get<1>(tuple_); // # chars. longest word
      int t = universalCodeLength(3) + universalCodeLength(t_1) + universalCodeLength(t_2);

      // BEGIN: Compute the distances to the nearest inliers
      // For outliers
      unordered_set<TGraph *> A; // All outliers
      for (MicroCluster *Mj : M)
      {
         for (TGraph *object : Mj->getMembers())
            A.insert(object);
      }
      vector<TGraph *> Inliers;
      for (TGraph *object : P)
      { // All inliers
         if (!A.contains(object))
            Inliers.insert(Inliers.end(), object);
      }

      double *g = new double[n];
      for (int i = 0; i < n; i++)
         g[i] = 0;
      if (!A.empty())
      {
         mySlimTree *T = buildTree(A); // Tree for outliers

         mySlimTree *_T = buildTree(Inliers); // Tree for inliers

         NCSJ *ncsj = new NCSJ();
         int expectedSizeOfOutliers = 0;
         bool *enoughNeighbors = new bool[n];
         for (int i = 0; i < n; i++)
            enoughNeighbors[i] = false;
         int *f = new int[n];
         for (int e = 0; e <= a; e++)
         { // from small to large radii
            for (int i = 0; i < n; i++)
               f[i] = 0;
            ncsj->simJoin(T, _T, f, R[e]); // simJoin

            for (TGraph *object : A)
            {
               int i = (object->GetOID() - 1);
               if (f[i] > 0 && !enoughNeighbors[i])
               {
                  expectedSizeOfOutliers++;
                  enoughNeighbors[i] = true;
                  g[i] = R[e - 1 >= 0 ? e - 1 : 0]; // gi = distance to the nearest inlier
                                                    //						T.delete(pi);
               }
            }

            if (expectedSizeOfOutliers == A.size())
               break;
         }

         delete ncsj;
         delete[] enoughNeighbors;
         done(T);
         done(_T);
      }

      // For inliers
      for (TGraph *object : Inliers)
      {
         int i = (object->GetOID() - 1);
         g[i] = O->GetX()[i]; // gi is the 1NN Distance of pi
      }
      // END: Compute the distances to the nearest inliers

      // BEGIN: Compute the scores per microcluster
      for (MicroCluster *Mj : M)
      {
         double downgj = numeric_limits<double>::max();
         double averagexj = 0.;
         for (TGraph *object : Mj->getMembers())
         {
            int i = (object->GetOID() - 1);
            if (downgj > g[i])
            {
               downgj = g[i]; // ‘Bridge’s Lgth.’
            }
            averagexj += O->GetX()[i];
         }
         averagexj /= Mj->size();

         double _1_ = universalCodeLength(Mj->size());                            // Cardinality
         double _2_ = universalCodeLength(n);                                     // Nearest inlier
         double _3_ = t * universalCodeLength((int)(downgj / R[0]));              // ‘Bridge’s Length’
         double _4_ = t * universalCodeLength((int)(1 + ceil(averagexj / R[0]))); // Average 1NN Dist.
         double sj = (_1_ + _2_ + _3_ + (Mj->size() - 1) * _4_) / Mj->size();
         Mj->setScore(sj); // Set of scores per mc
      }
      // END: Compute the scores per microcluster

      // BEGIN: Compute the scores per point
      double *W = new double[n];
      for (TGraph *object : P)
      {
         int i = (object->GetOID() - 1);
         double wi = universalCodeLength((int)(1 + g[i] / R[0]));
         W[i] = wi;
      }
      // END: Compute the scores per point
      delete[] g;
      freeContainer(A);
      freeContainer(Inliers);
      return W;
   }

   /**
    * Loads the tree from file with a set of objects.
    */
   void loadTree(vector<TGraph *> P, mySlimTree *SlimTree)
   {
      for (TGraph *object : P)
         SlimTree->Add(object);
      SlimTree->Optimize();
   } // end loadTree

   /**
    * Deinitialize the objects.
    */
   void done(mySlimTree *T)
   {
      if (T != NULL)
         delete T;
   } // end done

   template <typename T>
   inline void freeContainer(T &p_container)
   {
      T empty;
      swap(p_container, empty);
   }

   int MDL(double array[], size_t arraySize)
   {
      int cutPoint = -1;
      int numberOfDimensions = arraySize;
      double preAverage, postAverage, descriptionLength, minimumDescriptionLength;
      for (int i = 0; i < numberOfDimensions; i++)
      {
         descriptionLength = 0;
         // calculates the average of both sets
         preAverage = 0;
         for (int j = 0; j < i; j++)
         {
            preAverage += array[j];
         } // end for
         if (i)
         {
            preAverage /= i;
            descriptionLength += (ceil(preAverage)) ? (log10(ceil(preAverage)) / log10((double)2)) : 0; // changes the log base from 10 to 2
         }                                                                                              // end if
         postAverage = 0;
         for (int j = i; j < numberOfDimensions; j++)
         {
            postAverage += array[j];
         } // end for
         if (numberOfDimensions - i)
         {
            postAverage /= (numberOfDimensions - i);
            descriptionLength += (ceil(postAverage)) ? (log10(ceil(postAverage)) / log10((double)2)) : 0; // changes the log base from 10 to 2
         }                                                                                                // end if
         // calculates the description length
         for (int j = 0; j < i; j++)
         {
            descriptionLength += (ceil(fabs(preAverage - array[j]))) ? (log10(ceil(fabs(preAverage - array[j]))) / log10((double)2)) : 0; // changes the log base from 10 to 2
         }                                                                                                                                // end for
         for (int j = i; j < numberOfDimensions; j++)
         {
            descriptionLength += (ceil(fabs(postAverage - array[j]))) ? (log10(ceil(fabs(postAverage - array[j]))) / log10((double)2)) : 0; // changes the log base from 10 to 2
         }                                                                                                                                  // end for
         // verify if this is the best cut point
         if (cutPoint == -1 || descriptionLength < minimumDescriptionLength)
         {
            cutPoint = i;
            minimumDescriptionLength = descriptionLength;
         } // end if
      }    // end for
      return cutPoint;

   } // end MDL

   double universalCodeLength(int x)
   {
      double inner = x;
      double result = 0.;
      while (inner > 0)
      {
         inner = log2(inner);
         if (inner > 0)
            result += inner;
      }
      return result;
   } // end universalCodeLength

   tuple<int, int> distinctCharsAndLongestWord(vector<TGraph *> P)
   {
      // return tuple_;
      return make_tuple(1, 1);
   } // end distinctCharsAndLongestWord
};   // end TApp

#endif // end appH
