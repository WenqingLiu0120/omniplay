/* Copyright (C) 2005, 2006, 2011 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Richard Henderson <rth@redhat.com>, 2005.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <stdbool.h>
#include <math.h>
#include <complex.h>

attribute_hidden
long double _Complex
__divtc3 (long double a, long double b, long double c, long double d)
{
  long double denom, ratio, x, y;

  /* ??? We can get better behavior from logarithmic scaling instead of
     the division.  But that would mean starting to link libgcc against
     libm.  We could implement something akin to ldexp/frexp as gcc builtins
     fairly easily...  */
  if (fabsl (c) < fabsl (d))
    {
      ratio = c / d;
      denom = (c * ratio) + d;
      x = ((a * ratio) + b) / denom;
      y = ((b * ratio) - a) / denom;
    }
  else
    {
      ratio = d / c;
      denom = (d * ratio) + c;
      x = ((b * ratio) + a) / denom;
      y = (b - (a * ratio)) / denom;
    }

  /* Recover infinities and zeros that computed as NaN+iNaN; the only cases
     are nonzero/zero, infinite/finite, and finite/infinite.  */
  if (isnan (x) && isnan (y))
    {
      if (denom == 0.0 && (!isnan (a) || !isnan (b)))
	{
	  x = __copysignl (INFINITY, c) * a;
	  y = __copysignl (INFINITY, c) * b;
	}
      else if ((__isinf_nsl (a) || __isinf_nsl (b))
	       && isfinite (c) && isfinite (d))
	{
	  a = __copysignl (__isinf_nsl (a) ? 1 : 0, a);
	  b = __copysignl (__isinf_nsl (b) ? 1 : 0, b);
	  x = INFINITY * (a * c + b * d);
	  y = INFINITY * (b * c - a * d);
	}
      else if ((__isinf_nsl (c) || __isinf_nsl (d))
	       && isfinite (a) && isfinite (b))
	{
	  c = __copysignl (__isinf_nsl (c) ? 1 : 0, c);
	  d = __copysignl (__isinf_nsl (d) ? 1 : 0, d);
	  x = 0.0 * (a * c + b * d);
	  y = 0.0 * (b * c - a * d);
	}
    }

  return x + I * y;
}
