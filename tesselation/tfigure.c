#include "tfigure.h"
#include "tfill.h"
#include "TinyTrig.h"

TFigure_type *TFigureCreate(double scale, int gridincx, int gridincy,
			    int shiftx, int shifty, double rotinc,
			    int offx, int offy, char *name)
{
    TFigure_type *t1;

    t1 = (TFigure_type *) MemPtrNew(sizeof(TFigure_type));

    t1->scale = scale;
    t1->gridincx = gridincx;
    t1->gridincy = gridincy;


    t1->shiftx = shiftx;
    t1->shifty = shifty;
    if (rotinc == 0)
	rotinc = TWO_PI;
    t1->rotinc = rotinc;

    t1->selvertex = NULL;
    t1->sellast = NULL;

    t1->oldp.x = 0;
    t1->oldp.y = 0;

    t1->offx = offx;
    t1->offy = offy;

    StrNCopy(t1->name, name, DESCSIZE);
    t1->name[DESCSIZE - 1] = 0;
    return t1;
}


void TFigureDraw(TFigure_type * t1)
{
    if (t1) {
	TLinenode_type *l1 = t1->rootnode;
	WinPushDrawState();
	WinSetClip(&(drawRect));
	while (l1 != NULL) {
	    TLineDraw(*l1, t1->offx, t1->offy);
	    l1 = l1->next;
	}

	if (t1->selvertex) {
	    WinSetForeColor(137);
	    TPointDraw(TPointAddXY(t1->selvertex->p1, t1->offx, t1->offy));
	    TPointDraw(TPointAddXY(t1->selvertex->p2, t1->offx, t1->offy));
	}
	WinResetClip();
	WinPopDrawState();
    }
}

void TFigureTesselate(TFigure_type * t1)
{
    if (t1) {
	double rot;
	int sx, sy;
	double ca, sa;
	int minx = -(t1->scale) * (t1->gridincx / 2 + t1->offx);
	int maxx = 160 + t1->scale * (t1->gridincx / 2);
	int miny = -(t1->scale) * (t1->gridincy / 2 + t1->offy);
	int maxy = 160 + t1->scale * (t1->gridincy / 2 + t1->offy);
	WinPushDrawState();
	WinSetClip(&(drawRect));
	WinEraseRectangle(&(drawRect), 0);

	sx = 60;
	sy = 60;
	while (miny <= maxy) {
	    sx = minx;
	    sy = miny;
	    while (sx <= maxx) {
		rot = 0;
		while (rot < TWO_PI) {
		    TLinenode_type *l1 = t1->rootnode;
		    ca = _cos(rot);
		    sa = _sin(rot);
		    while (l1 != NULL) {
			// calculate fillcoord

			TLineTesselate(*l1, sx, sy, t1->scale, ca, sa);
			l1 = l1->next;
		    }
		    rot += t1->rotinc;
		}
		sx += t1->scale * t1->gridincx;
		sy += t1->scale * t1->shifty;
	    }
	    minx += t1->scale * t1->shiftx;
	    miny += t1->scale * t1->gridincy;
	    if (minx > 0) {
		minx -= t1->scale * t1->gridincx;
		maxy -= t1->scale * t1->shifty;
	    }
	}
#ifdef __TFILL_H__
	TFigureFill(t1);
#endif
	WinResetClip();
	WinPopDrawState();
    }
}

Boolean TFigureMouseDown(TFigure_type * t1, int x, int y)
{
    if (t1) {
	TLinenode_type *htl;
	TVertexnode_type *htv = NULL;
	if (t1->selvertex != NULL)
	    return false;
	htl = t1->rootnode;
	while (htl) {
	    htv = TLineHit(htl, x - t1->offx, y - t1->offy);
	    if (!htv)
		htv = TLineBreakAtXY(htl, x - t1->offx, y - t1->offy);
	    if (htv) {
		t1->selline = htl;
		t1->selvertex = htv;
		t1->oldp.x = x;
		t1->oldp.y = y;
		return true;
	    }
	    htl = htl->next;
	}
	t1->selvertex = NULL;
    }
    return false;
}

Boolean TFigureMouseDrag(TFigure_type * t1, int x, int y)
{
    if (t1 && t1->selvertex) {
	TPoint_type pa = { x - t1->oldp.x, y - t1->oldp.y };
	if ((pa.x == 0) && (pa.y == 0))
	    return false;
	if (t1->selline->corrp) {
	    t1->selvertex->p1.x += (pa.x * t1->selline->ca +
				    pa.y * t1->selline->sa);
	    t1->selvertex->p1.y += (-pa.x * t1->selline->sa +
				    pa.y * t1->selline->ca);
	    t1->selvertex->p2 = TPointAddPoint(t1->selvertex->p2, pa);
	} else {
	    t1->selvertex->p1 = TPointAddPoint(t1->selvertex->p1, pa);

	    t1->selvertex->p2.x += (pa.x * t1->selline->ca -
				    pa.y * t1->selline->sa);
	    t1->selvertex->p2.y += (pa.x * t1->selline->sa +
				    pa.y * t1->selline->ca);
	}
	t1->oldp.x = x;
	t1->oldp.y = y;
	return true;
    }
    return false;
}


void TFigureMouseUp(TFigure_type * t1)
{
    if (t1) {
	if (t1->selvertex)
	    t1->sellast = t1->selvertex;
	t1->selvertex = NULL;
    }
}


void TFigureRedraw(TFigure_type * t1)
{
    if (t1) {
	WinEraseRectangle(&(drawRect), 0);
	TFigureDraw(t1);
    }
}

void TFigureRemovePoint(TFigure_type * t1)
{
    // make sure not a base point!
    TVertexnode_type *v1;
    if (t1 && t1->selline && t1->sellast) {
	v1 = t1->selline->rootnode;
	while (v1->next) {
	    if (v1->next == t1->sellast) {
		v1->next = v1->next->next;
		TVertexnodeFree(t1->sellast);
		t1->sellast = NULL;
	    }
	    v1 = v1->next;
	}
    }
}

void TFigureFree(TFigure_type * t1)
{
    if (t1) {
	TLinenode_type *l1;
	TLinenode_type *next = NULL;

	l1 = t1->rootnode;
	while (l1) {
	    next = l1->next;
	    TLinenodeFree(l1);
	    l1 = next;
	}
	t1->selvertex = NULL;
	t1->sellast = NULL;
	t1->rootnode = NULL;
	t1->selline = NULL;

	MemPtrFree(t1);
    }
}
