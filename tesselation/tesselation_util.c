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
#include <PalmTypes.h>

/*
 * Check to see that ROM version is greater or equal to ver 
 */
Boolean CheckROMVerGreaterThan(UInt16 major, UInt16 minor)
{
    UInt32 romVersion;
    UInt16 maj;
    UInt16 min;

    FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
    maj = sysGetROMVerMajor(romVersion);
    min = sysGetROMVerMinor(romVersion);
    if (maj > major)
	return true;
    else if ((maj == major) && (min >= minor))
	return true;

    return false;
}
