#include "tfill.h"

/* 
   Author: andrew@drawit.co.nz
   www   : http://www.drawit.co.nz/
*/

static Boolean CheckIt(int x, int y)
{
    // inside figure rectangle 
    // stupid function returns zero if outside window!
    if ((x < 0) || (x > 159) || (y < 0) || (y > 159))
	return false;

    return (WinGetPixel(x, y) == 0);
}

/* A diamond flood-fill using a circular queue system.
 Each pixel surrounding the current pixel is added to
 the queue if it meets the criteria, then is retrieved in
 its turn. In theory, the most pixels in the queue should
 be 640 on the pilot screen */

static void FloodFill(int xx, int yy, UInt8 Col)
{
    if (CheckIt(xx, yy)) {
	//prepare memory for queue
	MemHandle qh;		//queue handle
	UInt8 *qs, *qst, *qr;	//queue save, start, read
	UInt32 qSz = 640 * 2 * sizeof(UInt8);	//queue size (physical)
	UInt32 qpSz = 640 * 2;	//queue size (pointer)
	int xt, yt;		//temporary x and y locations

	qh = MemHandleNew(qSz);
	if (!qh) {
	    return;
	}
	qst = (UInt8 *) MemHandleLock(qh);
	MemSet(qst, qSz, 0);	//Clear the contents
	qs = qr = qst;
	*qs = xt = xx;
	qs++;
	*qs = yt = yy;
	qs++;

	WinSetForeColor(Col);
	WinDrawPixel(xt, yt);

	//Main queue loop
	while (qr != qs) {
	    //Add new members to queue
	    //Above current pixel
	    if (CheckIt(xt, yt - 1)) {
		*qs = xt;
		qs++;
		*qs = yt - 1;
		qs++;
		WinDrawPixel(xt, yt - 1);
		if (qs >= (qst + qpSz))
		    qs = qst;	//Loop back to beginning of queue
	    }
	    //Below current pixel
	    if (CheckIt(xt, yt + 1)) {
		*qs = xt;
		qs++;
		*qs = yt + 1;
		qs++;
		WinDrawPixel(xt, yt + 1);
		if (qs >= (qst + qpSz)) {
		    qs = qst;
		}
	    }
	    //Left of current pixel
	    if (CheckIt(xt - 1, yt)) {
		*qs = xt - 1;
		qs++;
		*qs = yt;
		qs++;
		WinDrawPixel(xt - 1, yt);
		if (qs >= (qst + qpSz)) {
		    qs = qst;
		}
	    }
	    //Right of current pixel
	    if (CheckIt(xt + 1, yt)) {
		*qs = xt + 1;
		qs++;
		*qs = yt;
		qs++;
		WinDrawPixel(xt + 1, yt);
		if (qs >= (qst + qpSz))
		    qs = qst;
	    }
	    //Retrieve current queue member
	    qr += 2;
	    if (qr >= (qst + qpSz)) {
		qr = qst;	//Loop back to the beginning
	    }
	    xt = *qr;
	    yt = *(qr + 1);
	}			//Back to beginning of loop

	//Free the memory
	MemHandleUnlock(qh);
	MemHandleFree(qh);
    }
}


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

void TFigureFill(TFigure_type * t1, int mode)
{
    int x, y;
    UInt32 depth;
    UInt8 c = 1;
    WinScreenMode(winScreenModeGet, 0, 0, &depth, 0);

    if ((mode != 0) && (depth > 1)) {
	for (y = 0; y < 160; y++) {
	    for (x = 0; x < 160; x++) {

		if (mode == 1) {
		    if (depth >= 8) {
			c = SysRandom(0) % 11 + 215;
		    }
		    if (depth == 4) {
			c = SysRandom(0) % 14 + 1;
		    }
		    if (depth == 2) {
			c = SysRandom(0) % 2 + 1;
		    }
		}

		if (mode == 2) {
		    c = SysRandom(0) % 200 + 1;
		}
		FloodFill(x, y, c);
	    }
	}
    }
}
