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
/*! \file tesselation_util.h
 */

#ifndef __TESSELATION_UTIL_H__
#define __TESSELATION_UTIL_H__

/*!
  \brief check to see that ROM version is greate or equal to ver
  \param major major version number
  \param minor minor version number
*/

Boolean CheckROMVerGreaterThan(UInt16 major, UInt16 minor);

#ifndef vchrJogUp
/*!
  \brief jog up
*/
#  define vchrJogUp             (0x1700)
/*!
  \brief jog down
*/
#  define vchrJogDown           (0x1701)
/*!
  \brief jog pushed up
*/
#  define vchrJogPushedUp       (0x1703)
/*!
  \brief jog pushed down
*/
#  define vchrJogPushedDown     (0x1704)
#endif

#ifndef vchrJogDown
/*!
  \brief jog down
*/
#  define vchrJogDown           (0x1701)
#endif


#endif
