/**********************************************************************
* GBDI Arboretum - Copyright (c) 2002-2004 GBDI-ICMC-USP
*
*                           Homepage: http://gbdi.icmc.usp.br/arboretum
**********************************************************************/
/* ====================================================================
 * The GBDI-ICMC-USP Software License Version 1.0
 *
 * Copyright (c) 2004 Grupo de Bases de Dados e Imagens, Instituto de
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
*
* This file defines the class stGHTree.
* $Author: seraphim $
*
* @author Enzo Seraphim (seraphim@unifei.edu.br)
*/
// Copyright (c) 2004 GBDI-ICMC-USP

#ifndef __STGNUPLOT3D_H
#define __STGNUPLOT3D_H


#include <stdexcept>
#include <arboretum/stGnuplot.h>

class stGnuplot3D: public stGnuplot {
   public:
   
      /**
      *
      */
      stGnuplot3D(int xvaluesCount, int barsCount);

      virtual ~stGnuplot3D();

      void SetYLabel(const char * label);

      void SetYRange(const char * label);

      void SetLogScaleX(bool log = true) { xlogscale = log; };
      void SetLogScaleY(bool log = true) { ylogscale = log; };
      void SetLogScaleXY(bool log = true) { xlogscale = log; ylogscale = log; };

      void SetBarTexture(int bar, int texture) {
         #ifdef __stDEBUG__
         if ((bar < 0) && (bar >= bars)){
            throw range_error("bar is out of range.");
         }//end if
         #endif //__stDEBUG__
         barsTextures[bar] = texture;
      };

      void SetBarLabel(int pos, const char * label);

      void SetXValueLabel(int pos, const char * label);

      void SetValue(int pos, int bar, long double value) {
   	   #ifdef __stDEBUG__
         if ((pos < 0) && (pos >= xvalues) && (bar < 0) && (bar >= bars)){
            throw range_error("pos or bar are out of range.");
         }//end if
         #endif //__stDEBUG__
         xrealvalues[pos][bar] = value;
      };

      long double GetValue(int pos, int bar) {
         #ifdef __stDEBUG__
         if ((pos < 0) && (pos >= xvalues) && (bar < 0) && (bar >= bars)){
            throw range_error("pos or bar are out of range.");
         }//end if
         #endif //__stDEBUG__
         return xrealvalues[pos][bar];
      };

      void SaveTo(char * filename, tOutputPlot type = plotWindows);

   private:
      bool xlogscale;
      bool ylogscale;
      int xvalues;
      int bars;
      char ** barsLabels;
      char ** xvaluesLabels;
      char * ylabel;
      char * yrange;
      int * barsTextures;
      long double **xrealvalues;
};

#endif // __STGNUPLOT3D_H
