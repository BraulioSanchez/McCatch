//---------------------------------------------------------------------------
// stLevelDiskAccess.h ...
//
// Author: Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
// Copyright (c) 2005 GBDI-ICMC-USP
//---------------------------------------------------------------------------
#include <arboretum/stLevelDiskAccess.h>

//---------------------------------------------------------------------------
// class stLevelDiskAccess
//---------------------------------------------------------------------------
stLevelDiskAccess::stLevelDiskAccess(unsigned int height){
   Height = height;
   Radius = new double [Height];
   Nodes = new unsigned int [Height];
   Entries = new unsigned int [Height];
   for (unsigned int i = 0; i < Height; i++){
      Radius[i] = 0;
      Nodes[i] = 0;
      Entries[i] = 0;
   }//end if
}//end stLevelDiskAccess

//---------------------------------------------------------------------------
stLevelDiskAccess::~stLevelDiskAccess(){
   // Delete the resources.
   delete[] Radius;
   delete[] Nodes;
   delete[] Entries;
}//end ~stLevelDiskAccess

//---------------------------------------------------------------------------
void stLevelDiskAccess::AddEntry(double radius, unsigned int height){
   if (height < Height){
      Radius[height] += radius;
      Entries[height]++;
   }//end if
}//end AddEntry

//---------------------------------------------------------------------------
double stLevelDiskAccess::GetAvgRadius(unsigned int idx){
   if (idx < Height){
      return Radius[idx];
   }//end if
   return 0;
}//end GetAvgRadius

//---------------------------------------------------------------------------
long stLevelDiskAccess::GetNumberOfNodes(unsigned int idx){
   if (idx < Height){
      return Nodes[idx];
   }//end if
   return 0;
}//end GetNumberOfNodes

//---------------------------------------------------------------------------
void stLevelDiskAccess::AddNumberOfNodes(unsigned int height){
   if (height < Height){
      Nodes[height]++;
   }//end if
}//end GetNumberOfNodes

//---------------------------------------------------------------------------
void stLevelDiskAccess::Sumarize(){
   unsigned int i;
   
   for (i = 0; i < Height; i++){
      if (Entries[i] != 0){
         Radius[i] = Radius[i] / Entries[i];
      }else{
         Radius[i] = 0; 
      }//end if
   }//end if
}//end Sumarize
