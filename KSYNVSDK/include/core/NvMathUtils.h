//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 8. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"


static inline bool NvIsPowerOfTwo(unsigned int n)
{
    return n == (n & (unsigned int)(-(int)n));
}

// Return the lowest number greater than v, which is a power of 2
uint32_t NvNextPowerOfTwo(uint32_t v);

// Find GCD(greatest common divisor) of two numbers
int NvFindGCD(int a, int b);

// devide num/den by their GCD
void NvNormalizeRational(SNvRational *r);

// Check if a rational is equal to another rational
bool NvIsEqualRational(const SNvRational *r1, const SNvRational *r2);

// Rescale a 64-bit integer by 2 rational numbers
// return round(i * r1 / r2), rounding method is away from zero
int64_t NvRescaleInt64(int64_t i, const SNvRational *r1, const SNvRational *r2);

// Check whether a rectangle is empty
bool NvIsRectangleEmpty(const SNvRectF *rect);

void NvCalcLetterBoxRectangle(const SNvRectF *refRect,
                              float srcWidth,
                              float srcHeight,
                              SNvRectF *resultRect);

void NvCalcPanAndScanRectangle(const SNvRectF *refRect,
                               float srcWidth,
                               float srcHeight,
                               float pan,   // Control translation (-1 -- 1)
                               float scan,  // Control scale (0 -- 1)
                               SNvRectF *resultRect);

// Interpolate two integers linearly
static inline int NvIntpIntegerValue(int from, int to, float factor)
{
    const float result = (to - from) * factor + from;
    if (result >= 0)
        return (int)(result + 0.5f);
    else
        return (int)(result - 0.5f);
}

// Interpolate two single precision float value linearly
static inline float NvIntpFloatValue(float from, float to, float factor)
{
    return(to - from) * factor + from;
}

// Interpolate two double precision float value linearly
static inline float NvIntpFloatValue(double from, double to, double factor)
{
    return(to - from) * factor + from;
}

// Interpolate two color linearly
static inline void NvIntpColorValue(const SNvColor *from, const SNvColor *to, float factor, SNvColor *result)
{
    result->r = NvIntpFloatValue(from->r, to->r, factor);
    result->g = NvIntpFloatValue(from->g, to->g, factor);
    result->b = NvIntpFloatValue(from->b, to->b, factor);
    result->a = NvIntpFloatValue(from->a, to->a, factor);
}

// Interpolate two 2D position linearly
static inline void NvIntpPosition2DValue(const SNvPosition2D *from, const SNvPosition2D *to, float factor, SNvPosition2D *result)
{
    result->x = NvIntpFloatValue(from->x, to->x, factor);
    result->y = NvIntpFloatValue(from->y, to->y, factor);
}

// Interpolate two 3D position linearly
static inline void NvIntpPosition3DValue(const SNvPosition3D *from, const SNvPosition3D *to, float factor, SNvPosition3D *result)
{
    result->x = NvIntpFloatValue(from->x, to->x, factor);
    result->y = NvIntpFloatValue(from->y, to->y, factor);
    result->z = NvIntpFloatValue(from->z, to->z, factor);
}

