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

#include "tvertex.h"
#include "tesselation.h"

TVertexnode_type *TVertexnodeCreateXY(double x1, double y1,
				      double x2, double y2,
				      TVertexnode_type * next)
{
    TVertexnode_type *v1;
    v1 = (TVertexnode_type *) MemPtrNew(sizeof(TVertexnode_type));
    v1->p1.x = x1;
    v1->p1.y = y1;
    v1->p2.x = x2;
    v1->p2.y = y2;
    v1->next = next;
    return v1;
}

void
TVertexnodeInsert(TVertexnode_type * v1, double x1, double y1, double x2,
		  double y2)
{
    /* insert a new vertex node behind v1 */
    TVertexnode_type *v2;
    v2 = TVertexnodeCreateXY(x1, y1, x2, y2, v1->next);
    v1->next = v2;
}


void
TVertexnodeAdd(TVertexnode_type * rootnode, double x1, double y1,
	       double x2, double y2)
{
    /* insert a new vertex node at the rear */
    TVertexnode_type *last = rootnode;
    while (last->next != NULL)
	last = last->next;
    TVertexnodeInsert(last, x1, y1, x2, y2);

}

void TVertexnodeFree(TVertexnode_type * v1)
{
    v1->next = NULL;
    MemPtrFree(v1);
}
