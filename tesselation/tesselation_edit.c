#include "tesselation.h"
#include "tfigure.h"
#include "tfrecord.h"
#include "tesselation_add.h"
#include "tesselation_edit.h"

static Boolean theMouseDown = false;
static int tesselateMode = false;
static TFigure_type *my_figure = NULL;

static Boolean EditFormMenuHandler(EventPtr event);
static Boolean EditFormScreenHandler(EventPtr event);
static void EditFormInit(FormPtr frm);
static void EditFormDone();
static void EditFormCleanUp();

static Boolean removePoint();
static void setTesselate(Boolean m);
static Boolean zoomPoint(double zoom);

Boolean EditFormEventHandler(EventPtr event)
{
  Boolean handled = false;
  FormPtr frm;
  switch (event->eType) {
  case frmOpenEvent:
    frm = FrmGetActiveForm();
    FrmDrawForm(frm);
    EditFormInit(frm);
    handled = true;
    break;
  case frmCloseEvent:
    EditFormCleanUp();
    break;

  case menuEvent:

    handled = EditFormMenuHandler(event);
    break;
  default:
    break;
  }
  if (!handled)
    handled = EditFormScreenHandler(event);
  return handled;
}

static Boolean EditFormScreenHandler(EventPtr event)
{
  Boolean handled = false;
  if ((my_figure) &&
      (RctPtInRectangle(event->screenX, event->screenY, &drawRect))) {
    switch (event->eType) {
    case penDownEvent:
      if (!theMouseDown) {
	if (TFigureMouseDown(my_figure, event->screenX, event->screenY)) {
	  TFigureRedraw(my_figure);
	}
	handled = true;
	theMouseDown = true;
      }
      break;
    case penMoveEvent:
      if (theMouseDown) {
	if (TFigureMouseDrag(my_figure, event->screenX, event->screenY))
	  TFigureRedraw(my_figure);
      }
      break;
    case penUpEvent:
      if (theMouseDown) {
	TFigureMouseUp(my_figure);
	theMouseDown = false;
      }
    default:
      break;
    }
  }
  return handled;
}

static void EditFormInit(FormPtr frm)
{
  if (my_figure) {
    TFigureFree(my_figure);
  }
  my_figure = TFigurerecordGet(currentFigure);

  ErrFatalDisplayIf(!my_figure, "Could not open figure");

  theMouseDown = false;
  setTesselate(false);
  TFigureFit(my_figure);

  if (my_figure) {
    TFigureRedraw(my_figure);
  }
}

static void EditFormDone()
{
  if (my_figure)
    TFigurerecordChange(currentFigure, my_figure);
  FrmGotoForm(formMain);
}

static void setTesselate(Boolean m)
{
  FormPtr frm = FrmGetActiveForm();
  tesselateMode = m;

  if (tesselateMode) {
    FrmSetMenu(frm, menuTessellate);
  } else {
    FrmSetMenu(frm, menuEdit);
  }
}

static Boolean EditFormMenuHandler(EventPtr event)
{
  Boolean handled = false;

  switch (event->data.menu.itemID) {

  case menuItemTesselateBW:
    setTesselate(true);
    if (my_figure)

      TFigureTesselate(my_figure, 0);
    handled = true;
    break;

  case menuItemTesselateGray:
    setTesselate(true);
    if (my_figure)
      TFigureTesselate(my_figure, 1);
    handled = true;
    break;

  case menuItemTesselate:
    setTesselate(true);
    if (my_figure)
      TFigureTesselate(my_figure, 2);
    handled = true;
    break;

  case menuItemEdit:

    setTesselate(false);
    if (my_figure)
      TFigureRedraw(my_figure);
    handled = true;
    break;

  case menuItemRemove:

    if (removePoint()) {
      TFigureRedraw(my_figure);
    }
    handled = true;
    break;

  case menuItemFit:
    if (my_figure) {
      TFigureFit(my_figure);
      TFigureRedraw(my_figure);
    }
    handled = true;
    break;

  case menuItemZoomIn:
    if (zoomPoint(1.6)) {
      TFigureRedraw(my_figure);
    }
    handled = true;
    break;
  case menuItemZoomOut:
    if (zoomPoint(0.625)) {
      TFigureRedraw(my_figure);
    }
    handled = true;
    break;
  case menuItemEDone:

    handled = true;
    EditFormDone();
    break;

  default:
    handled = false;
    break;

  }
  return handled;
}

// clean up the form
void EditFormCleanUp()
{
  if (my_figure) {
    TFigureFree(my_figure);
    my_figure = NULL;
  }
}

Boolean removePoint()
{
  UInt16 choice;
  if (my_figure->sellast) {
    choice = FrmAlert(alertID_remove);
    if (choice == 0) {
      TFigureRemovePoint(my_figure);
      return true;
    }
  } else {
    choice = FrmAlert(alertID_selection);
  }
  return false;
}

Boolean zoomPoint(double zoom)
{
  if (my_figure->sellast) {
    TFigureZoom(my_figure, zoom);
    return true;
  } else {
    int choice;
    choice = FrmAlert(alertID_selection);
  }
  return false;
}
