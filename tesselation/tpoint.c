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

#include "tpoint.h"
#include "TinyTrig.h"

void TPointDraw(TPoint_type p1)
{
    RectangleType rt =
	{ {(short int) (p1.x - POINTSIZE), (short int) (p1.y - POINTSIZE)},
    {(short int) (POINTSIZE * 2 + 1), (short int) (POINTSIZE * 2 + 1)}
    };

    WinDrawRectangleFrame(simpleFrame, &rt);
}

void TPointDrawLine(TPoint_type p1, TPoint_type p2)
{
    WinDrawLine((short int) p1.x, (short int) p1.y, (short int) p2.x,
		(short int) p2.y);
}

TPoint_type TPointAddPoint(TPoint_type p1, TPoint_type p2)
{
    TPoint_type p3 = { p1.x + p2.x, p1.y + p2.y };
    return p3;
}

TPoint_type TPointAddXY(TPoint_type p1, double x, double y)
{
    TPoint_type p2 = { p1.x + x, p1.y + y };
    return p2;
}

TPoint_type TPointMinusPoint(TPoint_type p1, TPoint_type p2)
{
    TPoint_type p3 = { p1.x - p2.x, p1.y - p2.y };
    return p3;
}

TPoint_type TPointMul(TPoint_type p1, double lambda)
{
    TPoint_type p2 = { p1.x * lambda, p1.y * lambda };
    return p2;
}

double TPointRadius(TPoint_type p1)
{
    return _sqrt((p1.x * p1.x + p1.y * p1.y));
}

double
TPointDistanceFromLine(TPoint_type p1, TPoint_type p2, TPoint_type p3)
{
    TPoint_type rv = TPointMinusPoint(p3, p2);
    double r = TPointRadius(rv);
    if (r != 0)
	return ((p1.x * rv.y - p1.y * rv.x + rv.x * p2.y -
		 rv.y * p2.x) / r);
    else
	return 0;
}

double TPointDistanceFromPoint(TPoint_type p1, TPoint_type p2)
{
    return _sqrt(sqr((p1.x - p2.x)) + sqr((p1.y - p2.y)));
}

Boolean TPointHitXY(TPoint_type p1, double hx, double hy, double ps)
{
    return ((hx >= (p1.x - ps)) && (hx <= (ps + p1.x)) &&
	    (hy >= (p1.y - ps)) && (hy < (ps + p1.y)));
}
