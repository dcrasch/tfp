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

#ifndef __TESSELATION_H__
#define __TESSELATION_H__

#include "tfigure.h"
#include "tesselation_resource.h"

#define appCreator             'TeAp'
#define appFileCreator         'DCRP'
#define appExgName             "tesselation.cst"
#define appExgDescription      "Tesselation figure"
#define appExgRegisterDataType "cst"
#define appDatabaseName        "FigureDB"
#define appDatabaseType        'Data'

// eventhandlers

// mainform

// extern TFigure_type *currentFigure;
extern Boolean MainFormEventHandler(EventPtr e);
extern Boolean EditFormEventHandler(EventPtr e);

extern Int16 currentFigure;
extern RectangleType drawRect;

#endif
