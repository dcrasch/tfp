/********************************************************************
* FILE:				TinyTrig.c
*
* DESCRIPTION:	Various modest-resolution trignometric functions.
*
* VERSION:		1.0
**********************************************************************/

#undef COUNTRY			// Because of OS 3.1 & OS 3.5 includes

#include <PalmOS.h>
#include <PalmCompatibility.h>
#include "TinyTrig.h"

/*
 * * Lookup tables for radians, sine, cosine, and tangent values. * The
 * values are stored as 32-bit integers, with a scaling of 1.oe+5, to *
 * reduce memory requirements and speed up computations. * There is
 * basically one entry per degree, between 0 and 90 degrees, but *
 * because the radians values are more sensitive at the beginning and end,
 * * there are extra values in all of the tables. These tables are used
 * strictly * for computing the inverse functions asin, acos, and atan.
 * There are * approximation equations for computing sin, cos, and tan
 * values. 
 */


/*****************************************************
 * FUNCTION:       ScaleNearZero
 *
 * DESCRIPTION:    Scale a radians number that is outside the 
 *		   range - scale <--> scale to something within that range.
 *		   Assume scale is either PI or 2PI
 *
 * RETURNED:       The scaled result.
*****************************************************/
static double ScaleNearZero(double x, double scale)
{
    double adjCntD, result;
    UInt32 adjCnt;

    // Get the number of scale increments in the input value.

    adjCntD = scale;
    adjCntD = x / adjCntD;
    adjCntD = _abs(adjCntD);
    adjCnt = (UInt32) adjCntD;

    // Adjust increment count for scale factor == PI

    if (scale == PI)
	adjCnt = adjCnt * 2;

    // Adjust the input value accordingly.

    if (x > 0.0)
	result = x - (scale * (double) adjCnt);
    else
	result = x + (scale * (double) adjCnt);

    return (result);
}

/***********************************************************************
 * FUNCTION:    	_abs
 *
 * DESCRIPTION: 	Calculate the absolute value of a double number.
 *
 * RETURNED:    	The result.
 ***********************************************************************/
double _abs(double x)
{
    if (x < 0.0)
	return (-x);
    else
	return (x);
}


/***********************************************************************
 * FUNCTION:    	_sqrt
 *
 * DESCRIPTION: 	Calculate the square root of a double number using
 *					successive approximations. The loop stops when the 
 *					difference between two successive iterations is smaller
 *					than the Tiny Trig accuracy threshold.
 *
 * RETURNED:    	The square root or zero if the number is smaller
 *					than the minimum value handled by the library.
 ***********************************************************************/
double _sqrt(double x)
{
    double result = 0.0, prevResult, diff = 0.0;

    // Eliminate negatives and small values outside the library range.  

    if (x >= TT_MIN_VALUE) {
	result = x;

	// Calculate until two successive guesses are less than
	// the library's minimum value. # iterations ranges from about
	// 10 (for very small and very large #s) to about 5 for numbers
	// close to zero.

	do {
	    prevResult = result;
	    result = (result + (x / result)) / 2.0;
	    diff = _abs(prevResult - result);
	}
	while (diff > TT_ACCURACY);
    }
    return result;
}


/*****************************************************
 * FUNCTION:    	_sin
 *
 * DESCRIPTION: 	Calculate the sine of a double radians number
 *			using  the power series sin x = x - 
 *		        ( x**3 / 3! ) + ( x**5 / 5! ) - ( x**7 / 7! ) + ...
 *
 * RETURNED:    	The result.
*****************************************************/
double _sin(double x)
{
    double result,
	prevResult, numerator, denominator, term, sign, factorial, diff;

    // Scale input angle to proper value between -2 PI and + 2 PI.
    // The power series is not as accurate outside that range.

    if ((x > TWO_PI) || (x < -TWO_PI))
	x = ScaleNearZero(x, TWO_PI);

    // initialize everything

    result = x;
    numerator = x;
    denominator = 1.0;
    factorial = -1.0;
    sign = 1.0;

    // Add the next term to the power series until within allowable
    // limits.

    do {

	// prepare for next term calculation

	factorial = factorial + 2.0;
	prevResult = result;
	sign = sign * -1.0;

	// calculate the next term

	numerator = numerator * x * x;
	denominator = denominator * (factorial + 1.0) * (factorial + 2.0);
	term = numerator / denominator;
	result = result + (sign * term);

	// Prep for termination check. Stop when the library's accuracy is 
	// reached.

	diff = _abs(_abs(prevResult) - _abs(result));
    }
    while (diff > TT_ACCURACY);

    return result;
}


/*****************************************************
 * FUNCTION:    	_cos
 *
 * DESCRIPTION: 	Calculate the cosine of a double radians number
 *					using  the power series cos x = 1 - 
 *					( x**2 / 2! ) + ( x**4 / 4! ) - ( x**6 / 6! ) + .....
 *
 * RETURNED:    	The result.
*****************************************************/
double _cos(double x)
{
    double result,
	prevResult, diff, numerator, denominator, term, sign, factorial;

    // Scale input angle to proper value between -2 PI and + 2 PI.
    // The power series is not as accurate outside that range.

    if ((x > TWO_PI) || (x < -TWO_PI))
	x = ScaleNearZero(x, TWO_PI);

    // initialize everything

    result = 1.0;
    prevResult = 1.0;
    numerator = x * x;
    denominator = 2.0;
    factorial = 0.0;
    sign = 1.0;

    do {

	// Prep for the next calculation

	prevResult = result;
	sign = -sign;
	factorial = factorial + 2.0;

	// Calculate the next term

	term = numerator / denominator;
	result = result + (sign * term);

	// Prepare for next sequence thru loop

	numerator = numerator * x * x;
	denominator = denominator * (factorial + 1.0) * (factorial + 2.0);

	// Check for loop termination. Stop when the library's accuracy is 
	// reached.

	diff = _abs(_abs(prevResult) - _abs(result));
    }
    while (diff > TT_ACCURACY);

    return result;
}
