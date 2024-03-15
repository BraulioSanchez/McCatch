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
#include "name.cpp"
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
    * This is the type of the Slim-Tree defined by TName and
    * TNameDistanceEvaluator.
    */
   typedef stSlimTree<TName, TNameDistanceEvaluator> mySlimTree;

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
      if (argc < 2 || argc > 5)
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

      cout << "[Metric McCatch]\n";
      cout << "Hyperparameters readed:\n";
      cout << "fileDataset = [" << fileDataset << "]\n";
      cout << "a = [" << a << "]\n";
      cout << "b = [" << b << "]\n";
      cout << "c = [" << c << "]\n";
   }

   /**
    * Runs the application.
    */
   void Run()
   {
      // BEGIN: Define the neighborhood radii
      vector<TName *> P = loadDataset(fileDataset); // Lets load the tree with a lot values from the file
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

      {
         cout << "\nOID,Name,Score" << endl;
         for (int i = 0; i < P.size(); i++)
            cout << P[i]->GetOID() << "," << P[i]->GetName() << "," << W[i] << endl;
      }

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
    * y_true, to compute the AUROC
    */
   vector<double> y_true;

   vector<TName *> loadDataset(char *fileDataset)
   {
      ifstream infile(fileDataset);
      string line;
      vector<string> fields;
      vector<TName *> P;

      while (getline(infile, line))
      {
         istringstream iss(line);
         string field;
         while (getline(iss, field, ','))
         {
            fields.push_back(field);
         }
         P.insert(P.end(), new TName(std::stol(fields[0]), fields[1]));
         y_true.push_back(std::stod(fields[2]));
         fields.clear();
      }

      infile.close();

      return P;
   }

   mySlimTree *buildTree(vector<TName *> P)
   {
      int treePageSize = 4096;

      // create for Slim-Tree
      mySlimTree *T = new mySlimTree(new stMemoryPageManager(treePageSize));
      T->SetSplitMethod(stSlimTree<TName, TNameDistanceEvaluator>::smSPANNINGTREE);
      T->SetChooseMethod(stSlimTree<TName, TNameDistanceEvaluator>::cmMINDIST);

      for (TName *object : P)
         T->Add(object);
      T->Optimize();

      return T;
   } // end buildTree

   mySlimTree *buildTree(unordered_set<TName *> P)
   {
      int treePageSize = 4096;

      // create for Slim-Tree
      mySlimTree *T = new mySlimTree(new stMemoryPageManager(treePageSize));
      T->SetSplitMethod(stSlimTree<TName, TNameDistanceEvaluator>::smSPANNINGTREE);
      T->SetChooseMethod(stSlimTree<TName, TNameDistanceEvaluator>::cmMINDIST);

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

   OPlot *buildOPlot(vector<TName *> P, mySlimTree *T, double *R)
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

      vector<TName *> ids;
      for (TName *object : P)
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
            for (TName *object : P)
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

      for (TName *object : P)
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

   vector<MicroCluster *> spotMCs(vector<TName *> P, OPlot *O, double *R)
   {
      // BEGIN: Compute the Cutoff d
      // Build the Histogram of 1NN Distances H
      int *H = new int[a + 1];
      for (int i = 0; i < (a + 1); i++)
         H[i] = 0;
      for (TName *object : P)
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
      unordered_set<TName *> A;
      // Gel nonsingleton microclusters
      unordered_set<TName *> M_;
      double upx = -numeric_limits<double>::max();
      int e = 0;
      for (TName *object : P)
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

         vector<unordered_set<TName *>> E;
         CSJ *csj = new CSJ();
         csj->simJoin(T, T, R[e + 1], E); // Find neighbors
         for (int i = 0; i < E.size(); i++)
         { // Build neighborhood graph
            unordered_set<TName *> Gi = E[i];
            for (int j = i + 1; j < E.size(); j++)
            {
               unordered_set<TName *> Gj = E[j];
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
         for (unordered_set<TName *> Gj : E)
         {
            MicroCluster *Mj = new MicroCluster();
            for (TName *object : Gj)
               Mj->addMember(object);
            M.insert(M.end(), Mj);
         }

         delete csj;
         done(T);
      }

      // Gel singleton microclusters
      for (TName *object : A)
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

   double *scoreMCs(vector<TName *> P, vector<MicroCluster *> M, OPlot *O, double *R)
   {
      int n = P.size();
      tuple<int, int> tuple_ = distinctCharsAndLongestWord(P);
      int t_1 = get<0>(tuple_); // # distinct chars.
      int t_2 = get<1>(tuple_); // # chars. longest word
      int t = universalCodeLength(3) + universalCodeLength(t_1) + universalCodeLength(t_2);

      // BEGIN: Compute the distances to the nearest inliers
      // For outliers
      unordered_set<TName *> A; // All outliers
      for (MicroCluster *Mj : M)
      {
         for (TName *object : Mj->getMembers())
            A.insert(object);
      }
      vector<TName *> Inliers;
      for (TName *object : P)
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

            for (TName *object : A)
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
      for (TName *object : Inliers)
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
         for (TName *object : Mj->getMembers())
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
      for (TName *object : P)
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
   void loadTree(vector<TName *> P, mySlimTree *SlimTree)
   {
      for (TName *object : P)
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

   tuple<int, int> distinctCharsAndLongestWord(vector<TName *> P)
   {
      unordered_map<char, int> distinct;
      int longest = 0;

      for (TName *object : P)
      {
         string name = object->GetName();
         for (int i = 0; i < name.length(); i++)
            distinct[name[i]]++;

         if (name.size() > longest)
            longest = name.size();
      }

      tuple<int, int> tuple_ = make_tuple(distinct.size(), longest);
      return tuple_;
   } // end distinctCharsAndLongestWord

   double computeAUROC(const vector<double> &y_true, const vector<double> &y_score)
   {
      int n = y_true.size();
      vector<pair<double, double>> pairs(n);
      for (int i = 0; i < n; i++)
      {
         pairs[i] = make_pair(y_score[i], y_true[i]);
      }
      sort(pairs.begin(), pairs.end(), greater<>());
      int n_pos = std::count(y_true.begin(), y_true.end(), 1);
      int n_neg = n - n_pos;
      double tp = 0, fp = 0, last_tp = 0, last_fp = 0, auroc = 0;
      for (const auto &p : pairs)
      {
         double label = p.second;
         if (label == 1)
         {
            tp += 1;
         }
         else
         {
            fp += 1;
         }
         double tpr = tp / n_pos;
         double fpr = fp / n_neg;
         auroc += (tpr + last_tp) * (fpr - last_fp) / 2;
         last_tp = tpr;
         last_fp = fpr;
      }
      return auroc;
   } // end computeAUROC
};   // end TApp

#endif // end appH
