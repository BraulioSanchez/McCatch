/**********************************************************************
* GBDI Arboretum - Copyright (c) 2002-2004 GBDI-ICMC-USP
*
*                           Homepage: http://gbdi.icmc.usp.br/arboretum
**********************************************************************/
/* ====================================================================
 * The GBDI-ICMC-USP Software License Version 1.0
 *
 * Copyright (c) 2004 Grupo de Bases de Dados e Imagens, Instituto de
 * Ciï¿½cias Matemï¿½icas e de Computaï¿½o, University of Sï¿½ Paulo -
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
 *        de Dados e Imagens, Instituto de Ciï¿½cias Matemï¿½icas e de
 *        Computaï¿½o, University of Sï¿½ Paulo - Brazil (the Databases 
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
* This file implements the classes stResult and stResults.
*
* @version 1.0
* $Revision: 1.1 $
* $Date: 2009-02-27 23:35:00 $
* $Author: seraphim $
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
// Copyright (c) 2002/2003 GBDI-ICMC-USP

//----------------------------------------------------------------------------
// Class template stResult
//----------------------------------------------------------------------------
template < class ObjectType >
stResult<ObjectType>::~stResult(){
   unsigned int i;

   for (i = 0; i < Pairs.size(); i++){
      if (Pairs[i] != NULL){
         delete Pairs[i];
      }//end if
   }//end for

   // Dispose sample object.
   if (Sample != NULL){
      delete Sample;
   }//end if
}//end stResult<ObjectType>::~stResult

//----------------------------------------------------------------------------
template < class ObjectType >
void stResult<ObjectType>::AddPair(tObject * obj, double distance){
    unsigned int pos;
    pos = this->Find(distance);
    while((pos < Pairs.size()) && (distance == Pairs[pos]->GetDistance()) &&
            (obj->GetOID() > ((tObject *)(Pairs[pos]->GetObject()))->GetOID())) {
        pos++;
    }//end while
    Pairs.insert(Pairs.begin() + pos, new tPair(obj, distance));
}//end AddPair

//----------------------------------------------------------------------------
template < class ObjectType >
void stResult<ObjectType>::Cut(unsigned int limit){
   double max;
   bool stop;
   int i;

   // Will I do something ?
   if (GetNumOfEntries() > limit){
      if (this->Tie){ // if wants tie list
         // What is the max distance ?
         max = (* this)[limit - 1].GetDistance();

         // I'll cut out everybody which has distance greater than max.
         i = GetNumOfEntries() - 1;
         stop = i < (int)limit;
         while (!stop){
            if ((* this)[i].GetDistance() > max){
               // Cut!
               RemoveLast();
               // The next to check is...
               i--;
               stop = (i < (int)limit);
            }else{
               // Oops! I found someone who will not go out.
               stop = true;
            }//end if
         }//end while
      }else{
         RemoveLast();
      }//end if
   }//end if
}//end stResult<ObjectType>::Cut

//----------------------------------------------------------------------------
template < class ObjectType >
void stResult<ObjectType>::CutFirst(unsigned int limit){
   double min;
   bool stop;
   int idx;

   // Will I do something ?
   if (GetNumOfEntries() > limit){
      if (this->Tie){ // if wants tie list
         idx = GetNumOfEntries() - limit;
         // What is the min distance?
         min = (* this)[idx].GetDistance();
         // I'll cut out everybody which has distance lesser than min.
         stop = ((GetNumOfEntries() < limit) || (idx < 0));
         while (!stop){
            if ((* this)[idx].GetDistance() < min){
               // Cut!
               RemoveFirst();
            }//end if
            // The next to check is...
            idx--;
            stop = ((GetNumOfEntries() < limit) || (idx < 0));
         }//end while
      }else{
         RemoveFirst();
      }//end if
   }//end if
}//end stResult<ObjectType>::CutFirst

//----------------------------------------------------------------------------
template < class ObjectType >
unsigned int stResult<ObjectType>::Find(double distance){
   bool stop;
   unsigned int idx;

   idx = 0;
   stop = (idx >= Pairs.size());
   while (!stop){
      if (Pairs[idx]->GetDistance() < distance){
         idx++;
         stop = (idx >= Pairs.size());
      }else{
         stop = true;
      }//end if
   }//end while

   return idx;
}//end stResult<ObjectType>::Find

//----------------------------------------------------------------------------
template < class ObjectType >
bool stResult<ObjectType>::IsEqual(const stResult * r1){
   ObjectType * tmp;
   bool result, result2;
   unsigned int i, j;
   u_int32_t numObj1, numObj2;

   numObj1 = this->GetNumOfEntries();
   numObj2 = r1->GetNumOfEntries();

   // the default answer.
   result = false;
   // test if two results have the same number of entries and maximum
   // distance.
   if ((this->GetMaximumDistance() == r1->GetMaximumDistance()) &&
       (numObj1 == numObj2)){
      // if there are, test one with the other.
      result = true;
      i = 0;
      // for each object in this class.
      while ((i < numObj1) && result) {
         // set the variables.
         result2 = false;
         // test starting with the first object. 
         j = 0;
         // for each object in the r1 set check until find a equal object.
         while ((j < numObj2) && (!result2)) {
            // check the distance first between the two objects.
            if (Pairs[i]->GetDistance() == r1->Pairs[j]->GetDistance()) {
               // the distance is equal, now test the object.
               tmp = r1->Pairs[j]->GetObject()->Clone();
               // set the result2 with the equality of the two objects.
               result2 = Pairs[i]->GetObject()->IsEqual(tmp);
               // delete the object's copy.
               delete tmp;
            }//end if
            // increment the couter of the second set.
            j++;
         }//end while
         // if the object in the first set was not in the second set, then
         // result will be false, otherwise true.
         result = result && result2;
         // increment the couter of the first set.
         i++;
      }//end while
   }//end if
   // return the result.
   return result;
}//end stResult<ObjectType>::IsEqual

//----------------------------------------------------------------------------
template < class ObjectType >
void stResult<ObjectType>::Intersection(stResult * result1, stResult * result2){
   bool result = false;
   ObjectType * tmpObj1, * tmpObj2;
   unsigned int idx, i;
   u_int32_t numObj1, numObj2;
   double distance;

   numObj1 = result1->GetNumOfEntries();
   numObj2 = result2->GetNumOfEntries();

   if ((numObj1 != 0) && (numObj2 != 0)){
      for (idx = 0; idx < numObj1; idx++){

         // get the object from result1.
         tmpObj1 = (ObjectType *)result1->Pairs[idx]->GetObject();
         distance = result1->Pairs[idx]->GetDistance();
         i = 0;
         // check if the object from result1 is in result2.
         do{
            tmpObj2 = (ObjectType *)result2->Pairs[i]->GetObject();
            // is it equal to object1?
            result = tmpObj2->IsEqual(tmpObj1);
            // store if the two objects are equal.
            if (result){
              this->AddPair(tmpObj1->Clone(), distance);
            }//end if
            i++;
         }while ((i < numObj2) && (!result));
      }//end for
   }//end if

}//end stResult<ObjectType>::Intersection

//----------------------------------------------------------------------------
template < class ObjectType >
void stResult<ObjectType>::Union(stResult * result1, stResult * result2){
   bool result = false;
   ObjectType * tmpObj1, * tmpObj2;
   unsigned int idx, i;
   u_int32_t numObj1, numObj2;
   double distance;

   numObj1 = result1->GetNumOfEntries();
   numObj2 = result2->GetNumOfEntries();

   if ((numObj1 != 0) && (numObj2 != 0)){
      // put all objects in result1 in unionResult.
      for (idx = 0; idx < numObj1; idx++){
         tmpObj1 = (ObjectType *)result1->Pairs[idx]->GetObject();
         distance = result1->Pairs[idx]->GetDistance();
         this->AddPair(tmpObj1->Clone(), distance);
      }//end for

      // now put all objects in result2 that are not in result1.
      for (idx = 0; idx < numObj2; idx++){
         // put all the objects in result1 and put in unionResult.
         tmpObj2 = (ObjectType *)result2->Pairs[idx]->GetObject();
         // it is storage the distance from result2's representative.
         distance = result2->Pairs[idx]->GetDistance();
         // check if the tmpObj2 in result2 is in result1.
         i = 0;
         do{
            tmpObj1 = (ObjectType *)result1->Pairs[i]->GetObject();
            distance = result1->Pairs[i]->GetDistance();
            // is it equal to object1?
            result = tmpObj1->IsEqual(tmpObj2);
            i++;
         }while ((i < numObj1) && (!result));
         // if the object2 is not in unionResult put it in unionResult.
         if (!result){
           this->AddPair(tmpObj2->Clone(), distance);
         }//end if
      }//end for
   }else if (numObj1 != 0){
      for (idx = 0; idx < numObj1; idx++){
         tmpObj1 = (ObjectType *)result1->Pairs[idx]->GetObject();
         distance = result1->Pairs[idx]->GetDistance();
         this->AddPair(tmpObj1->Clone(), distance);
      }//end for
   }else{
      for (idx = 0; idx < numObj2; idx++){
         tmpObj2 = (ObjectType *)result2->Pairs[idx]->GetObject();
         distance = result2->Pairs[idx]->GetDistance();
         this->AddPair(tmpObj2->Clone(), distance);
      }//end for
   }//end if
}//end stResult<ObjectType>::Union

//----------------------------------------------------------------------------
template < class ObjectType >
double stResult<ObjectType>::Precision(const stResult * r1){
   double result = -1;
   ObjectType * tmpObj1, * tmpObj2;
   int idx, i;
   u_int32_t numObj1, numObj2;
   int equal_count = 0;

   numObj1 = this->GetNumOfEntries();
   numObj2 = r1->GetNumOfEntries();

   // test if two results have the same number of entries
   if ((numObj1 != 0) && (numObj2 != 0)){
      for (idx = 0; idx < numObj2; idx++){
         // get the object from r1.
         tmpObj2 = (ObjectType *)r1->Pairs[idx]->GetObject();
         i = 0;
         // check if the object from r1 is in "this".
         do{
            tmpObj1 = (ObjectType *)this->Pairs[i]->GetObject();
            // is it equal to object1?
            result = tmpObj1->IsEqual(tmpObj2);
            // if the two objects are equal, increment the "equal_count".
            if (result){
               equal_count++;;
            }//end if
            i++;
         }while (i < numObj1); //end do while
      }//end for
   }//end if
   result = ((double)equal_count / numObj2);
   // return the result.
   return result;
}//end stResult<ObjectType>::Precision

//----------------------------------------------------------------------------
// Class template stResultPaged
//----------------------------------------------------------------------------
template < class ObjectType >
stResultPaged<ObjectType>::~stResultPaged(){
   unsigned int i;

   for (i = 0; i < Pairs.size(); i++){
      if (Pairs[i] != NULL){
         delete Pairs[i];
      }//end if
   }//end for

   // Dispose sample object.
   if (Sample != NULL){
      delete Sample;
   }//end if
}//end stResultPaged<ObjectType>::~stResultPaged

//----------------------------------------------------------------------------
template < class ObjectType >
void stResultPaged<ObjectType>::AddPair(tObject * obj, double distance){
    unsigned int pos;
    pos = this->Find(distance);
    while((pos < Pairs.size()) && (distance == Pairs[pos]->GetDistance()) &&
            (obj->GetOID() > ((tObject *)(Pairs[pos]->GetObject()))->GetOID())) {
        pos++;
    }//end while
    Pairs.insert(Pairs.begin() + pos, new tPair(obj, distance));
}//end AddPair

//----------------------------------------------------------------------------
template < class ObjectType >
void stResultPaged<ObjectType>::Cut(unsigned int limit){
   double max;
   bool stop;
   int i;

   // Will I do something ?
   if (GetNumOfEntries() > limit){
      if (this->Tie){ // if wants tie list
         // What is the max distance ?
         max = (* this)[limit - 1].GetDistance();

         // I'll cut out everybody which has distance greater than max.
         i = GetNumOfEntries() - 1;
         stop = i < (int)limit;
         while (!stop){
            if ((* this)[i].GetDistance() > max){
               // Cut!
               RemoveLast();
               // The next to check is...
               i--;
               stop = (i < (int)limit);
            }else{
               // Oops! I found someone who will not go out.
               stop = true;
            }//end if
         }//end while
      }else{
         RemoveLast();
      }//end if
   }//end if
}//end stResultPaged<ObjectType>::Cut

//----------------------------------------------------------------------------
template < class ObjectType >
void stResultPaged<ObjectType>::CutFirst(unsigned int limit){
   double min;
   bool stop;
   int idx;

   // Will I do something ?
   if (GetNumOfEntries() > limit){
      if (this->Tie){ // if wants tie list
         idx = GetNumOfEntries() - limit;
         // What is the min distance?
         min = (* this)[idx].GetDistance();
         // I'll cut out everybody which has distance lesser than min.
         stop = ((GetNumOfEntries() < limit) || (idx < 0));
         while (!stop){
            if ((* this)[idx].GetDistance() < min){
               // Cut!
               RemoveFirst();
            }//end if
            // The next to check is...
            idx--;
            stop = ((GetNumOfEntries() < limit) || (idx < 0));
         }//end while
      }else{
         RemoveFirst();
      }//end if
   }//end if
}//end stResultPaged<ObjectType>::CutFirst

//----------------------------------------------------------------------------
template < class ObjectType >
unsigned int stResultPaged<ObjectType>::Find(double distance){
   bool stop;
   unsigned int idx;

   idx = 0;
   stop = (idx >= Pairs.size());
   while (!stop){
      if (Pairs[idx]->GetDistance() < distance){
         idx++;
         stop = (idx >= Pairs.size());
      }else{
         stop = true;
      }//end if
   }//end while

   return idx;
}//end stResultPaged<ObjectType>::Find

//----------------------------------------------------------------------------
template < class ObjectType >
bool stResultPaged<ObjectType>::IsEqual(const stResultPaged * r1){
   ObjectType * tmp;
   bool result, result2;
   unsigned int i, j;
   u_int32_t numObj1, numObj2;

   numObj1 = this->GetNumOfEntries();
   numObj2 = r1->GetNumOfEntries();

   // the default answer.
   result = false;
   // test if two results have the same number of entries and maximum
   // distance.
   if ((this->GetMaximumDistance() == r1->GetMaximumDistance()) &&
       (numObj1 == numObj2)){
      // if there are, test one with the other.
      result = true;
      i = 0;
      // for each object in this class.
      while ((i < numObj1) && result) {
         // set the variables.
         result2 = false;
         // test starting with the first object. 
         j = 0;
         // for each object in the r1 set check until find a equal object.
         while ((j < numObj2) && (!result2)) {
            // check the distance first between the two objects.
            if (Pairs[i]->GetDistance() == r1->Pairs[j]->GetDistance()) {
               // the distance is equal, now test the object.
               tmp = r1->Pairs[j]->GetObject()->Clone();
               // set the result2 with the equality of the two objects.
               result2 = Pairs[i]->GetObject()->IsEqual(tmp);
               // delete the object's copy.
               delete tmp;
            }//end if
            // increment the couter of the second set.
            j++;
         }//end while
         // if the object in the first set was not in the second set, then
         // result will be false, otherwise true.
         result = result && result2;
         // increment the couter of the first set.
         i++;
      }//end while
   }//end if
   // return the result.
   return result;
}//end stResultPaged<ObjectType>::IsEqual

//----------------------------------------------------------------------------
template < class ObjectType >
void stResultPaged<ObjectType>::Intersection(stResultPaged * result1, stResultPaged * result2){
   bool result = false;
   ObjectType * tmpObj1, * tmpObj2;
   unsigned int idx, i;
   u_int32_t numObj1, numObj2;
   double distance;

   numObj1 = result1->GetNumOfEntries();
   numObj2 = result2->GetNumOfEntries();

   if ((numObj1 != 0) && (numObj2 != 0)){
      for (idx = 0; idx < numObj1; idx++){

         // get the object from result1.
         tmpObj1 = (ObjectType *)result1->Pairs[idx]->GetObject();
         distance = result1->Pairs[idx]->GetDistance();
         i = 0;
         // check if the object from result1 is in result2.
         do{
            tmpObj2 = (ObjectType *)result2->Pairs[i]->GetObject();
            // is it equal to object1?
            result = tmpObj2->IsEqual(tmpObj1);
            // store if the two objects are equal.
            if (result){
              this->AddPair(tmpObj1->Clone(), distance);
            }//end if
            i++;
         }while ((i < numObj2) && (!result));
      }//end for
   }//end if

}//end stResultPaged<ObjectType>::Intersection

//----------------------------------------------------------------------------
template < class ObjectType >
void stResultPaged<ObjectType>::Union(stResultPaged * result1, stResultPaged * result2){
   bool result = false;
   ObjectType * tmpObj1, * tmpObj2;
   unsigned int idx, i;
   u_int32_t numObj1, numObj2;
   double distance;

   numObj1 = result1->GetNumOfEntries();
   numObj2 = result2->GetNumOfEntries();

   if ((numObj1 != 0) && (numObj2 != 0)){
      // put all objects in result1 in unionResult.
      for (idx = 0; idx < numObj1; idx++){
         tmpObj1 = (ObjectType *)result1->Pairs[idx]->GetObject();
         distance = result1->Pairs[idx]->GetDistance();
         this->AddPair(tmpObj1->Clone(), distance);
      }//end for

      // now put all objects in result2 that are not in result1.
      for (idx = 0; idx < numObj2; idx++){
         // put all the objects in result1 and put in unionResult.
         tmpObj2 = (ObjectType *)result2->Pairs[idx]->GetObject();
         // it is storage the distance from result2's representative.
         distance = result2->Pairs[idx]->GetDistance();
         // check if the tmpObj2 in result2 is in result1.
         i = 0;
         do{
            tmpObj1 = (ObjectType *)result1->Pairs[i]->GetObject();
            distance = result1->Pairs[i]->GetDistance();
            // is it equal to object1?
            result = tmpObj1->IsEqual(tmpObj2);
            i++;
         }while ((i < numObj1) && (!result));
         // if the object2 is not in unionResult put it in unionResult.
         if (!result){
           this->AddPair(tmpObj2->Clone(), distance);
         }//end if
      }//end for
   }else if (numObj1 != 0){
      for (idx = 0; idx < numObj1; idx++){
         tmpObj1 = (ObjectType *)result1->Pairs[idx]->GetObject();
         distance = result1->Pairs[idx]->GetDistance();
         this->AddPair(tmpObj1->Clone(), distance);
      }//end for
   }else{
      for (idx = 0; idx < numObj2; idx++){
         tmpObj2 = (ObjectType *)result2->Pairs[idx]->GetObject();
         distance = result2->Pairs[idx]->GetDistance();
         this->AddPair(tmpObj2->Clone(), distance);
      }//end for
   }//end if
}//end stResultPaged<ObjectType>::Union

//----------------------------------------------------------------------------
template < class ObjectType >
double stResultPaged<ObjectType>::Precision(const stResultPaged * r1){
   double result = -1;
   ObjectType * tmpObj1, * tmpObj2;
   int idx, i;
   u_int32_t numObj1, numObj2;
   int equal_count = 0;

   numObj1 = this->GetNumOfEntries();
   numObj2 = r1->GetNumOfEntries();

   // test if two results have the same number of entries
   if ((numObj1 != 0) && (numObj2 != 0)){
      for (idx = 0; idx < numObj2; idx++){
         // get the object from r1.
         tmpObj2 = (ObjectType *)r1->Pairs[idx]->GetObject();
         i = 0;
         // check if the object from r1 is in "this".
         do{
            tmpObj1 = (ObjectType *)this->Pairs[i]->GetObject();
            // is it equal to object1?
            result = tmpObj1->IsEqual(tmpObj2);
            // if the two objects are equal, increment the "equal_count".
            if (result){
               equal_count++;;
            }//end if
            i++;
         }while (i < numObj1); //end do while
      }//end for
   }//end if
   result = ((double)equal_count / numObj2);
   // return the result.
   return result;
}//end stResultPaged<ObjectType>::Precision

//----------------------------------------------------------------------------
// Class template stJoinedResult
//----------------------------------------------------------------------------
template < class ObjectType >
stJoinedResult<ObjectType>::~stJoinedResult(){
   unsigned int i;

   for (i = 0; i < Triples.size(); i++){
      if (Triples[i] != NULL){
         delete Triples[i];
      }//end if
   }//end for
}//end stJoinedResult<ObjectType>::~stJoinedResult

//----------------------------------------------------------------------------
template < class ObjectType >
void stJoinedResult<ObjectType>::AddLocalResult(stResult<ObjectType> * local){
   for (unsigned i=0; i < local->GetNumOfEntries(); i++){
      AddJoinedTriple(local->GetSample()->Clone(),
        ((ObjectType *)(* local)[i].GetObject())->Clone(),(* local)[i].GetDistance());
   }//end for
}//end stJoinedResult<ObjectType>::AddLocalResult

//----------------------------------------------------------------------------
template < class ObjectType >
void stJoinedResult<ObjectType>::AddLocalResultMaxDist(stResult<ObjectType> * local){
   for (unsigned i=0; (i < local->GetNumOfEntries()) &&
          ((* local)[i].GetDistance() <= GetMaximumDistance()); i++){
      AddJoinedTriple(local->GetSample()->Clone(),
         ((ObjectType *)(* local)[i].GetObject())->Clone(),(* local)[i].GetDistance());
   }//end for
}//end stJoinedResult<ObjectType>::AddLocalResultMaxDist

//----------------------------------------------------------------------------
template < class ObjectType >
void stJoinedResult<ObjectType>::AddtJoinedResult(stJoinedResult<ObjectType> * res){
   for (unsigned i=0; i < res->GetNumOfEntries(); i++){
      AddJoinedTriple( ((ObjectType *)(* res)[i].GetJoinedObject())->Clone(),
         ((ObjectType *)(* res)[i].GetObject())->Clone(),(* res)[i].GetDistance());
   }//end for
}//end stJoinedResult<ObjectType>::AddtJoinedResult

//----------------------------------------------------------------------------
template < class ObjectType >
void stJoinedResult<ObjectType>::Cut(unsigned int limit){
   double max;
   bool stop;
   int i;

   // Will I do something ?
   if (GetNumOfEntries() > limit){
      if (this->Tie){ // if wants tie list
         // What is the max distance ?
         max = (* this)[limit - 1].GetDistance();

         // I'll cut out everybody which has distance greater than max.
         i = GetNumOfEntries() - 1;
         stop = i < (int)limit;
         while (!stop){
            if ((* this)[i].GetDistance() > max){
               // Cut!
               RemoveLast();
               // The next to check is...
               i--;
               stop = (i < (int)limit);
            }else{
               // Oops! I found someone who will not go out.
               stop = true;
            }//end if
         }//end while
      }else{
         RemoveLast();
      }//end if
   }//end if
}//end stJoinedResult<ObjectType>::Cut

//----------------------------------------------------------------------------
template < class ObjectType >
void stJoinedResult<ObjectType>::CutFirst(unsigned int limit){
   double min;
   bool stop;
   int idx;

   // Will I do something ?
   if (GetNumOfEntries() > limit){
      if (this->Tie){ // if wants tie list
         idx = GetNumOfEntries() - limit;
         // What is the min distance?
         min = (* this)[idx].GetDistance();
         // I'll cut out everybody which has distance lesser than min.
         stop = ((GetNumOfEntries() < limit) || (idx < 0));
         while (!stop){
            if ((* this)[idx].GetDistance() < min){
               // Cut!
               RemoveFirst();
            }//end if
            // The next to check is...
            idx--;
            stop = ((GetNumOfEntries() < limit) || (idx < 0));
         }//end while
      }else{
         RemoveFirst();
      }//end if
   }//end if
}//end stJoinedResult<ObjectType>::CutFirst

//----------------------------------------------------------------------------
template < class ObjectType >
unsigned int stJoinedResult<ObjectType>::Find(double distance){
   bool stop;
   unsigned int idx;

   idx = 0;
   stop = (idx >= Triples.size());
   while (!stop){
      if (Triples[idx]->GetDistance() < distance){
         idx++;
         stop = (idx >= Triples.size());
      }else{
         stop = true;
      }//end if
   }//end while

   return idx;
}//end stJoinedResult<ObjectType>::Find

//----------------------------------------------------------------------------
template < class ObjectType >
bool stJoinedResult<ObjectType>::IsEqual(const stJoinedResult * r1){
   ObjectType * tmp, * joinedTmp;
   bool result, result2;
   unsigned int i, j;
   u_int32_t numObj1, numObj2;

   numObj1 = this->GetNumOfEntries();
   numObj2 = r1->GetNumOfEntries();

   // the default answer.
   result = false;
   // test if two results have the same number of entries and maximum
   // distance.
   if ((this->GetMaximumDistance() == r1->GetMaximumDistance()) &&
       (numObj1 == numObj2)){
      // if there are, test one with the other.
      result = true;
      i = 0;
      // for each object in this class.
      while ((i < numObj1) && result) {
         // set the variables.
         result2 = false;
         // test starting with the first object. 
         j = 0;
         // for each object in the r1 set check until find a equal object.
         while ((j < numObj2) && (!result2)) {
            // check the distance first between the two objects.
            if (Triples[i]->GetDistance() == r1->Triples[j]->GetDistance()) {
               // the distance is equal, now test the object.
               tmp = r1->Triples[j]->GetObject()->Clone();
               joinedTmp = r1->Triples[j]->GetJoinedObject()->Clone();
               // set the result2 with the equality of the two objects.
               result2 = (Triples[i]->GetObject()->IsEqual(tmp) &&
                         Triples[i]->GetJoinedObject()->IsEqual(joinedTmp)) ||
                         (Triples[i]->GetObject()->IsEqual(joinedTmp) &&
                         Triples[i]->GetJoinedObject()->IsEqual(tmp));
               // delete the object's copy.
               delete tmp;
               delete joinedTmp;
            }//end if
            // increment the couter of the second set.
            j++;
         }//end while
         // if the object in the first set was not in the second set, then
         // result will be false, otherwise true.
         result = result && result2;
         // increment the couter of the first set.
         i++;
      }//end while
   }//end if
   // return the result.
   return result;
}//end stJoinedResult<ObjectType>::IsEqual

//----------------------------------------------------------------------------
template < class ObjectType >
void stJoinedResult<ObjectType>::Intersection(stJoinedResult * result1,
                                              stJoinedResult * result2){
   bool result = false;
   ObjectType * tmpObj1, * tmpObj2;
   unsigned int idx, i;
   u_int32_t numObj1, numObj2;
   double distance;

   numObj1 = result1->GetNumOfEntries();
   numObj2 = result2->GetNumOfEntries();

   if ((numObj1 != 0) && (numObj2 != 0)){
      for (idx = 0; idx < numObj1; idx++){

         // get the object from result1.
         tmpObj1 = (ObjectType *)result1->Triples[idx]->GetObject();
         distance = result1->Triples[idx]->GetDistance();
         i = 0;
         // check if the object from result1 is in result2.
         do{
            tmpObj2 = (ObjectType *)result2->Triples[i]->GetObject();
            // is it equal to object1?
            result = tmpObj2->IsEqual(tmpObj1);
            // store if the two objects are equal.
            if (result){
              this->AddPair(tmpObj1->Clone(), distance);
            }//end if
            i++;
         }while ((i < numObj2) && (!result));
      }//end for
   }//end if

}//end stJoinedResult<ObjectType>::Intersection

//----------------------------------------------------------------------------
template < class ObjectType >
void stJoinedResult<ObjectType>::Union(stJoinedResult * result1,
                                       stJoinedResult * result2){
   bool result = false;
   ObjectType * tmpObj1, * tmpObj2;
   unsigned int idx, i;
   u_int32_t numObj1, numObj2;
   double distance;

   numObj1 = result1->GetNumOfEntries();
   numObj2 = result2->GetNumOfEntries();

   if ((numObj1 != 0) && (numObj2 != 0)){
      // put all objects in result1 in unionResult.
      for (idx = 0; idx < numObj1; idx++){
         tmpObj1 = (ObjectType *)result1->Triples[idx]->GetObject();
         distance = result1->Triples[idx]->GetDistance();
         this->AddPair(tmpObj1->Clone(), distance);
      }//end for

      // now put all objects in result2 that are not in result1.
      for (idx = 0; idx < numObj2; idx++){
         // put all the objects in result1 and put in unionResult.
         tmpObj2 = (ObjectType *)result2->Triples[idx]->GetObject();
         // it is storage the distance from result2's representative.
         distance = result2->Triples[idx]->GetDistance();
         // check if the tmpObj2 in result2 is in result1.
         i = 0;
         do{
            tmpObj1 = (ObjectType *)result1->Triples[i]->GetObject();
            distance = result1->Triples[i]->GetDistance();
            // is it equal to object1?
            result = tmpObj1->IsEqual(tmpObj2);
            i++;
         }while ((i < numObj1) && (!result));
         // if the object2 is not in unionResult put it in unionResult.
         if (!result){
           this->AddPair(tmpObj2->Clone(), distance);
         }//end if
      }//end for
   }else if (numObj1 != 0){
      for (idx = 0; idx < numObj1; idx++){
         tmpObj1 = (ObjectType *)result1->Triples[idx]->GetObject();
         distance = result1->Triples[idx]->GetDistance();
         this->AddPair(tmpObj1->Clone(), distance);
      }//end for
   }else{
      for (idx = 0; idx < numObj2; idx++){
         tmpObj2 = (ObjectType *)result2->Triples[idx]->GetObject();
         distance = result2->Triples[idx]->GetDistance();
         this->AddPair(tmpObj2->Clone(), distance);
      }//end for
   }//end if
}//end stJoinedResult<ObjectType>::Union

//----------------------------------------------------------------------------
template < class ObjectType >
double stJoinedResult<ObjectType>::Precision(const stJoinedResult * r1){
   double result = -1;
   ObjectType * tmpObj1, * tmpObj2;
   int idx, i;
   u_int32_t numObj1, numObj2;
   int equal_count = 0;

   numObj1 = this->GetNumOfEntries();
   numObj2 = r1->GetNumOfEntries();

   // test if two results have the same number of entries
   if ((numObj1 != 0) && (numObj2 != 0)){
      for (idx = 0; idx < numObj2; idx++){
         // get the object from r1.
         tmpObj2 = (ObjectType *)r1->Triples[idx]->GetObject();
         i = 0;
         // check if the object from r1 is in "this".
         do{
            tmpObj1 = (ObjectType *)this->Triples[i]->GetObject();
            // is it equal to object1?
            result = tmpObj1->IsEqual(tmpObj2);
            // if the two objects are equal, increment the "equal_count".
            if (result){
               equal_count++;;
            }//end if
            i++;
         }while (i < numObj1); //end do while
      }//end for
   }//end if
   result = ((double)equal_count / numObj2);
   // return the result.
   return result;
}//end stJoinedResult<ObjectType>::Precision
