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
/*! \file tfill.h
*/
#ifndef __TFILL_H__
#define __TFILL_H__

#include <PalmOS.h>
#include "tfigure.h"

/*!
  \brief fills the space of the figure with a color
  \param t1 figure
  \param mode mode of fill grayscale or color
*/
void TFigureFill(TFigure_type * t1, int mode);

#endif
