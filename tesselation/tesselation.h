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
/*! \file tesselation.h
 */
#ifndef __TESSELATION_H__
#define __TESSELATION_H__

#include "tfigure.h"
#include "tesselation_resource.h"
/*!
  \brief application id 
*/
#define appCreator             'TeAp'
/*!
  \brief creator id
*/
#define appFileCreator         'DCRP'
/*!
  \brief beam figure filename
*/
#define appExgName             "tesselation.cst"
/*!
  \brief extension description
*/
#define appExgDescription      "Tesselation figure"
/*!
  \brief standard beam extension
*/
#define appExgRegisterDataType "cst"

/*!
\brief database name
*/
#define appDatabaseName        "FigureDB"

/*!
  \brief database type
*/
#define appDatabaseType        'Data'

// eventhandlers

// mainform

// extern TFigure_type *currentFigure;
/*!
  \brief event handler of main form
  \param e event
  \return boolean if event is handled ok
*/
extern Boolean MainFormEventHandler(EventPtr e);

/*!
  \brief event handler of edit form
  \param e event
  \return boolean if event is handled ok
*/
extern Boolean EditFormEventHandler(EventPtr e);

/*!
  \brief current edit figure
*/
extern Int16 currentFigure;

/*!
  \brief drawable area
*/
extern RectangleType drawRect;

#endif
