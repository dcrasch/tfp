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

/*!
  \brief create a figure
  \param scale scale of figure
  \param gridincx increase x each column
  \param gridincy increase y each column
  \param shiftx increase x each row
  \param shifty increase y each row
  \param rotinc rotation of each figure
  \param offx offset x edit figure
  \param offy offset y edit figure
  \param name name of the figure
*/
TFigure_type *TFigureCreate(double scale, int gridincx, int gridincy,
			    int shiftx, int shifty, double rotinc,
			    int offx, int offy, char *name);

/*!
  \brief free the figure and all its lines
  \param t1 figure
*/
void TFigureFree(TFigure_type * t1);

/*!
  \brief remove the selected point from a figure 
  \param t1 figure
*/
void TFigureRemovePoint(TFigure_type * t1);

/*!
  \brief draw the edit figure
  \param t1 figure
*/
void TFigureDraw(TFigure_type * t1);

/*!
  \brief clear the screen and redraw the figure
  \param t1 figure
*/
void TFigureRedraw(TFigure_type * t1);

/*!
  \brief draw the tessellation
  \param t1 figure
  \param mode coloring mode
*/
void TFigureTesselate(TFigure_type * t1, int mode);


/*!
  \brief mouse down 
  \param t1 figure
  \param x x coordinate mouse
  \param y y coordinate mouse
*/
Boolean TFigureMouseDown(TFigure_type * t1, int x, int y);

/*!
  \brief mouse drag
  \param t1 figure
  \param x x coordinate mouse
  \param y y coordinate mouse
*/
Boolean TFigureMouseDrag(TFigure_type * t1, int x, int y);

/*!
  \brief mouse up
  \param t1 figure
*/
void TFigureMouseUp(TFigure_type * t1);

/*!
  \brief fit the edit figure inside the screen
  \param t1 figure
*/
void TFigureFit(TFigure_type * t1);

/*!
  \brief zoom in or out on a point
  \param t1 figure
  \param zoom zoom factor
*/
void TFigureZoom(TFigure_type * t1, double zoom);

#endif
