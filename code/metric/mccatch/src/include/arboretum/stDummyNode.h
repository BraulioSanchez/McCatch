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
* This file defines the Dummy Tree node.
*
* @version 1.0
* $Revision: 1.12 $
* $Date: 2005-03-21 20:04:50 $
* $Author: marcos $
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
// Copyright (c) 2002 GBDI-ICMC-USP
#ifndef __STDUMMYNODE_H
#define __STDUMMYNODE_H

#include <arboretum/stPage.h>
#include <stdexcept>

/**
* This class implements the node of the Dummy Tree. Since the Dummy Tree is a
* sequential list, this node holds only objects and a link to the next node.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @ingroup dummy
*/
// +-----------------------------------------------------------------------------+
// | Occupation | Next | Offset0 | ... | OffsetN |... | ObjectN | ... | Object 0 |
// +-----------------------------------------------------------------------------+
class stDummyNode{
   public:
      /**
      * Creates a new instance of this class. The parameter <i>page</i> is an
      * instance of stPage that hold the node data.
      *
      * <P>The parameter <i>create</i> tells to stIndexPage what operation will
      * be performed. True means that the page will be initialized and false
      * means that the page will be used as it is. The default value is false.
      *
      * @param page The page that hold the data of this node.
      * @param create The operation to be performed.
      */
      stDummyNode(stPage * page, bool create = false);

      /**
      * Returns the associated page.
      *
      * @return The associated page.
      */
      stPage * GetPage(){
         return Page;
      }//end GetPage

      /**
      * Returns the ID of the associated page.
      *
      * @return The ID of the associated page.
      */
      u_int32_t GetPageID(){
         return Page->GetPageID();
      }//end GetPage

      /**
      * Returns the number of entries in this node.
      *
      * @return the number of entries.
      * @see GetEntry()
      * @see GetObject()
      * @see GetObjectSize()
      */
      u_int32_t GetNumberOfEntries(){
         return this->Header->Occupation;
      }//end GetNumberOfEntries

      /**
      * This method adds an object to this node.
      *
      * @param size The size of the object in bytes.
      * @param object The object data.
      * @return The position in the vector Entries or a negative value for failure.
      * @see RemoveObject()
      */
      int AddEntry(u_int32_t size, const unsigned char * object);

      /**
      * Gets the serialized object. Use GetObjectSize to determine the size of
      * the object.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return A pointer to the serialized object.
      * @see GetObjectSize()
      */
      const unsigned char * GetObject(int idx);

      /**
      * Returns the size of the object in bytes. Use GetObject() to get the
      * object data.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The size of the serialized object.
      * @see GetObject()
      */
      u_int32_t GetObjectSize(int idx);

      /**
      * Removes an entry from this object.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @see GetObjectSize()
      * @todo This method is not implemented yet.
      */
      void RemoveEntry(u_int32_t idx);

      /**
      * Gets the next node linked to this node.
      */
      u_int32_t GetNextNode(){

         return this->Header->Next;
      }//end GetNextNode

      /**
      * Sets the next node linked to this node.
      *
      * @param id The ID of the next node.
      */
      void SetNextNode(u_int32_t idx){
         this->Header->Next = idx;
      }//end SetNextNode

   private:

      /**
      * Header of the stDummyNode.
      */
      typedef struct DummyHeader{
         /**
         * Occupation of this node.
         */
         u_int32_t Occupation;

         /**
         * The ID of the next page.
         */
         u_int32_t Next;
      }stDummyHeader;//end

      /**
      * The associated page.
      */
      stPage * Page;

      /**
      * Header of this node.
      */
      stDummyHeader * Header;

      /**
      * Entries.
      */
      u_int32_t * Entries;

      /**
      * Returns the free space available in this node.
      */
      u_int32_t GetFree();
      
};//end stDummyNode

#endif //__STDUMMYNODE_H
