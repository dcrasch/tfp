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

#include <PalmOS.h>
#include <PalmCompatibility.h>
#include <ExgMgr.h>

#include "tesselation.h"
#include "tfrecord.h"
#include "tesselation_main.h"
#include "tesselation_util.h"

/*
 * function prototypes 
 */
static Boolean AppEventHandler(EventPtr pEvent);
static void AppEventLoop(void);
static void AppLoadForm(UInt16 wFormID);
static Err StartApplication(void);
static void StopApplication(void);

/*
 * Tesselation globals 
 */

Int16 currentFigure = noListSelection;
RectangleType drawRect = { {0, 0}, {160, 160} };

/*
 * Get preferences, open (or create) app database 
 */
static Err StartApplication(void)
{
    TFigureOpen();
    FrmGotoForm(formMain);
    return 0;
}

/*
 * Save preferences, close forms, close app database 
 */
static void StopApplication(void)
{
    TFigureClose();
    FrmSaveAllForms();
    FrmCloseAllForms();

}

void AppLoadForm(UInt16 wFormID)
{
    FormPtr frm = FrmInitForm(wFormID);
    FrmSetActiveForm(frm);

    /* Set the event handler for the form.
     * The handler of the currently active form is called by
     * FrmHandleEvent each time it receives an event.
     */


    switch (wFormID) {
    case formMain:
	FrmSetEventHandler(frm, MainFormEventHandler);
	break;

    case formEdit:
	FrmSetEventHandler(frm, EditFormEventHandler);
	break;

    default:
	break;
    }

}

Boolean AppEventHandler(EventPtr event)
{
    Boolean bHandled = false;

    switch (event->eType) {
    case frmLoadEvent:
	// Load the form resource.

	AppLoadForm(event->data.frmLoad.formID);
	bHandled = true;
	break;
    default:
	bHandled = false;
	break;
    }
    return bHandled;
}

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
    Err err;

    switch (cmd) {
    case sysAppLaunchCmdNormalLaunch:
	if (CheckROMVerGreaterThan(3, 5))
	    ExgRegisterData(appCreator, exgRegExtensionID,
			    appExgRegisterDataType);

	err = StartApplication();
	if (err)
	    return err;
	AppEventLoop();
	StopApplication();
	break;
    case sysAppLaunchCmdSyncNotify:
	if (CheckROMVerGreaterThan(3, 5))
	    ExgRegisterData(appCreator, exgRegExtensionID,
			    appExgRegisterDataType);
	break;
    case sysAppLaunchCmdExgReceiveData:
	err = TFigureReceive((ExgSocketPtr) cmdPBP,
			     (launchFlags & sysAppLaunchFlagSubCall));

	break;
    default:
	break;
    }
    return errNone;
}

void AppEventLoop(void)
{
    EventType event;

    do {
	EvtGetEvent(&event, evtWaitForever);
	if (false == SysHandleEvent(&event)) {
	    UInt16 error;
	    if (false == MenuHandleEvent(0, &event, &error)) {
		if (false == AppEventHandler(&event)) {
		    FrmDispatchEvent(&event);
		}
	    }
	}
    }
    while (event.eType != appStopEvent);
}
