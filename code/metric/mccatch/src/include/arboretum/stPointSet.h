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
* This file defines the class stStorageGrid.
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
#ifndef __STPOINTSET_H
#define __STPOINTSET_H

#include <assert.h>

//----------------------------------------------------------------------------
// class stPointSet
//----------------------------------------------------------------------------
/**
* This class is used to store the points from the database.
*
* @version 2.0
* @author Caetano Traina Jr (caetano@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Elaine Parros Machado de Sousa (parros@icmc.usp.br)
* @author Ana Carolina Riekstin (anacarol@grad.icmc.usp.br)
* @ingroup fastmap
*/
//---------------------------------------------------------------------------
class stPointSet {
   
   public:
   
      /**
      * Creates a new set of NumberOfPoints points
      *
      * @param stNumberOfPoints Number of points in the dataset.
      */
      stPointSet(int NumberOfPointsP);

      /**
      * Creates a new set of NumberOfPoints points copying it.
      */
      stPointSet(const stPointSet &);

      /**
      * Disposes this set of points
      */
      ~stPointSet();

      /**
      * Insert a set of point in the end.
      *
      * @param stPointSet.
      * @return stPointSet.
      */
      stPointSet & operator += (const stPointSet &);

      /**
      * Remove a set of points from the end.
      *
      * @param stPointSet.
      * @return stPointSet.
      */
      stPointSet & operator -= (const stPointSet & );

      /**
      * Copy the set of points.
      *
      * @param stPointSet.
      * @return stPointSet.
      */
      stPointSet & operator = (const stPointSet & );

      /**
      * Clear the point set.
      */
      void Clear();

      /**
      * Vector that will store the values of log(R) - R are the values of r
      * (the lenght of the side of a grid cell) to plot the graph to find out
      * de intrinsic dimension.
      */
      double * logR;

      /**
      * Vector that will store the values of log(SqR) - SqR is the sum of
      * squared occupancy for the grid of cell side r.
      */
      double * logSqR;

      /**
      * Value of Alpha, that is the Slope of the result line of the graph.
      */
      double Alpha;

      /**
      * Value of Beta, that is the Y increment of the result line of the graph.
      ,0

      */
      double Beta;

      /**
      * Value of the fitting error.
      */
      double error;

      /**
      * Index of the first point of the set.
      */
      int firstPoint;

      /**
      * Index of the last point of the set.
      */
      int lastPoint;

      /**
      * Current number of points in the set.
      */
      int count;

      /**
      * Maximum number of points in the set.
      */
      int maxCount;

};//end stPointSet
//---------------------------------------------------------------------------
#endif //__STPOINTSET_H 
