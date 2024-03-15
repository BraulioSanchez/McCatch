/**********************************************************************
* GBDI Arboretum - Copyright (c) 2002-2006 GBDI-ICMC-USP
*
*                           Homepage: http://gbdi.icmc.usp.br/arboretum
**********************************************************************/
/* ====================================================================
 * The GBDI-ICMC-USP Software License Version 1.0
 *
 * Copyright (c) 2006 Grupo de Bases de Dados e Imagens, Instituto de
 * Ciï¿½ncias Matemï¿½ticas e de Computaï¿½ï¿½o, University of Sï¿½o Paulo -
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
 *        de Dados e Imagens, Instituto de Ciï¿½ncias Matemï¿½ticas e de
 *        Computaï¿½ï¿½o, University of Sï¿½o Paulo - Brazil (the Databases 
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
* This file defines the interface stCellId and the classes stCellIdLowDimensional
* and stCellIdHighDimensional.
*
* @version 2.0
* @author Caetano Traina Jr (caetano@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Elaine Parros Machado de Sousa (parros@icmc.usp.br)
* @author Ana Carolina Riekstin (anacarol@grad.icmc.usp.br)
*
* @ingroup fastmap
*/
// Copyright (c) 2005-2006 GBDI-ICMC-USP

#ifndef __STCELLID_H
#define __STCELLID_H

#include <arboretum/stCommon.h>
#include <stdio.h>
#include <math.h>

//----------------------------------------------------------------------------
// class stCellId
//----------------------------------------------------------------------------
/**
* Interface to deal with different types of cell ids, depending on the number of
* dimensions
*
* @version 2.0
* @author Caetano Traina Jr (caetano@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Elaine Parros Machado de Sousa (parros@icmc.usp.br)
* @author Ana Carolina Riekstin (anacarol@grad.icmc.usp.br)
* @ingroup fastmap
*/
class stCellId {
   
   public:
   
      /**
      * Constructor
      */
      stCellId() {
      }//end stCellId

      /**
      * Destructor
      */
      virtual ~stCellId(){
      }//end ~stCellId

      /**
      * Operator += add a value to the variable
      *
      * @param ValueAdd The value to be added.
      */
      virtual void operator += (int valueAdd) = 0;

      /**
      * Operator = assign a value to a variable.
      *
      * @param stCellId The stCellId to be assigned.
      */
      virtual void operator = (stCellId &) = 0;

      /**
      * Operator == Compare two values.
      *
      * @param stCellId The stCellId to be compared.
      * @return 0 if equal, -1 if this < cell, 1  if this > cell.
      */
      virtual short operator == (stCellId &) = 0;

      /**
      * Prints CellId
      */
      virtual void print() = 0;

      /**
      * Does the left shift.
      *
      * @param indexTest Index of the dimension being tested.
      */
      virtual void LeftShift(int indexTest) = 0;

      /**
      * Does the right shift.
      *
      * @param indexTest Index of the dimension being tested.
      */
      virtual void operator >>=(int indexTest) = 0;

      /**
      * Operator & does the logic and.
      *
      * @param VariableCompare Variable to be compared.
      * @return bool.
      */
      virtual bool operator & (int variableCompare) = 0;

      /**
      * Reset index.
      */
      virtual void reset() = 0;
};

//----------------------------------------------------------------------------
// class stCellIdLowDimensional
//----------------------------------------------------------------------------
/**
* To identify cells in datasets with 32 or less dimensions.
*
*  @version 2.0
*  @author Caetano Traina Jr (caetano @icmc.usp.br)
*  @author Agma Juci Machado Traina (agma @icmc.usp.br)
*  @author Christos Faloutsos (christos @cs.cmu.edu)
*  @author Elaine Parros Machado de Sousa (parros @icmc.usp.br)
*  @author Ana Carolina Riekstin (anacarol @grad.icmc.usp.br)
*  @ingroup fastmap
*/
class stCellIdLowDimensional : public stCellId {
   
   private:
   
      /**
      * 32 bits - one for each attribute.
      */
      unsigned int index;
      
   public:
   
      /**
      * Constructor
      */
      stCellIdLowDimensional(){
         index = 0;
      }//end stCellIdLowDimensional

      /**
      * Destructor
      */
      virtual ~stCellIdLowDimensional() {
      }//end ~stCellIdLowDimensional

      /**
      * Operator += add a value to the variable
      *
      * @param valueAdd The value to be added.
      */
      virtual void operator += (int valueAdd) {
         index += valueAdd;
      }////end operator +=

      /**
      * Operator = assign a value to a variable.
      *
      * @param stCellId The stCellId to be assigned.
      */
      virtual void operator = (stCellId &cell) {
         this->index = (static_cast<stCellIdLowDimensional &>(cell)).index;
      }//end operator =

      /**
      * Operator == Compare two values.
      *
      * @param stCellId The stCellId to be compared.
      * @return 0 if equal, -1 if this < cell, 1  if this > cell.
      */
      virtual short operator == (stCellId &cell) {
         if (this->index == static_cast<stCellIdLowDimensional &>(cell).index) {
            return 0;
         }else if (this->index < static_cast<stCellIdLowDimensional &>(cell).index) {
            return -1;
         }else{
            return 1;
         }//end if
         //return (this->index == static_cast<stCellIdLowDimensional &>(cell).index);
      }//end operator ==

      /**
      * Print CellId
      */
      virtual void print() {
         printf("%d ", index);
      }//end print

      /**
      * Do the left shift.
      *
      * @param indexTest Index of the dimension being tested.
      */
      virtual void LeftShift(int indexTest) {
         index <<= 1;
      }//end LeftShift

      /**
      * Do the right shift.
      *
      * @param indexTest Index of the dimension being tested.
      */
      virtual void operator >>=(int indexTest) {
         index >>= indexTest;
      }//end operator >>=

      /**
      * Operator & do the logic and.
      *
      * @param VariableCompare Variable to be compared.
      * @return bool.
      */
      virtual bool operator & (int variableCompare) {
         return index&variableCompare;
      }//end operator &

      /**
      * Reset index.
      */
      virtual void reset() {
         index = 0;
      }//end reset

};

//----------------------------------------------------------------------------
// class stCellIdHighDimensional
//----------------------------------------------------------------------------
/**
* To identify cells in datasets with more than 32 dimensions.
*
*  @version 2.0
*  @author Caetano Traina Jr (caetano @icmc.usp.br)
*  @author Agma Juci Machado Traina (agma @icmc.usp.br)
*  @author Christos Faloutsos (christos @cs.cmu.edu)
*  @author Elaine Parros Machado de Sousa (parros @icmc.usp.br)
*  @author Ana Carolina Riekstin (anacarol @grad.icmc.usp.br)
*  @ingroup fastmap
*/
class stCellIdHighDimensional : public stCellId {
   
   private:
   
      /**
      * Vector that is gonna be the index.
      */
      unsigned char *index;

      /**
      * One position for each 8 attributes.
      */
      int nPos;
      
   public:
   
      /**
      * Constructor
      */
      stCellIdHighDimensional(int dimension){
         nPos = (int)ceil((double)dimension / 8);
         index = new unsigned char[nPos];  // one position for each 8 attributes
         memset(index, '\x0', nPos);
         //OBS: the logical position 0 corresponds to the last physical position of the array
      }//end stCellIdHighDimensional::stCellIdHighDimensional

      /**
      * Destructor
      */
      virtual ~stCellIdHighDimensional(){
         delete[] index;
      }

      /**
      * Operator += add a value to the variable
      *
      * @param valueAdd The value to be added.
      */
      virtual void operator += (int valueAdd);

      /**
      * Operator = assign a value to a variable.
      *
      * @param stCellId The stCellId to be assigned.
      */
      virtual void operator = (stCellId &cell) {
         memcpy(this->index, (static_cast<stCellIdHighDimensional &>(cell)).index, nPos);
      }//end operator =

      /**
      * Operator == Compare two values.
      *
      * @param stCellId The stCellId to be compared.
      * @return 0 if equal, <0 if this < cell, >0  if this > cell.
      */
      virtual short operator == (stCellId &cell) {
         return (memcmp (this->index, (static_cast<stCellIdHighDimensional &>(cell)).index, nPos));
      }//end operator ==

      /**
      * Do the left shift.
      *
      * @param indexTest Index of the dimension being tested.
      */
      virtual void LeftShift(int indexTest);

      /**
      * Do the right shift.
      *
      * @param indexTest Index of the dimension being tested.
      */
      virtual void operator >>=(int indexTest);

      /**
      * Operator & do the logic and.
      *
      * @param VariableCompare Variable to be compared.
      * @return bool.
      */
      virtual bool operator & (int variableCompare) {
         return index[nPos-1]&variableCompare;
      }//end operator &

      /**
      * Print CellId
      */
      virtual void print() {
         for (int i =0; i<nPos; i++){
            printf("%d ", index[i]);
         }//end for
      }//end print

      /**
      * Reset index.
      */
      virtual void reset() {
         memset(index, '\x0', nPos);
      }//end reset

};

#endif //__STCELLID_H
