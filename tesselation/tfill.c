#include "tfill.h"

// only for palm os 3.5
// can not fill the whole screen!
// use a stack!!

static int isWhite(int x, int y)
{
  // inside figure rectangle 
  // stupid function returns zero if outside window!
  if ((x < 0) || (x > 159) || (y < 0) || (y > 159))
    return 0;
  if (WinGetPixel(x, y) == 0)
    return 1;
  else
    return 0;
}

// scan left for the edge of the screen or index=0
static int scanleft(int x, int y)
{
  while (isWhite(x, y))
    x--;
  return x + 1;
}

static int scanright(int x, int y)
{
  while (isWhite(x, y))
    x++;
  return x - 1;
}

static void recfill(int x1, int x2, int y, int d)
{
  int x = x1;
  int lx, rx;
  int ny = y + d;
  WinDrawLine(x1, y, x2, y);
  while (x <= x2) {
    if (isWhite(x, ny)) {
      lx = scanleft(x, ny);
      rx = scanright(x, ny);
      recfill(lx, rx, ny, d);
      if (lx < x1)
	recfill(lx, x1 - 1, ny, -d);
      if (rx > x2)
	recfill(x2 + 1, rx, ny, -d);
      x = rx;
    }
    x++;
  }
}

/*
 * generic fill function void tfill(int x,int y) { if (isWhite(x,y)) { int 
 * lx=scanleft(x,y); int rx=scanright(x,y); recfill(lx,rx,y,-1);
 * recfill(lx,rx,y,1); } }
 * 
 */

void TFigureFill(TFigure_type * t1, int mode)
{
  int lx, rx, y;
  UInt32 depth;
  WinScreenMode(winScreenModeGet, 0, 0, &depth, 0);

  if ((mode != 0) && (depth > 1)) {
    for (y = 0; y < 160; y++) {
      rx = 0;
      while (rx < 160) {
	if (isWhite(rx, y)) {
	  lx = rx;
	  rx = scanright(rx, y);	// skip right 
	  if (mode == 1) {
	    if (depth >= 8) {
	      WinSetForeColor(SysRandom(0) % 11 + 215);
	    }
	    if (depth == 4) {
	      WinSetForeColor(SysRandom(0) % 14 + 1);
	    }
	    if (depth == 2) {
	      WinSetForeColor(SysRandom(0) % 2 + 1);
	    }
	  }

	  if (mode == 2) {
	    WinSetForeColor(SysRandom(0) % 200 + 1);
	  }
	  recfill(lx, rx, y, 1);
	}
	rx++;
      }
    }
  }
}
