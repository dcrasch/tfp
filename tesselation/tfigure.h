#ifndef __TFIGURE_H__
#define __TFIGURE_H__

#include <PalmOS.h>
#include "tline.h"
#include "tesselation.h"

#define DESCSIZE   32

typedef struct tfigure_tag {
  TLinenode_type *rootnode;
  TVertexnode_type *selvertex;
  TVertexnode_type *sellast;
  TLinenode_type *selline;

  TPoint_type oldp;
  double scale;
  double rotinc;

  int gridincx,gridincy;
  int shiftx,shifty; 
  int offx,offy;

  char name[DESCSIZE];
} TFigure_type;

TFigure_type *TFigureCreate(double scale,int gridincx,int gridincy,
			   int shiftx,int shifty,double rotinc,int offx,int offy,char *name);

void TFigureFree(TFigure_type *t1);
void TFigureRemovePoint(TFigure_type *t1);

void TFigureDraw(TFigure_type *t1);
void TFigureRedraw(TFigure_type *t1);
void TFigureTesselate(TFigure_type *t1);

Boolean TFigureMouseDown(TFigure_type *t1,int x,int y);
Boolean TFigureMouseDrag(TFigure_type *t1,int x,int y);
void TFigureMouseUp(TFigure_type *t1);

#endif  
    
   
    
