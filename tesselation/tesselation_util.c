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
