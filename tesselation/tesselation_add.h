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
/*! \file tesselation_add.h
 */
#ifndef __TESSELATION_ADD_H__
#define __TESSELATION_ADD_H__

#include <PalmOS.h>
#include "TinyTrig.h"

/*!
  \brief add new figure dialog
*/
extern Boolean DoAddFigure();

/*!
  \brief square figure 
  \return figure
*/
TFigure_type *TFigureSquare();
/*!
  \brief square2 figure 
  \return figure
*/
TFigure_type *TFigureSquare2();
/*!
  \brief square90 figure 
  \return figure
*/
TFigure_type *TFigureSquare90();
/*!
  \brief squaren8 figure 
  \return figure
*/
TFigure_type *TFigureSquareN8();
/*!
  \brief brick figure 
  \return figure
*/
TFigure_type *TFigureBrick();
/*!
  \brief triangle figure 
  \return figure
*/
TFigure_type *TFigureTriangle();
/*!
  \brief diamond figure
  \return figure
*/
TFigure_type *TFigureDiamond();

#endif
