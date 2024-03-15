/**********************************************************************
* GBDI Arboretum - Copyright (c) 2002-2004 GBDI-ICMC-USP
*
*                           Homepage: http://gbdi.icmc.usp.br/arboretum
**********************************************************************/
/* ====================================================================
 * The GBDI-ICMC-USP Software License Version 1.0
 *
 * Copyright (c) 2004 Grupo de Bases de Dados e Imagens, Instituto de
 * Ciências Matemáticas e de Computação, University of São Paulo -
 * Brazil (the Databases and Image Group - Intitute of Matematical and 
 * Computer Sciences).  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by Grupo de Bases
 *        de Dados e Imagens, Instituto de Ciências Matemáticas e de
 *        Computação, University of São Paulo - Brazil (the Databases 
 *        and Image Group - Intitute of Matematical and Computer 
 *        Sciences)"
 *
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names of the research group, institute, university, authors
 *    and collaborators must not be used to endorse or promote products
 *    derived from this software without prior written permission.
 *
 * 5. The names of products derived from this software may not contain
 *    the name of research group, institute or university, without prior
 *    written permission of the authors of this software.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OF THIS SOFTWARE OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * ====================================================================
 *                                            http://gbdi.icmc.usp.br/
 */
/**
* @file
*
* This file implements the stSlimMemLeafNode class.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @author Thiago Galbiatti Vespa (thiago@icmc.usp.br)
*/
// Copyright (c) 2003 GBDI-ICMC-USP

//==============================================================================
// Class stSlimMemLeafNode
//------------------------------------------------------------------------------
template <class ObjectType>
stSlimMemLeafNode< ObjectType >::stSlimMemLeafNode(stSlimLeafNode * leafNode){
   u_int32_t idx;
   u_int32_t numberOfEntries;
   ObjectType * obj;

   numberOfEntries = leafNode->GetNumberOfEntries();
   // Get the information to be ajust.
   this->numEntries = 0;
   this->capacity = numberOfEntries;
   this->maximumSize = (leafNode->GetPage())->GetPageSize();
   this->usedSize = stSlimLeafNode::GetGlobalOverhead();
   this->srcLeafNode = leafNode;
   // Allocate memory for new entries.
   this->Entries = new stSlimMemNodeEntry[numberOfEntries * sizeof(stSlimMemNodeEntry)];

   // insert the entries of leafNode.
   for (idx = 0; idx < numberOfEntries; idx++){
      obj = new ObjectType();
      // Get the first object in leafNode.
      obj->Unserialize(leafNode->GetObject(idx), leafNode->GetObjectSize(idx));
      // Add data.
      this->Add(obj, leafNode->GetLeafEntry(idx).Distance);
   }//end while

   // remove the entry from the leafNode.
   srcLeafNode->RemoveAll();

}//end stSlimMemLeafNode::stSlimMemLeafNode()

template <class ObjectType>
stSlimLeafNode * stSlimMemLeafNode< ObjectType >::ReleaseNode(){
   int idx;
   int insertIdx;
   ObjectType * obj;

   for (idx = 0; idx < this->numEntries; idx++){
      // Get a object in idx.
      obj = this->ObjectAt(idx);
      // insert this entry in srcLeafNode.
      insertIdx = srcLeafNode->AddEntry(obj->GetSerializedSize(), obj->Serialize());

      // if there is some problem in insertion.
      #ifdef __stDEBUG__
         if (insertIdx < 0){
            throw std::logic_error("The slimLeafNode does not have enough space to store objects.");
         }//end if
      #endif //__stDEBUG__
      // Fill entry's fields
      srcLeafNode->GetLeafEntry(insertIdx).Distance = this->DistanceAt(idx);
   }//end for

   // release the resources.
   for (idx = this->numEntries-1; idx >= 0; idx--){
      delete this->ObjectAt(idx);
   }//end for
   delete[] Entries;
   Entries = NULL;
   
   // update fields.
   this->numEntries = 0;
   this->usedSize = stSlimLeafNode::GetGlobalOverhead();
   // return the leafNode
   return srcLeafNode;
}//end stSlimMemLeafNode::ReleaseNode()

//------------------------------------------------------------------------------
template <class ObjectType>
bool stSlimMemLeafNode< ObjectType >::Add(ObjectType * obj, double distance){
   int insertIdx, idx;

   #ifdef __stDEBUG__
      if (obj == NULL){
         throw std::logic_error("The object is NULL.");
      }//end if
   #endif //__stDEBUG__

   // Does it fit ?
   if (!this->CanAdd(obj)){
      // No. There is no space in the node to put this object!
      return false;
   }//end if

   // if there is free entry to store the new entry.
   if (this->capacity <= this->numEntries){
      // resize the entries.
      this->Resize();
   }//end if
   
   // Look the right position to insert the new object.
   insertIdx = InsertPosition(distance);
   // Get the number of entries.
   idx = this->numEntries;
   // Lets move the data, according to insertIdx.
   while (insertIdx != idx){
      idx--;
      Entries[idx+1].Distance = Entries[idx].Distance;
      Entries[idx+1].Object = Entries[idx].Object;
   }//end while
   // add the new entry in the right position.
   Entries[idx].Distance = distance;
   Entries[idx].Object = obj;

   // Update # of Entries
   this->numEntries++; // One more!
   // Update the usedSize
   this->usedSize += obj->GetSerializedSize() + stSlimLeafNode::GetLeafEntryOverhead();

   return true;
}//end stSlimMemLeafNode::Add()

//------------------------------------------------------------------------------
template <class ObjectType>
ObjectType * stSlimMemLeafNode< ObjectType >::Remove(u_int32_t idx){
   ObjectType * returnObject;

   #ifdef __stDEBUG__
      if (idx >= this->numEntries){
         throw std::logic_error("idx value is out of range.");
      }//end if
   #endif //__stDEBUG__

   // copy the entry.
   returnObject = this->GetObject(idx);

   // Lets move the data, according to idx.
   while (idx < (this->numEntries - 1)){
      Entries[idx].Distance = Entries[idx+1].Distance;
      Entries[idx].Object = Entries[idx+1].Object;
      idx++;
   }//end while

   // Update # of Entries
   this->numEntries--; // One less!
   // Update the usedSize
   this->usedSize -= (returnObject->GetSerializedSize() +
                      stSlimLeafNode::GetLeafEntryOverhead());
   // return the removed entry.
   return returnObject;
}//end stSlimMemLeafNode::Remove

//------------------------------------------------------------------------------
template <class ObjectType>
ObjectType * stSlimMemLeafNode< ObjectType >::PopObject(){
   ObjectType * returnObject;

   #ifdef __stDEBUG__
      if (this->numEntries==0){
         throw std::logic_error("There is no object.");
      }//end if
   #endif //__stDEBUG__

   // copy the entry.
   returnObject = this->ObjectAt(this->numEntries-1);
   // Update # of Entries
   this->numEntries--; // One less!
   // Update the usedSize
   this->usedSize -= (returnObject->GetSerializedSize() +
                      stSlimLeafNode::GetLeafEntryOverhead());
   // return the removed entry.
   return returnObject;
}//end stSlimMemLeafNode::PopObject

//------------------------------------------------------------------------------
template <class ObjectType>
int stSlimMemLeafNode< ObjectType >::InsertPosition(double distance){
   int idx;

   #ifdef __stDEBUG__
      if (distance < 0){
         throw std::logic_error("The distance is less than 0.");
      }//end if
   #endif //__stDEBUG__

   // if there is no entries, return 0.
   if (this->numEntries==0){
      return 0;
   }//end if

   // Lets search the correct position, according to its distance.
   idx = this->numEntries;
   while (distance < Entries[idx-1].Distance){
      idx--;
   }//end while

   return idx;
}//end stSlimMemLeafNode::InsertPosition()

//------------------------------------------------------------------------------
template <class ObjectType>
void stSlimMemLeafNode< ObjectType >::Resize(u_int32_t incSize){
   stSlimMemNodeEntry * tmpEntries;

   // Is incSize correct?
   if (incSize > 0){
      // allocate new space.
      tmpEntries = new stSlimMemNodeEntry[(this->numEntries + incSize) * sizeof(stSlimMemNodeEntry)];
      // copy the entries.
      memcpy(tmpEntries, this->Entries, this->numEntries * sizeof(stSlimMemNodeEntry));
      // delete the old data.
      delete []this->Entries;
      // ajust the pointers.
      this->Entries = tmpEntries;
   }//end if
   this->capacity += incSize;
}//end stSlimMemLeafNode::Resize()