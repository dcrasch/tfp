#include "tesselation.h"
#include "tfigure.h"
#include "tfrecord.h"
#include "tesselation_add.h"

static Boolean theMouseDown = false;
static int tesselateMode = false;
static TFigure_type *my_figure = NULL;

static Boolean EditFormMenuHandler(EventPtr event);
static Boolean EditFormScreenHandler(EventPtr event);
static void EditFormInit(FormPtr frm);
static void EditFormDone();

static Boolean removePoint();

Boolean EditFormEventHandler(EventPtr event)
{
    Boolean handled = false;

    switch (event->eType) {
    case frmOpenEvent:
	{
	    FormPtr frm = FrmGetActiveForm();
	    FrmDrawForm(frm);
	    EditFormInit(frm);

	    handled = true;
	    break;
	}
    case frmCloseEvent:
	{
	    if (my_figure) {
		TFigureFree(my_figure);
		my_figure = NULL;
	    }
	    handled = true;
	    break;
	}
    case menuEvent:
	{
	    handled = EditFormMenuHandler(event);
	    break;
	}
    default:
	{
	    break;
	}
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
		if (TFigureMouseDown
		    (my_figure, event->screenX, event->screenY))
		    TFigureRedraw(my_figure);
		handled = true;
		theMouseDown = true;
	    }
	    break;
	case penMoveEvent:
	    if (theMouseDown) {
		if (TFigureMouseDrag
		    (my_figure, event->screenX, event->screenY))
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
    my_figure = TFigurerecordGet(currentFigure);
    ErrFatalDisplayIf(!my_figure, "Could not get record");
    theMouseDown = false;
    tesselateMode = false;
    TFigureRedraw(my_figure);
}

static void EditFormDone()
{
    TFigurerecordChange(currentFigure, my_figure);
    FrmGotoForm(formMain);
}

static Boolean EditFormMenuHandler(EventPtr event)
{
    Boolean handled = false;
    switch (event->data.menu.itemID) {
    case menuItemTesselate:
	{
	    tesselateMode = true;
	    if (my_figure)
		TFigureTesselate(my_figure);
	    handled = true;
	    break;
    case menuItemEdit:
	    {
		tesselateMode = false;
		if (my_figure)
		    TFigureRedraw(my_figure);
		handled = true;
		break;
	    }
    case menuItemRemove:
	    {
		if (removePoint()) {
		    TFigureRedraw(my_figure);
		}
		handled = true;
		break;
	    }
    case menuItemEDone:
	    {
		handled = true;
		EditFormDone();
		break;
	    }

    default:
	    handled = false;
	    break;
	}
    }
    return handled;
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
