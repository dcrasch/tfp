#ifndef __TINYTRIG_H__
#define __TINYTRIG_H__ 
/********************************************************************
* FILE:				TinyTrig.h
*
* DESCRIPTION:	Header file for Tiny Trig functions library.
*
* VERSION:		1.0
**********************************************************************/
// NOTE: CHANGE THESE THREE VALUES TO CHANGE THE LIBRARY ACCURACY
// The inverse trig functions accuracy cannot be improved because
// they use table lookup methods, not approximation equations.

#define		TT_MIN_VALUE				1.0e-6
#define		TT_MAX_VALUE				1.0e+6
#define		TT_ACCURACY					1.0e-6
#define		POS_INFINITY					1.2e+38	// Float max
#define		NEG_INFINITY					3.4e-38	// Float min

// Plain old numeric constants and conversion factors

#define 		PI								3.14159265358979311600e+00
#define		TWO_PI						2.0 * PI
#define		HALF_PI						PI / 2.0
#define         THIRD_PI                                        PI / 3.0

// External function prototypes

double 		_sqrt ( double x );			// Square root of x
double		_abs  ( double x );			// absolute value

double 		_sin    ( double x );			// Sine of x using power series
double 		_cos   ( double x );			// Cosine of x	


     
  



#endif
