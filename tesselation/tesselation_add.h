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

#ifndef __TESSELATION_ADD_H__
#define __TESSELATION_ADD_H__

#include <PalmOS.h>
#include "TinyTrig.h"
extern Boolean DoAddFigure();

TFigure_type *TFigureSquare();
TFigure_type *TFigureSquare2();
TFigure_type *TFigureSquare90();
TFigure_type *TFigureSquareN8();
TFigure_type *TFigureBrick();
TFigure_type *TFigureTriangle();
TFigure_type *TFigureDiamond();

#endif
