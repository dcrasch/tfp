/*
 * Tesselation 
 *
 * Copyright (c) 2001, David Rasch <drasch@xs4all.nl>
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

#include "tesselation.h"
#include "tfrecord.h"
#include "tesselation_main.h"

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

UInt16 currentFigure = 0;
RectangleType drawRect = { {0, 0}, {160, 160} };

/*
 * Get preferences, open (or create) app database 
 */
static Err StartApplication(void)
{
  // ReadPrefs();
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
  // WritePrefs();
  FrmSaveAllForms();
  FrmCloseAllForms();

}

void AppLoadForm(UInt16 wFormID)
{
  FormPtr frm = FrmInitForm(wFormID);
  FrmSetActiveForm(frm);

  // Set the event handler for the form.
  // The handler of the currently active form is called by
  // FrmHandleEvent each time it receives an event.
  switch (wFormID) {
  case formMain:
    {
      FrmSetEventHandler(frm, MainFormEventHandler);
      break;
    }
  case formEdit:
    {
      FrmSetEventHandler(frm, EditFormEventHandler);
      break;
    }
  default:
    {
      break;
    }
  }
}

Boolean AppEventHandler(EventPtr event)
{
  Boolean bHandled = false;

  switch (event->eType) {
  case frmLoadEvent:
    {
      // Load the form resource.

      AppLoadForm(event->data.frmLoad.formID);
      bHandled = true;
      break;
    }
  default:
    {
      bHandled = false;
      break;
    }
  }
  return bHandled;
}

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
  Err err;

  if (cmd == sysAppLaunchCmdNormalLaunch) {
    err = StartApplication();
    if (err)
      return err;

    AppEventLoop();
    StopApplication();
  }
  else if (cmd == sysAppLaunchCmdExgReceiveData) {
    DmOpenRef dbP;
    
    // if our app is not active, we need to open the database
    // The subcall flag is used to determine whether we are active
    if (launchFlags & sysAppLaunchFlagSubCall) {
      dbP = gCustomerDB;
      
      // save any data we may be editing.
      FrmSaveAllForms();

      error = ReceiveSentData(dbP, (ExgSocketPtr) cmdPBP);
    } else {
      dbP = DmOpenDatabaseByTypeCreator(kCustomerDBType, kSalesCreator, 
        dmModeReadWrite);
      if (dbP) {
        error = ReceiveSentData(dbP, (ExgSocketPtr) cmdPBP);
      
        DmCloseDatabase(dbP);
      }
    }
  }
}

void AppEventLoop(void)
{
  EventType event;

  do {
    EvtGetEvent(&event, evtWaitForever);

    // Ask system to handle event.
    if (false == SysHandleEvent(&event)) {
      // System did not handle event.

      UInt16 error;
      // Ask Menu to handle event.
      if (false == MenuHandleEvent(0, &event, &error)) {
	// Menu did not handle event.
	// Ask App (that is, this) to handle event.
	if (false == AppEventHandler(&event)) {
	  // App did not handle event.
	  // Send event to appropriate form.
	  FrmDispatchEvent(&event);
	}			// end if (false == AppEventHandler
	// (&event))
      }				// end if (false == MenuHandleEvent (0,
      // &event, &error))
    }				// end if (false == SysHandleEvent
    // (&event))
  }
  while (event.eType != appStopEvent);
}
