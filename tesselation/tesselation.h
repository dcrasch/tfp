#ifndef __TESSELATION_H__
#define __TESSELATION_H__

#include "tfigure.h"
#include "tesselation_resource.h"

/*
 * Database name of Tesselation 
 */ 
// #define TSLN_NAME "Tesselation"
// #define TSLN_MIMETYPE "application/x-tesselation"
// #define TSLN_FILENAME "Tesselation.prc"

// eventhandlers

// mainform

// extern TFigure_type *currentFigure;
extern Boolean MainFormEventHandler(EventPtr e);
extern Boolean EditFormEventHandler(EventPtr e);

extern UInt16 currentFigure;
extern RectangleType drawRect;

#endif
