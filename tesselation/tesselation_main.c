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

#include "tesselation.h"
#include "tesselation_main.h"
#include "tesselation_add.h"
#include "tfrecord.h"
#include "tesselation_edit.h"
#include "tesselation_rename.h"
#include "tesselation_util.h"

static void MainFormInit(FormPtr formP);
static Boolean MainFormMenuHandler(EventPtr e);
static Boolean MainFormButtonHandler(FormPtr formP, EventPtr e);
static Boolean MainFormMenuHandler(EventPtr e);
static void FigureListFill(FormPtr frm);
static void FigureListDrawFunc(UInt16 itemNum, RectanglePtr bounds,
			       Char * data[]);

static Boolean DeleteFigure(void);
static void BeamCurrentFigure(void);
static void FigureListSetSelection(FormPtr frm, Int16 selection);
static Boolean MainFormKeyHandler(FormPtr frm, EventPtr e);
static void FigureListScroll(FormPtr frm, WinDirectionType direction);

Boolean MainFormEventHandler(EventPtr event)
{
    Boolean handled = false;

    switch (event->eType) {
    case frmOpenEvent:
	{
	    FormPtr frm = FrmGetActiveForm();
	    FrmDrawForm(frm);
	    MainFormInit(frm);
	    handled = true;
	    break;
	}
    case menuEvent:
	handled = MainFormMenuHandler(event);
	break;
    case ctlSelectEvent:
	{
	    FormPtr frm = FrmGetActiveForm();
	    handled = MainFormButtonHandler(frm, event);
	    break;
	}
    case lstSelectEvent:
	currentFigure = event->data.lstSelect.selection;
	handled = true;
    case keyDownEvent:
	{
	    FormPtr frm = FrmGetActiveForm();
	    handled = MainFormKeyHandler(frm, event);
	    break;
	}
    default:
	break;
    }
    return handled;
}

static Boolean MainFormKeyHandler(FormPtr frm, EventPtr e)
{
    Boolean handled = false;
    switch (e->data.keyDown.chr) {
    case vchrPageUp:
    case vchrJogUp:
    case vchrJogPushedUp:
	FigureListScroll(frm, winUp);
	handled = true;
	break;
    case vchrPageDown:
    case vchrJogDown:
    case vchrJogPushedDown:
	FigureListScroll(frm, winDown);
	handled = true;
	break;
    default:
	break;
    }
    return handled;
}
static Boolean MainFormMenuHandler(EventPtr e)
{
    Boolean handled = false;
    switch (e->data.menu.itemID) {
    case menuItemRename:
	if (DoRenameFigure()) {
	    FormPtr frm = FrmGetActiveForm();
	    FigureListFill(frm);
	    FrmDrawForm(frm);
	}
	handled = true;
	break;
    case menuItemDelete:
	if (DeleteFigure()) {
	    FormPtr frm = FrmGetActiveForm();
	    FigureListFill(frm);
	    FrmDrawForm(frm);
	}
	handled = true;
	break;
    case menuItemBeam:
	BeamCurrentFigure();
	handled = true;
	break;

    case menuItemThanks:
	FrmAlert(alertID_thanks);
	handled = true;
	break;
    case menuItemAbout:
	FrmAlert(alertID_about);
	handled = true;
	break;
    case menuItemLicense:
	FrmAlert(alertID_license);
	handled = true;
	break;
    default:
	handled = false;
	break;
    }
    return handled;
}

static Boolean MainFormButtonHandler(FormPtr frm, EventPtr event)
{
    Boolean handled = false;
    switch (event->data.ctlEnter.controlID) {
    case buttonNew:
	if (DoAddFigure()) {
	    FigureListFill(frm);
	    currentFigure = 0;
	    FigureListSetSelection(frm, currentFigure);
	}
	handled = true;
	break;
    case buttonEdit:
	ErrFatalDisplayIf(!CheckROMVerGreaterThan(3, 5),
			  "You need Palm OS >= 3.5! Sorry Fred.");

	if ((currentFigure != noListSelection)
	    && (TFigurerecordGetCount() > 0)) {
	    FrmGotoForm(formEdit);
	}
	handled = true;
	break;
    default:
	handled = false;
	break;
    }
    return handled;
}

void MainFormInit(FormPtr frm)
{
    Int16 tmpFigure = currentFigure;
    FigureListFill(frm);
    if (tmpFigure != noListSelection)
	FigureListSetSelection(frm, tmpFigure);

}

static void FigureListSetSelection(FormPtr frm, Int16 selection)
{
    ListPtr list = (ListPtr) FrmGetObjectPtr(frm,
					     FrmGetObjectIndex(frm,
							       listFigure));

    LstSetSelection(list, selection);
}

static void FigureListScroll(FormPtr frm, WinDirectionType direction)
{
    ListPtr list = (ListPtr) FrmGetObjectPtr(frm,
					     FrmGetObjectIndex(frm,
							       listFigure));
    LstScrollList(list, direction, 10);
}

static void FigureListFill(FormPtr frm)
{
    int numFigures = TFigurerecordGetCount();
    ListPtr list = (ListPtr) FrmGetObjectPtr(frm,
					     FrmGetObjectIndex(frm,
							       listFigure));

    LstSetListChoices(list, NULL, numFigures);
    LstSetDrawFunction(list, (ListDrawDataFuncPtr) FigureListDrawFunc);
    LstDrawList(list);
    currentFigure = LstGetSelection(list);
    LstSetSelection(list, currentFigure);
}

static void
FigureListDrawFunc(UInt16 itemNum, RectanglePtr bounds, Char * data[])
{
    char currFigure[DESCSIZE];

    TFigurerecordGetName(itemNum, currFigure);
    WinDrawChars(currFigure, StrLen(currFigure), bounds->topLeft.x + 6,
		 bounds->topLeft.y);
}

static Boolean DeleteFigure(void)
{
    UInt16 choice;
    int numFigures = TFigurerecordGetCount();
    if ((currentFigure != noListSelection) && (currentFigure < numFigures)) {
	char currFigure[DESCSIZE];
	TFigurerecordGetName(currentFigure, currFigure);
	choice = FrmCustomAlert(alertID_delete, currFigure, NULL, NULL);
	if (choice == 0) {
	    TFigurerecordDelete(currentFigure);
	    return true;
	}
    }
    return false;
}

static void BeamCurrentFigure(void)
{
    int numFigures = TFigurerecordGetCount();
    if ((currentFigure != noListSelection) && (currentFigure < numFigures)) {
	TFigurerecordBeam(currentFigure);
    }
}
