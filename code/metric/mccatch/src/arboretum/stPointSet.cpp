/**********************************************************************
* GBDI Arboretum - Copyright (c) 2002-2006 GBDI-ICMC-USP
*
*                           Homepage: http://gbdi.icmc.usp.br/arboretum
**********************************************************************/
/* ====================================================================
 * The GBDI-ICMC-USP Software License Version 1.0
 *
 * Copyright (c) 2006 Grupo de Bases de Dados e Imagens, Instituto de
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
* This file implements the class stStorageGrid.
*
* @version 2.0
* @author Caetano Traina Jr (caetano@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Elaine Parros Machado de Sousa (parros@icmc.usp.br)
* @author Ana Carolina Riekstin (anacarol@grad.icmc.usp.br)
*/
// Copyright (c) 2005-2006 GBDI-ICMC-USP

#include <arboretum/stPointSet.h>

//---------------------------------------------------------------------------
// Class stPointSet
//---------------------------------------------------------------------------
stPointSet::stPointSet(int NumberOfPoints) {
   count = firstPoint = lastPoint = 0;
   logR = new double[NumberOfPoints]; //The values of X in the graphic
   logSqR = new double[NumberOfPoints]; //the values of Y in the graph
   for (int i=0; i<NumberOfPoints; i++){
      logR[i] = 0;
      logSqR[i] = 0;
   }//end for
   maxCount = NumberOfPoints;
}//end stPointSet::stPointSet

//---------------------------------------------------------------------------
stPointSet::stPointSet(const stPointSet & set) {
   int i;
   count = set.count;
   maxCount = set.maxCount;
   logR = new double[maxCount]; //The values of X in the graphic
   logSqR = new double[maxCount]; //the values of Y in the graph
   for (i=0; i<count; i++){ //Setting the values
      logR[i] = set.logR[i];
      logSqR[i] = set.logSqR[i];
   }//end for
   for (i < count; i < maxCount; i++){
      logR[i] = 0;
      logSqR[i] = 0;
   }//end for
   firstPoint = set.firstPoint;
   lastPoint = set.lastPoint;
   Alpha = set.Alpha;
   Beta = set.Beta;
   error = set.error;
}//end stPointSet::stPointSet

//---------------------------------------------------------------------------
stPointSet::~stPointSet() {
   delete[] logR;
   delete[] logSqR;
}//end stPointSet::~stPointSet

//---------------------------------------------------------------------------
stPointSet & stPointSet::operator += (const stPointSet & set){
   int i, j, aux;
   aux = this->count;
   assert((this->count += set.count -1) <= maxCount); //verifies if the sum is less than maxCount
   for (i = aux, j=1; i < this->count; i++, j++){ //Setting the values to add the set
      this->logR[i] = set.logR[j];
      this->logSqR[i] = set.logSqR[j];
   }//end for
   this->lastPoint = set.lastPoint;
   this->Alpha = 0;
   this->Beta = 0;
   this->error = 0;
   return *this;
}//end stPointSet & stPointSet::operator +=

//---------------------------------------------------------------------------
stPointSet & stPointSet::operator -= (const stPointSet & set){
   int i,j,aux;
   bool equal;
   assert(this->maxCount >= set.count); //Verifies if maxCount is bigger than count
   aux = this->count - set.count + 1;
   for (i = aux-1 , j=0, equal = 1; j < set.count && equal == 1; i++, j++) {//Setting values
      equal = (this->logR[i] == set.logR[j] && this->logSqR[i] == set.logSqR[j]);
   }//end for
   if (equal){ //If equal, it will be removed from the graph
      aux = this->count;
      this->count -= set.count -1;
      for (i = this->count; i < aux; i++){
         this->logR[i] = 0;
         this->logSqR[i] = 0;
      }//end for
   }//end if

   this->lastPoint = set.firstPoint;
   this->Alpha = 0;
   this->Beta = 0;
   this->error = 0;
   return *this;
}//end stPointSet & stPointSet::operator -=

//---------------------------------------------------------------------------
stPointSet & stPointSet::operator = (const stPointSet & set){
   int i;
   // assert (this->maxCount >= set.count);
   this->count = set.count;
   for (i = 0; i < this->count; i ++){ //Until count, it will receive the current values, just copying
      this->logR[i] = set.logR[i];
      this->logSqR[i] = set.logSqR[i];
   }//end for
   for (i = this->count; i < maxCount; i ++){ //The others are zero yet
      this->logR[i] = 0;
      this->logSqR[i] = 0;
   }//end for
   this->firstPoint = set.firstPoint;
   this->lastPoint = set.lastPoint;
   this->Alpha = set.Alpha;
   this->Beta = set.Beta;
   this->error = set.error;
   return *this;
}//end stPointSet & stPointSet::operator =

//---------------------------------------------------------------------------
void stPointSet::Clear( ){
   for (int i = 0; i< count; i++){
      logR[i] = 0;
      logSqR[i] = 0;
   }//end for
   lastPoint = lastPoint = count = 0;
   Alpha = Beta = error = 0.0;
}//end stPointSet::Clear
