#include "tesselation.h"
#include "tesselation_rename.h"
#include "tfrecord.h"
#include "tfigure.h"

Boolean DoRenameFigure(void)
{
    FormPtr prevFrm = FrmGetActiveForm();
    int numFigures = TFigurerecordGetCount();
    if ((currentFigure != noListSelection) && (currentFigure < numFigures)) {
	TFigure_type *t1 = TFigurerecordGet(currentFigure);
	if (t1) {
	    FormPtr frm = FrmInitForm(formRenameFigure);
	    FieldPtr field = (FieldPtr) FrmGetObjectPtr(frm,
							FrmGetObjectIndex
							(frm,
							 fieldFigureName));
	    MemHandle hText = MemHandleNew(DESCSIZE);
	    char *titleptr = (char *) MemHandleLock(hText);
	    StrNCopy(titleptr, t1->name, DESCSIZE);
	    titleptr[DESCSIZE - 1] = 0;

	    MemPtrUnlock(titleptr);
	    FldSetTextHandle(field, (MemHandle) hText);

	    if (FrmDoDialog(frm) == buttonRFOK) {
		hText = FldGetTextHandle(field);
		titleptr = (char *) MemHandleLock(hText);
		if ((titleptr) && StrLen(titleptr) > 0) {

		    StrNCopy(t1->name, titleptr, DESCSIZE);
		    t1->name[DESCSIZE - 1] = 0;
		    TFigurerecordChange(currentFigure, t1);
		}
		MemPtrUnlock(titleptr);
	    }

	    TFigureFree(t1);
	    if (prevFrm)
		FrmSetActiveForm(prevFrm);
	    FrmDeleteForm(frm);
	}
	return true;
    }

    return false;
}
