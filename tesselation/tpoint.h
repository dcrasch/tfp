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


/*! \file tpoint.h
*/

#ifndef __TPOINT_H__
#define __TPOINT_H__

#include <PalmOS.h>

/*!
  \brief size of the square around the point
*/
#define POINTSIZE 4

/*!
  \brief point structure
*/
typedef struct tpoint_tag {
  double x; //!< x coordinate
  double y; //!< y coordinate
} TPoint_type;

/*!
  \brief Draw a single square
  \param p1 point
*/
void TPointDraw(TPoint_type p1);

/*!
  \brief Draw a line between two points
  \param p1 point
  \param p2 point
*/
void TPointDrawLine(TPoint_type p1, TPoint_type p2);

/*!
  \brief add x and y to p1
  \param p1 point
  \param x x coordinate
  \param y y coordinate
  \return a new point with x and y added to p1
*/
TPoint_type TPointAddXY(TPoint_type p1, double x, double y);

/*!
  \brief add p1 to p2
  \param p1 point
  \param p2 point
  \return the addition of p1 and p2
*/
TPoint_type TPointAddPoint(TPoint_type p1, TPoint_type p2);

/*!
  \brief the substract of p1 and p2
  \param p1 point
  \param p2 point
  \return the substraction of p1 and p2
*/
TPoint_type TPointMinusPoint(TPoint_type p1, TPoint_type p2);

/*!
  \brief multiply p1 by lambda
  \param p1 point
  \param lambda multiplication value
  \return the multiplication of p1 by lambda
*/
TPoint_type TPointMul(TPoint_type p1, double lambda);

/*!
  \brief distance of p1 from 0

  \f$\sqrt{(p1.x+p1.y)^2}\f$
  \param p1 point
  \return the distance of p1 from 0
*/
double TPointRadius(TPoint_type p1);

/*!
  \brief distance of p1 from the line p2,p3
  \param p1 point
  \param p2 first point of the line
  \param p3 second point of the line
  \return the distance of p1 from p2,p3
*/
double TPointDistanceFromLine(TPoint_type p1, TPoint_type p2,
			      TPoint_type p3);
/*!
  \brief the distance of p1 from p2
  \param p1 point
  \param p2 point
  \return the distance of p1 from p2
*/
double TPointDistanceFromPoint(TPoint_type p1, TPoint_type p2);

/*!
  \brief the distance of p1 from (x,y)
  \param p1 point
  \param x coordinate
  \param y coordinate
  \return the distance of p1 from (x,y)
*/
double TPointDistanceFromXY(TPoint_type p1, double x, double y);

/*!
  \brief Calculates if a point (hx,hy) is is inside a square of ps size
  \param p1 middle point of square
  \param hx x coordinate
  \param hy y coordinate
  \param ps width and height of square / 2 + 1
  \return if the point
*/
Boolean TPointHitXY(TPoint_type p1, double hx, double hy, double ps);

/*!
  \brief size of the square around the point
*/
#define sqr(x) (x*x)

#endif
