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

/*! \file tvertex.h
*/



#ifndef __TVERTEX_H__
#define __TVERTEX_H__
#include "tpoint.h"

/*!
  \brief vertex node structure
*/
typedef struct tvertexnode_tag {
    TPoint_type p1; //!< point
    TPoint_type p2; //!< corresponding point
    struct tvertexnode_tag *next; //!< next vertex
} TVertexnode_type;

/*!
  \brief Create a a vertex node
  \param x1 x coordinate
  \param y1 y coordinate
  \param x2 x coordinate corresponding point
  \param y2 y coordinate corresponding point
  \param next next vertex
*/
TVertexnode_type *TVertexnodeCreateXY(double x1, double y1, double x2, double y2, TVertexnode_type * next);

/*!
  \brief Insert a vertex after the node
  \param v1 vertex in the vertex list
  \param x1 x coordinate
  \param y1 y coordinate
  \param x2 x coordinate corresponding point
  \param y2 y coordinate corresponding point
*/
void TVertexnodeInsert(TVertexnode_type * v1, double x1,
		       double y1, double x2, double y2);

/*!
  \brief Add a vertex to the end of the vertex list
  \param rootnode root of the vertex list
  \param x1 x coordinate
  \param y1 y coordinate
  \param x2 x coordinate corresponding point
  \param y2 y coordinate corresponding point
*/
void TVertexnodeAdd(TVertexnode_type * rootnode, double x1,
		    double y1, double x2, double y2);
/*!
  \brief free the memory of the vertex
  \param v1 vertex
*/
void TVertexnodeFree(TVertexnode_type * v1);
#endif
