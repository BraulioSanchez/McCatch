//---------------------------------------------------------------------------
// stHistogram.h ...
//
// Author: Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
// Copyright (c) 2005 GBDI-ICMC-USP
//---------------------------------------------------------------------------
#ifndef stHistogramH
#define stHistogramH

#include <vector>

//---------------------------------------------------------------------------
// class stHistogram
//---------------------------------------------------------------------------
template <class ObjectType, class EvaluatorType>
class stHistogram {

   public:

      /**
      * Type of the object.
      */
      typedef ObjectType tObject;

      /**
      * Creates a new instance of this class.
      */
      stHistogram(unsigned int numberOfBins = 100);

      /**
      * Destruct of this class.
      */
      ~stHistogram();

      int Size(){
         return NumberOfBins;
      }//end Size

      int GetNumberOfObjects();

      void Build(EvaluatorType * metricEvaluator);

      void Add(ObjectType * obj);

      double GetBin(unsigned int idx);

      double GetValue(unsigned int idx);

      double GetValueOfBin(double value);

   private:

      std::vector <tObject *> Objects;

      double * Values;

      double * Bins;

      unsigned int NumberOfBins;

};//end stHistogram

#include "stHistogram-inl.h"

#endif //end stHistogramH
