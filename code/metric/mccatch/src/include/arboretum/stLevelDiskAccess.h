//---------------------------------------------------------------------------
// stHistogram.h ...
//
// Author: Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
// Copyright (c) 2005 GBDI-ICMC-USP
//---------------------------------------------------------------------------
#ifndef stLevelDiskAccessH
#define stLevelDiskAccessH

//---------------------------------------------------------------------------
// class stHistogram
//---------------------------------------------------------------------------
class stLevelDiskAccess {

   public:

      /**
      * Creates a new instance of this class.
      */
      stLevelDiskAccess(unsigned int height);

      /**
      * Destruct of this class.
      */
      ~stLevelDiskAccess();

      int GetHeight(){
         return Height;
      }//end GetHeight

      void AddEntry(double radius, unsigned int height);

      double GetAvgRadius(unsigned int idx);

      long GetNumberOfNodes(unsigned int idx);

      void AddNumberOfNodes(unsigned int height);

      void Sumarize();

   private:

      double * Radius;

      unsigned int * Nodes;

      unsigned int * Entries;

      unsigned int Height;

};//end stLevelDiskAccess

#endif //end stLevelDiskAccessH
