#include "tesselation.h"
#include "tesselation_add.h"
#include "tfrecord.h"

Boolean DoAddFigure(void) {
  TFigure_type *t1;
  char *titleptr;
  FormPtr prevFrm = FrmGetActiveForm();
  FormPtr frm = FrmInitForm(formAddFigure);
  FieldPtr field = (FieldPtr)FrmGetObjectPtr(frm,FrmGetObjectIndex(frm,fieldFigureName));
  ListPtr list =  (ListPtr)FrmGetObjectPtr(frm,FrmGetObjectIndex(frm,listBaseFigures));

  FrmSetActiveForm(frm);
   
  if (FrmDoDialog(frm)==buttonAFOK) {
    switch (LstGetSelection(list)) {
    case 0: {
      t1=TFigureSquare();
      break; 
    }
    case 1: {
      t1=TFigureSquare2();
      break; 
    }
    case 2: {
      t1=TFigureSquare90();
      break; 
    }
    case 3: {
      t1=TFigureSquareN8();
      break; 
    }
    case 4: {
      t1=TFigureBrick();
      break; 
    }
    case 5: {
      t1=TFigureDiamond();
      break; 
    }
    case 6: {
      t1=TFigureTriangle();
      break; 
    }      
    default: { 
      if (prevFrm)
	FrmSetActiveForm(prevFrm);
      return false;
      break;
    }
    }

    titleptr=FldGetTextPtr(field);
    if (titleptr) {
      StrNCopy(t1->name,titleptr,19);
      t1->name[19]=0; 
    }

    TFigurerecordNew(t1); 
    TFigureFree(t1);
    if (prevFrm)
      FrmSetActiveForm(prevFrm);
    return true;
  }
  
  if (prevFrm)
    FrmSetActiveForm(prevFrm);
  return false;
}

/* base figures all figures are derivated from these figures */

TFigure_type *TFigureSquare() {
  TFigure_type *t1=TFigureCreate(0.4,80,80,0,0,TWO_PI,40,40,"Square");
  t1->rootnode=TLinenodeCreateXY(0,0,0,80,80,0,80,80,0,NULL);
  TLinenodeAdd(t1->rootnode,0,0,80,0,0,80,80,80,0);
  return t1;
}  

TFigure_type *TFigureSquare2() {
  TFigure_type *t1=TFigureCreate(0.4,100,50,50,0,TWO_PI,40,40,"Square2");
  t1->rootnode=TLinenodeCreateXY(0,0,0,50,100,0,100,50,0,NULL);
  TLinenodeAdd(t1->rootnode,50,0,50,50,50,0,50,50,0);
  TLinenodeAdd(t1->rootnode,0,0,50,0,50,50,100,50,0);
  TLinenodeAdd(t1->rootnode,0,50,50,50,50,0,100,0,0);
  return t1;
}

TFigure_type *TFigureSquare90() {
  TFigure_type *t1=TFigureCreate(0.3,160,160,0,0,HALF_PI,40,40,"Square90");
  t1->rootnode=TLinenodeCreateXY(0,0,0,80,
				 0,0,80,0,-HALF_PI,NULL);
  TLinenodeAdd(t1->rootnode,80,80,0,80,
	       80,80,80,0,HALF_PI);
  return t1;
}
  
TFigure_type *TFigureDiamond() {
  TFigure_type *t1=TFigureCreate(0.5,225,65,113,0,2*THIRD_PI,25,50,"Diamond");
  t1->rootnode=TLinenodeCreateXY(38,65,113,65,38,65,0,0,-2*THIRD_PI,NULL);
  TLinenodeAdd(t1->rootnode,75,0,0,0,75,0,113,65,-2*THIRD_PI);
  return t1;
}

TFigure_type *TFigureTriangle() {
  TFigure_type *t1=TFigureCreate(0.4,300,87,150,0,THIRD_PI,80,40,"Triangle");
  t1->rootnode=TLinenodeCreateXY(0,0,-50,87,0,0,50,87,-THIRD_PI,NULL);
  TLinenodeAdd(t1->rootnode,0,87,-50,87,0,87,50,87,PI);
  
  return t1;
}

TFigure_type *TFigureBrick() {
  TFigure_type *t1=TFigureCreate(0.4,80,80,40,0,TWO_PI,40,40,"Brick");
  t1->rootnode=TLinenodeCreateXY(0,0,0,80,80,0,80,80,0,NULL);
  TLinenodeAdd(t1->rootnode,0,0,40,0,40,80,80,80,0);
  TLinenodeAdd(t1->rootnode,40,0,80,0,0,80,40,80,0);
  return t1;
}

TFigure_type *TFigureSquareN8() {
  TFigure_type *t1=TFigureCreate(0.5,86,43,43,0,TWO_PI,40,70,"SquareN8");

  t1->rootnode=TLinenodeCreateXY(0,0,25,0,43,43,68,43,0,NULL);
  TLinenodeAdd(t1->rootnode,0,0,0,25,86,0,86,25,0);
  TLinenodeAdd(t1->rootnode,0,25,25,25,43,-18,68,-18,0);
  TLinenodeAdd(t1->rootnode,25,0,25,25,25,0,25,25,0);
  
  TLinenodeAdd(t1->rootnode,25,0,  42,-17,68,43,  86,25,0);
  TLinenodeAdd(t1->rootnode,25,25, 43,43, 68,-18, 86,0,0);
  
  return t1;
}
