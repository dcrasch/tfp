#include "TinyTrig.h"
#include "tline.h"

TVertexnode_type *TLineBreakAtXY(TLinenode_type * l1, double x, double y,
				 double ps)
{
    TVertexnode_type *otv = l1->rootnode;
    TPoint_type pm = { x, y };
    double tx, ty;
    double len;
    double dx, dy;
    l1->corrp = false;
    while (otv->next != NULL) {

	if (_abs(TPointDistanceFromLine(pm, otv->p1, otv->next->p1)) < ps) {

	    len = TPointDistanceFromPoint(otv->p1, otv->next->p1);
	    if ((TPointDistanceFromPoint(pm, otv->p1) < len) &&
		(TPointDistanceFromPoint(pm, otv->next->p1) < len)) {
		dx = x - otv->p1.x;
		dy = y - otv->p1.y;
		tx = otv->p2.x + (dx * l1->ca - dy * l1->sa);
		ty = otv->p2.y + (dx * l1->sa + dy * l1->ca);
		TVertexnodeInsert(otv, x, y, tx, ty);
		l1->corrp = false;
		return otv->next;
	    }
	}

	if (_abs(TPointDistanceFromLine(pm, otv->p2, otv->next->p2)) < ps) {
	    len = TPointDistanceFromPoint(otv->p2, otv->next->p2);
	    if ((TPointDistanceFromPoint(pm, otv->p2) < len) &&
		(TPointDistanceFromPoint(pm, otv->next->p2) < len)) {
		dx = x - otv->p2.x;
		dy = y - otv->p2.y;
		// cos(-x) = cos(x), sin(-x)=-sin(x)
		tx = otv->p1.x + (dx * l1->ca + dy * l1->sa);
		ty = otv->p1.y + (-dx * l1->sa + dy * l1->ca);
		TVertexnodeInsert(otv, tx, ty, x, y);
		l1->corrp = true;
		return otv->next;
	    }
	}
	otv = otv->next;
    }
    return NULL;
}

TVertexnode_type *TLineHit(TLinenode_type * l1, double x, double y,
			   double ps)
{
    TVertexnode_type *htv = l1->rootnode;
    // don't hit on rootnode;
    if (htv->next)
	htv = htv->next;
    else
	return NULL;

    // don't hit on lastnode
    while (htv->next) {
	l1->corrp = false;
	if (TPointHitXY(htv->p1, x, y, ps)) {
	    return htv;
	}
	if (TPointHitXY(htv->p2, x, y, ps)) {
	    l1->corrp = true;
	    return htv;
	}
	htv = htv->next;
    }
    return NULL;
}

void TLineDraw(TLinenode_type l1, int x, int y, double scale)
{
    TVertexnode_type *otv = l1.rootnode;
    TPoint_type pf, pt;
    TPointDraw(TPointAddXY(TPointMul(otv->p1, scale), x, y));
    while (otv->next != NULL) {
	pf = TPointAddXY(TPointMul(otv->p1, scale), x, y);
	pt = TPointAddXY(TPointMul(otv->next->p1, scale), x, y);
	TPointDraw(pt);
	TPointDrawLine(pf, pt);
	pf = TPointAddXY(TPointMul(otv->p2, scale), x, y);
	pt = TPointAddXY(TPointMul(otv->next->p2, scale), x, y);
	TPointDraw(pt);
	TPointDrawLine(pf, pt);
	otv = otv->next;
    }
}

void
TLineTesselate(TLinenode_type l1, int x, int y, double scale, double ca,
	       double sa)
{
    TVertexnode_type *otv = l1.rootnode;
    int ox, oy;
    int nx, ny;
    ox = (int)((otv->p1.x * ca - otv->p1.y * sa) * scale + x);
    oy = (int)((otv->p1.x * sa + otv->p1.y * ca) * scale + y);
    while (otv->next != NULL) {
	nx = (int)((otv->next->p1.x * ca - otv->next->p1.y * sa) * scale + x);
	ny = (int)((otv->next->p1.x * sa + otv->next->p1.y * ca) * scale + y);
	WinDrawLine(ox, oy, nx, ny);
	ox = nx;
	oy = ny;
	otv = otv->next;
    }
}

TLinenode_type *TLinenodeCreateXY(double x1, double y1,
				  double x2, double y2,
				  double x3, double y3,
				  double x4, double y4, double rot,
				  TLinenode_type * next)
{
    TLinenode_type *l1;
    l1 = (TLinenode_type *) MemPtrNew(sizeof(TLinenode_type));
    l1->rootnode = TVertexnodeCreateXY(x1, y1, x3, y3, NULL);
    TVertexnodeAdd(l1->rootnode, x2, y2, x4, y4);
    l1->ca = _cos(rot);
    l1->sa = _sin(rot);
    l1->next = next;
    return l1;
}

void
TLinenodeInsert(TLinenode_type * l1,
		double x1, double y1,
		double x2, double y2,
		double x3, double y3, double x4, double y4, double rot)
{
    TLinenode_type *l2;
    l2 = TLinenodeCreateXY(x1, y1, x2, y2, x3, y3, x4, y4, rot, l1->next);
    l1->next = l2;
}

void
TLinenodeAdd(TLinenode_type * rootnode,
	     double x1, double y1,
	     double x2, double y2,
	     double x3, double y3, double x4, double y4, double rot)
{
    TLinenode_type *last = rootnode;

    while (last->next != NULL)
	last = last->next;
    TLinenodeInsert(last, x1, y1, x2, y2, x3, y3, x4, y4, rot);
}

void TLinenodeFree(TLinenode_type * l1)
{
    TVertexnode_type *v1 = l1->rootnode;
    TVertexnode_type *next = NULL;
    while (v1) {
	next = v1->next;
	TVertexnodeFree(v1);
	v1 = next;
    }
    l1->rootnode = NULL;
    l1->lastnode = NULL;
    MemPtrFree(l1);
}

void TLineFit(TLinenode_type * l1, TPoint_type * pmax, TPoint_type * pmin)
{
    TVertexnode_type *v1 = l1->rootnode;

    while (v1) {

	if (v1->p1.x > pmax->x)
	    pmax->x = v1->p1.x;

	if (v1->p1.y > pmax->y)
	    pmax->y = v1->p1.y;

	if (v1->p1.x < pmin->x)
	    pmin->x = v1->p1.x;

	if (v1->p1.y < pmin->y)
	    pmin->y = v1->p1.y;

	if (v1->p2.x > pmax->x)
	    pmax->x = v1->p2.x;

	if (v1->p2.y > pmax->y)
	    pmax->y = v1->p2.y;

	if (v1->p2.x < pmin->x)
	    pmin->x = v1->p2.x;

	if (v1->p2.y < pmin->y)
	    pmin->y = v1->p2.y;

	v1 = v1->next;
    }

}
