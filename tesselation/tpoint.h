#ifndef __TPOINT_H__
#define __TPOINT_H__

#include <PalmOS.h>

#define POINTSIZE 4

typedef struct tpoint_tag {
  double x;
  double y;
} TPoint_type;

void TPointDraw(TPoint_type p1);
void TPointDrawLine(TPoint_type p1, TPoint_type p2);
TPoint_type TPointAddXY(TPoint_type p1, double x, double y);
TPoint_type TPointAddPoint(TPoint_type p1, TPoint_type p2);
TPoint_type TPointMinusPoint(TPoint_type p1, TPoint_type p2);
TPoint_type TPointMul(TPoint_type p1, double lambda);

// TPoint_type TPointRotate(TPoint_type p1,double rot);
double TPointRadius(TPoint_type p1);
double TPointDistanceFromLine(TPoint_type p1, TPoint_type p2,
			      TPoint_type p3);
double TPointDistanceFromPoint(TPoint_type p1, TPoint_type p2);
double TPointDistanceFromXY(TPoint_type p1, double x, double y);
Boolean TPointHitXY(TPoint_type p1, double hx, double hy);

// not sure about this one
#define sqr(x) (x*x)

#endif
