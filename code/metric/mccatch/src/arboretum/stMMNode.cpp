/**
* @file
*
* This file implements the MMTree node.
*
* @version 1.0
* $Date: 2005-03-08 19:44:19 $
* @author Ives Renê Venturini Pola (ives@icmc.sc.usp.br)
*/
// Copyright (c) 2004 GBDI-ICMC-USP
#include <arboretum/stMMNode.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// class stMMNode
//-----------------------------------------------------------------------------
stMMNode::stMMNode(stPage * page, bool create){

   this->Page = page;

   // if create is true, we must to zero fill the page
   if (create){
      Page->Clear();
   }//end if

   // Set elements
   this->Header = (stMMNodeHeader *) this->Page->GetData();
   this->Entries = (u_int32_t *)(this->Page->GetData() + sizeof(stMMNodeHeader));

}//end stMMNode::stMMNode

//------------------------------------------------------------------------------
int stMMNode::AddEntry(u_int32_t size, const unsigned char * object){
   u_int32_t totalsize;
   u_int32_t offs;

   totalsize = size + sizeof(u_int32_t);
   if (totalsize <= GetFree()){

      // Object offset
      if (Header->Occupation == 0){
         offs = Page->GetPageSize() - size;
      }else{
         if (Header->Occupation == 1){
            offs = Entries[Header->Occupation-1] - size;
         }else{
            return -1; //there can be no more than two objects per node
         }//end if
      }//end if

      // Update entry
      this->Entries[Header->Occupation] = offs;

      // Write object
      memcpy((void *)(Page->GetData() + Entries[Header->Occupation]),
             (void *)object, size);

      // Update header
      this->Header->Occupation++;
      return ((int )this->Header->Occupation) - 1;
   }else{
      return -1;  // there is no room for the object
   }//end if
}//end stMMNode::AddEntry

//------------------------------------------------------------------------------
const unsigned char * stMMNode::GetObject(int id){

   return (unsigned char *) Page->GetData() + Entries[id];
}//end stMMNode::GetObject

//------------------------------------------------------------------------------
u_int32_t stMMNode::GetObjectSize(int id){

   if (id == 0){
      return Page->GetPageSize() - Entries[0];
   }else{
      return Entries[id-1] - Entries[id];
   }//end if
}//end stMMNode::GetObjectSize

//------------------------------------------------------------------------------
u_int32_t stMMNode::GetFree(){

   if (Header->Occupation == 0){
      return Page->GetPageSize() - sizeof(stMMNodeHeader);
   }else{
      return Page->GetPageSize() - sizeof(stMMNodeHeader) -
             (sizeof(u_int32_t) * Header->Occupation) -
             (Page->GetPageSize() - Entries[Header->Occupation-1]);
   }//end if
}//end stMMNode::GetFree
