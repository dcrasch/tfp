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
/*! \file tline.h
*/

#ifndef __TLINE_H__
#define __TLINE_H__

#include "tvertex.h"


/*!
  \brief line node structure
*/
typedef struct tline_tag {
  TVertexnode_type *rootnode; //!< root vertex of node list
  TVertexnode_type *lastnode; //!< last vertex in node list
  double ca; //!< precalculate cosine value
  double sa; //!< precalculate sine value
  Boolean corrp; //!< mouse on corresponding point yes or no
  struct tline_tag *next; //!< next line
} TLinenode_type;

/*!
  \brief create a new line
  \param x1 x coordinate first point
  \param y1 y coordinate first point
  \param x2 x coordinate first corresponding point
  \param y2 y coordinate first corresponding point
  \param x3 x coordinate second point
  \param y3 y coordinate second point
  \param x4 x coordinate second corresponding point
  \param y4 y coordinate second corresponding point
  \param rot rotation of corresponding point
  \param next next line in the list
*/
TLinenode_type *TLinenodeCreateXY(double x1, double y1,
				  double x2, double y2,
				  double x3, double y3,
				  double x4, double y4, double rot,
				  TLinenode_type * next);
/*!
  \brief free the line and all its vertices
  \param l1 line
*/
void TLinenodeFree(TLinenode_type * l1);

/*!
  \brief break a line in two at x,y with a margin of ps pixels
  \param l1 line
  \param x x coordinate
  \param y y coordinate
  \param ps margin
  \return the new point created or null if not on line
*/
TVertexnode_type *TLineBreakAtXY(TLinenode_type * l1, double x, double y,
				 double ps);
/*!
  \brief check if the cursor is on the points of the line
  \param l1 line
  \param x x coordinate
  \param y y coordinate
  \param ps margin
  \return the point hit or null if no point is hit
*/
TVertexnode_type *TLineHit(TLinenode_type * l1, double x, double y,
			   double ps);
/*!
  \brief draw both lines and point with squares
  \param l1 line
  \param x x coordinate addition
  \param y y coordinate addition
  \param scale scaling of the drawing
*/
void TLineDraw(TLinenode_type l1, int x, int y, double scale);

/*!
  \brief only the normal line
  \param l1 line
  \param x x coordinate addition
  \param y y coordinate addition
  \param ca cosine rotation
  \param sa sine rotation
*/
void TLineTesselate(TLinenode_type l1, int x, int y, double ca, double sa);

/*!
  \brief insert a line into the list
  \param l1 line before the new line
  \param x1 x coordinate first point
  \param y1 y coordinate first point
  \param x2 x coordinate first corresponding point
  \param y2 y coordinate first corresponding point
  \param x3 x coordinate second point
  \param y3 y coordinate second point
  \param x4 x coordinate second corresponding point
  \param y4 y coordinate second corresponding point
  \param rot rotation of corresponding point
*/
void TLinenodeInsert(TLinenode_type * l1,
		     double x1, double y1,
		     double x2, double y2,
		     double x3, double y3, double x4, double y4,
		     double rot);
/*!
  \brief insert a line into the list
  \param rootnode of the list
  \param x1 x coordinate first point
  \param y1 y coordinate first point
  \param x2 x coordinate first corresponding point
  \param y2 y coordinate first corresponding point
  \param x3 x coordinate second point
  \param y3 y coordinate second point
  \param x4 x coordinate second corresponding point
  \param y4 y coordinate second corresponding point
  \param rot rotation of corresponding point
*/
void TLinenodeAdd(TLinenode_type * rootnode, double x1, double y1,
		  double x2, double y2, double x3, double y3, double x4,
		  double y4, double rot);

/*!
  \brief calculate the min and max value of the points of the line
  \param rootnode of the list
  \param pmax highest values of x and y coordinates
  \param pmin lowest values of x and y coordinates
*/
void TLineFit(TLinenode_type * rootnode, TPoint_type * pmax,
	      TPoint_type * pmin);

#endif
