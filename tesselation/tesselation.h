#ifndef __TESSELATION_H__
#define __TESSELATION_H__

#include "tfigure.h"
#include "tesselation_resource.h"

#define appFileCreator         'DCRP'
#define appExgName             "_beam:tesselation.CST"
#define appExgDescription      "Tesselation figure"
#define appExgRegisterDataType "CST"
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
