#ifndef __TLINE_H__
#define __TLINE_H__

#include "tvertex.h"

typedef struct tline_tag {
  TVertexnode_type *rootnode;
  TVertexnode_type *lastnode;
  double ca;
  double sa;
  Boolean corrp;
  struct tline_tag *next;
} TLinenode_type;

TLinenode_type *TLinenodeCreateXY(double x1, double y1,
				  double x2, double y2,
				  double x3, double y3,
				  double x4, double y4, double rot,
				  TLinenode_type * next);

void TLinenodeFree(TLinenode_type * l1);

TVertexnode_type *TLineBreakAtXY(TLinenode_type * l1, double x, double y);
TVertexnode_type *TLineHit(TLinenode_type * l1, double x, double y);

void TLineDraw(TLinenode_type l1, int x, int y, double scale);
void TLineTesselate(TLinenode_type l1, int x, int y,
		    double scale, double ca, double sa);

void TLinenodeInsert(TLinenode_type * l1,
		     double x1, double y1,
		     double x2, double y2,
		     double x3, double y3, double x4, double y4,
		     double rot);
void TLinenodeAdd(TLinenode_type * rootnode, double x1, double y1,
		  double x2, double y2, double x3, double y3, double x4,
		  double y4, double rot);

void TLineFit(TLinenode_type * rootnode, TPoint_type * pmax,
	      TPoint_type * pmin);

#endif
