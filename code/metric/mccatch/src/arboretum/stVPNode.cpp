/**
* @file
*
* This file implements the VPTree node.
*
* @version 1.0
* $Date: 2005-03-21 14:43:17 $
* @author Ives Renê Venturini Pola (ives@icmc.usp.br)
*/
// Copyright (c) 2004 GBDI-ICMC-USP
#include <arboretum/stVPNode.h>

//-----------------------------------------------------------------------------
// class stVPNode
//-----------------------------------------------------------------------------
stVPNode::stVPNode(stPage * page, bool create){

   this->Page = page;

   // if create is true, we must to zero fill the page
   if (create){
      Page->Clear();
   }//end if

   // Set elements
   this->Header = (stVPNodeHeader *) this->Page->GetData();
   this->Entry = (u_int32_t *)(this->Page->GetData() + sizeof(stVPNodeHeader));
}//end stVPNode::stVPNode

//------------------------------------------------------------------------------
bool stVPNode::AddEntry(u_int32_t size, const unsigned char * object){
   u_int32_t totalsize;
   u_int32_t offs;

   totalsize = size + sizeof(u_int32_t);
   if (totalsize <= GetFree()){

      offs = Page->GetPageSize() - size;

      // Update entry offset
      *Entry = offs;
      // Write object
      memcpy( (void *) (Page->GetData() + *Entry), (void *) object, size);

      return true;
   }else{
      // there is no room for the object
      return false;
   }//end if
}//end stVPNode::AddEntry

//------------------------------------------------------------------------------
const unsigned char * stVPNode::GetObject(){
   return (unsigned char *) Page->GetData() + *Entry;
}//end stVPNode::GetObject

//------------------------------------------------------------------------------
u_int32_t stVPNode::GetObjectSize(){
   return (u_int32_t) Page->GetPageSize() - *Entry;
}//end stVPNode::GetObjectSize

//------------------------------------------------------------------------------
u_int32_t stVPNode::GetFree(){
   return Page->GetPageSize() - sizeof(stVPNodeHeader);
}//end stVPNode::GetFree
