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
* This file defines the classes stInputQuery and stInputQueryPair.
*
* @version 1.0
* $Revision: 1.5 $
* $Date: 2005-03-08 19:43:09 $
* $Author: marcos $
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
// Copyright (c) 2002/2003 GBDI-ICMC-USP

//----------------------------------------------------------------------------
// Class template stResult
//----------------------------------------------------------------------------
template < class ObjectType, class EvaluatorType>
stInputQuery<ObjectType, EvaluatorType>::~stInputQuery(){
   unsigned int idx;

   for (idx = 0; idx < Pairs.size(); idx++){
      if (Pairs[idx] != NULL){
         delete Pairs[idx];
      }//end if
   }//end for
   
}//end stInputQuery<ObjectType>::~stInputQuery()

//------------------------------------------------------------------------------
template < class ObjectType, class EvaluatorType>
double stInputQuery<ObjectType, EvaluatorType>::GetMinimumRadius(){
   double distance;
   u_int32_t idx;
   u_int32_t numOfEntries = GetNumOfEntries();
   double maxDistance = 0;

   for (idx = 0; idx < numOfEntries; idx++){
      distance = Pairs[idx]->GetDistance() + Pairs[idx]->GetRadius();
      if (maxDistance < distance){
         maxDistance = distance;
      }//end if
   }//end for

   // return the minimum radius.
   return maxDistance;
}//end stInputQuery<ObjectType>::GetMinimumRadius

//------------------------------------------------------------------------------
template <class ObjectType, class EvaluatorType>
u_int32_t stInputQuery<ObjectType, EvaluatorType>::ChooseRepresentative(){

   u_int32_t idx, idxRep;
   double newRadius = MAXDOUBLE;
   ObjectType * repObj;
   u_int32_t numberOfEntries = GetNumOfEntries();

   //test what representative in the Pairs is the best to the query.
   for (idx=0; idx < numberOfEntries; idx++){
      // Get the representative object
      repObj = Pairs[idx]->GetObject();
      UpdateDistances(repObj, idx);
      // if the radius is small that the previus.
      if (GetMinimumRadius() < newRadius){
         newRadius = GetMinimumRadius();
         idxRep = idx;
      }//end if
   }//end if

   // Update all distances with the real representative
   repObj = Pairs[idxRep]->GetObject();
   UpdateDistances(repObj, idxRep);
   // set the new minimum radius of this query.
   SetQueryRadius(GetMinimumRadius());

   // return the index of the new representative object for this input query.
   return idxRep;
}//end stInputQuery<ObjectType>::ChooseRepresentative

//------------------------------------------------------------------------------
template <class ObjectType, class EvaluatorType>
void stInputQuery<ObjectType, EvaluatorType>::UpdateDistances(
                           ObjectType * repObj, u_int32_t repObjIdx){
   u_int32_t idx;
   ObjectType * tmpObj;
   u_int32_t numberOfEntries = GetNumOfEntries();

   // set the idx of representative.
   SetRepresentativeIndex(repObjIdx);

   for (idx=0; idx < numberOfEntries; idx++){
      if (idx != repObjIdx){
         // It is not the representative object. Get the current object
         tmpObj = Pairs[idx]->GetObject();
         Pairs[idx]->SetDistance(Evaluator->GetDistance(repObj, tmpObj));
      }else{
         //it is the representative object
         Pairs[idx]->SetDistance(0.0);
      }//end if
   }//end for

}//end stInputQuery<ObjectType>::UpdateDistances

//------------------------------------------------------------------------------
template < class ObjectType, class EvaluatorType>
u_int32_t stInputQuery<ObjectType, EvaluatorType>::ChooseRepresentativeSmallRadius(){
   double radius;
   u_int32_t idx;
   u_int32_t numOfEntries = GetNumOfEntries();
   u_int32_t idxRep = 0;
   double minRadius = MAXDOUBLE;

   for (idx = 0; idx < numOfEntries; idx++){
      radius = Pairs[idx]->GetRadius();
      if (radius < minRadius){
         minRadius = radius;
         idxRep = idx;
      }//end if
   }//end for
   // set the radius query.
   SetQueryRadius(minRadius);
   // return the index of the new representative object.
   return idxRep;
}//end stInputQuery::ChooseRepresentativeSmallRadius()

