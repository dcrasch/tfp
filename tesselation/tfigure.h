/*
 * Tesselation 
 *
 * Copyright (c) 2001,2002 David Rasch <drasch@users.sourceforge.net>
 * 
 *
 * Licensed under the GNU GPL, version 2 or later 
 *
 * See file "COPYING"
 * that you should have received with this program 
 * 
 * or visit
 * http://www.gnu.org/copyleft/gpl.html 
 *
 * 
 */
/*! \file tfigure.h
*/

#ifndef __TFIGURE_H__
#define __TFIGURE_H__

#include <PalmOS.h>
#include "tline.h"
#include "tesselation.h"

/*!
  \brief max size of description
*/
#define DESCSIZE   32

/*!
  \brief structure of figure
*/
typedef struct tfigure_tag {
    TLinenode_type *rootnode;    //!< rootnode
    TVertexnode_type *selvertex; //!< selected vertex
    TVertexnode_type *sellast;   //!< last selected vertex
    TLinenode_type *selline;     //!< selected line

    TPoint_type oldp; //!< old x and y mouse coordinates
    double scale; //!< scale 
    double rotinc; //!< rotation

    int gridincx; //!< x increase each column
    int gridincy; //!< y increase each column
    int shiftx; //!< x increase each row
    int shifty; //!< y increase each row

    int offx; //!< edit figure x offset
    int offy; //!< edit figure y offset
    double offscale; //!< edit figure scale

    char name[DESCSIZE]; //!< name of the figure
} TFigure_type;


TFigure_type *TFigureCreate(double scale, int gridincx, int gridincy,
			    int shiftx, int shifty, double rotinc,
			    int offx, int offy, char *name);

void TFigureFree(TFigure_type * t1);
void TFigureRemovePoint(TFigure_type * t1);

void TFigureDraw(TFigure_type * t1);
void TFigureRedraw(TFigure_type * t1);
void TFigureTesselate(TFigure_type * t1, int mode);

Boolean TFigureMouseDown(TFigure_type * t1, int x, int y);
Boolean TFigureMouseDrag(TFigure_type * t1, int x, int y);
void TFigureMouseUp(TFigure_type * t1);

void TFigureFit(TFigure_type * t1);
void TFigureZoom(TFigure_type * t1, double zoom);

#endif
