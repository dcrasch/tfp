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

#ifndef __TVERTEX_H__
#define __TVERTEX_H__
#include "tpoint.h"
typedef struct tvertexnode_tag {
    TPoint_type p1;
    TPoint_type p2;
    struct tvertexnode_tag *next;
} TVertexnode_type;

TVertexnode_type *TVertexnodeCreateXY(double x1, double y1, double x2,
				      double y2, TVertexnode_type * next);
void TVertexnodeInsert(TVertexnode_type * v1, double x1,
		       double y1, double x2, double y2);
void TVertexnodeAdd(TVertexnode_type * rootnode, double x1,
		    double y1, double x2, double y2);
void TVertexnodeFree(TVertexnode_type * v1);
#endif
