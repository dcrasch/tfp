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

#ifndef __TESSELATION_UTIL_H__
#define __TESSELATION_UTIL_H__

Boolean CheckROMVerGreaterThan(UInt16 major, UInt16 minor);

#ifndef vchrJogUp
#  define vchrJogUp             (0x1700)
#  define vchrJogDown           (0x1701)
#  define vchrJogPushedUp       (0x1703)
#  define vchrJogPushedDown     (0x1704)
#endif
#ifndef vchrJogDown
#  define vchrJogDown           (0x1701)
#endif


#endif
