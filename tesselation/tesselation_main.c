#include "tesselation.h"
#include "tesselation_main.h"
#include "tesselation_add.h"
#include "tfrecord.h"
#include "tesselation_util.h"
#include "tesselation_edit.h"
#include "tesselation_rename.h"

static void MainFormInit(FormPtr formP);
static Boolean MainFormMenuHandler(EventPtr e);
static Boolean MainFormButtonHandler(FormPtr formP, EventPtr e);
static Boolean MainFormMenuHandler(EventPtr e);
static void FigureListFill(FormPtr frm);
static void FigureListDrawFunc(UInt16 itemNum, RectanglePtr bounds,
			       Char * data[]);

static Boolean DeleteFigure(void);
static BeamCurrentFigure(void);

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
    {
      handled = MainFormMenuHandler(event);
      break;
    }
  case ctlSelectEvent:
    {
      FormPtr frm = FrmGetActiveForm();
      handled = MainFormButtonHandler(frm, event);
      break;
    }
  case lstSelectEvent:
    {
      currentFigure = event->data.lstSelect.selection;
      handled = true;
    }

  default:
    {
      break;
    }
  }
  return handled;
}

static Boolean MainFormMenuHandler(EventPtr e)
{
  Boolean handled = false;
  switch (e->data.menu.itemID) {
  case menuItemBeam:
    BeamCurrentFigure();
    handled=true;
    break;
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
  FigureListFill(frm);
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

static BeamCurrentFigure(void) {
   UInt16 choice;
   int numFigures = TFigurerecordGetCount();
   if ((currentFigure != noListSelection) && (currentFigure < numFigures)) {
     TFigurerecordBeam(currentFigure);
}
