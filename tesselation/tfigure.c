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
	while (l1 != NULL) {
	    TLineDraw(*l1, t1->offx, t1->offy, t1->offscale);
	    l1 = l1->next;
	}

	if (t1->selvertex) {
	    TPoint_type p1 =
		TPointAddXY(TPointMul(t1->selvertex->p1, t1->offscale),
			    t1->offx,
			    t1->offy);
	    TPoint_type p2 =
		TPointAddXY(TPointMul(t1->selvertex->p2, t1->offscale),
			    t1->offx,
			    t1->offy);

	    WinPushDrawState();
	    WinSetClip(&(drawRect));
	    WinSetForeColor(137);

	    TPointDraw(p1);
	    TPointDraw(p2);

	    WinResetClip();
	    WinPopDrawState();
	}
    }
}


void TFigureTesselate(TFigure_type * t1, int mode)
{
    if (t1) {
	double rot;
	int sx, sy;
	double ca, sa;
	int minx = (int) (-(t1->scale) * (t1->gridincx / 2 + t1->offx));
	int maxx = (int) (160 + t1->scale * (t1->gridincx / 2));
	int miny = (int) (-(t1->scale) * (t1->gridincy / 2 + t1->offy));
	int maxy = (int) (160 + t1->scale * (t1->gridincy / 2 + t1->offy));
	WinPushDrawState();
	WinSetClip(&(drawRect));
	WinEraseRectangle(&(drawRect), 0);

	while (miny <= maxy) {
	    sx = minx;
	    sy = miny;
	    while (sx <= maxx) {
		rot = 0;
		while (rot < TWO_PI) {
		    TLinenode_type *l1 = t1->rootnode;
		    ca = _cos(rot) * t1->scale;
		    sa = _sin(rot) * t1->scale;
		    while (l1 != NULL) {
			// calculate fillcoord

			TLineTesselate(*l1, sx, sy, ca, sa);
			l1 = l1->next;
		    }
		    rot += t1->rotinc;
		}
		sx += (int) (t1->scale * t1->gridincx);
		sy += (int) (t1->scale * t1->shifty);
	    }
	    minx += (int) (t1->scale * t1->shiftx);
	    miny += (int) (t1->scale * t1->gridincy);
	    if (minx > 0) {
		minx -= (int) (t1->scale * t1->gridincx);
		maxy -= (int) (t1->scale * t1->shifty);
	    }
	}
	TFigureFill(t1, mode);
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
	    htv =
		TLineHit(htl,
			 (x - t1->offx) / t1->offscale,
			 (y - t1->offy) / t1->offscale,
			 (POINTSIZE / t1->offscale));
	    if (!htv)
		htv =
		    TLineBreakAtXY(htl, (x - t1->offx) / t1->offscale,
				   (y - t1->offy) / t1->offscale,
				   (POINTSIZE / t1->offscale));
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
	TPoint_type pa = { (x - t1->oldp.x) / t1->offscale,
	    (y - t1->oldp.y) / t1->offscale
	};
	if ((pa.x == 0) && (pa.y == 0))
	    return false;
	if (t1->selline->corrp) {
	    t1->selvertex->p1.x +=
		(pa.x * t1->selline->ca + pa.y * t1->selline->sa);
	    t1->selvertex->p1.y +=
		(-pa.x * t1->selline->sa + pa.y * t1->selline->ca);
	    t1->selvertex->p2 = TPointAddPoint(t1->selvertex->p2, pa);
	} else {
	    t1->selvertex->p1 = TPointAddPoint(t1->selvertex->p1, pa);

	    t1->selvertex->p2.x +=
		(pa.x * t1->selline->ca - pa.y * t1->selline->sa);
	    t1->selvertex->p2.y +=
		(pa.x * t1->selline->sa + pa.y * t1->selline->ca);
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


void TFigureFit(TFigure_type * t1)
{
    TLinenode_type *l1 = t1->rootnode;
    TPoint_type pmax, pmin;
    double width, height;

    pmax = l1->rootnode->p1;
    pmin = pmax;

    TLineFit(l1, &pmax, &pmin);
    while (l1->next) {
	l1 = l1->next;
	TLineFit(l1, &pmax, &pmin);
    }

    width = abs((int) (pmax.x - pmin.x));
    height = abs((int) (pmax.y - pmin.y));

    // calculate the scale , fit the largest
    t1->offscale = 100.0 / (width > height ? width : height);

    // center the figure
    t1->offx = (int) (80 - (pmin.x + width / 2) * t1->offscale);
    t1->offy = (int) (80 - (pmin.y + height / 2) * t1->offscale);

}


void TFigureZoom(TFigure_type * t1, double zoom)
{
    TPoint_type middle;
    t1->offscale = t1->offscale * zoom;
    if (t1->selline->corrp) {
	middle = t1->sellast->p2;
    } else {
	middle = t1->sellast->p1;
    }
    t1->offx = (int) (80 - middle.x * t1->offscale);
    t1->offy = (int) (80 - middle.y * t1->offscale);
}
