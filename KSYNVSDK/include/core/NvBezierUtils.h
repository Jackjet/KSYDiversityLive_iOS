//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Mar 25. 2015
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"


enum ENvBezierPtComponent
{
    keNvBezierPtComponent_EndPoint = 1,
    keNvBezierPtComponent_BwdCtlPt = 2,
    keNvBezierPtComponent_FwdCtlPt = 4
};


static inline double NvBezierValueFromParam(const double *coeffs, double t)
{
    const double s = 1 - t, s2 = s * s, t2 = t * t;
    return s2 * s * coeffs[0] + 3 * s2 * t * coeffs[1] +
        3 * s * t2 * coeffs[2] + t2 * t * coeffs[3];
}

static inline double NvBezierValueFromParamEx(double p0, double p1, double p2, double p3, double t)
{
    const double s = 1 - t, s2 = s * s, t2 = t * t;
    return s2 * s * p0 + 3 * s2 * t * p1 + 3 * s * t2 * p2 + t2 * t * p3;
}


//
// Structure to hold the coefficients of the derived function of a cubic bezier equation f(t)
//
struct SNvBezierDerivedFunc {
    // Coefficients of the first derived function
    double a1, b1, c1;
};

//////////////////////////////////////////////////////////////////////////
//
// Cubic bezier curve equation:
// f(t) = ((1-t)^3)*P0 + 3*((1-t)^2)*t*P1 + 3*(1-t)*(t^2)*P2 + (t^3)*P3
//      = (-P0+3*P1-3*P2+P3)*(t^3) + (3*P0-6*P1+3*P2)*(t^2) + (-3*P0+3*P1)*t + P0
//
// f'(t) = (-3*P0+9*P1-9*P2+3*P3)*(t^2) + (6*P0-12*P1+6*P2)*t - 3*P0 + 3*P1
//
// f"(t) = (-6*P0+18*P1-18*P2+6*P3)*t + 6*P0 - 12*P1 + 6*P2
//
//////////////////////////////////////////////////////////////////////////
static inline void NvBezierCalcDerivedFunction(double p0, double p1, double p2, double p3,
                                               SNvBezierDerivedFunc *derivedFunc)
{
    derivedFunc->a1 = (p3 - p0) * 3 + (p1 - p2) * 9;
    derivedFunc->b1 = (p0 + p2) * 6 - p1 * 12;
    derivedFunc->c1 = (p1 - p0) * 3;
}

static inline void NvBezierCalcDerivedFunctionEx(const double *coeffs,
                                                 SNvBezierDerivedFunc *derivedFunc)
{
    NvBezierCalcDerivedFunction(coeffs[0], coeffs[1], coeffs[2], coeffs[3], derivedFunc);
}

static inline double NvBezierCalcTangent(const SNvBezierDerivedFunc *derivedFunc,
                                         const double parameter)
{
    return derivedFunc->a1 * parameter * parameter + derivedFunc->b1 * parameter + derivedFunc->c1;
}

// Calculate minimum and maximum value for a bezier function (t in [0,1])
void NvBezierCalcMinMax(double p0, double p1, double p2, double p3,
                        const SNvBezierDerivedFunc *derivedFunc,
                        double &minVal, double &maxVal);

// Calculate minimum and maximum value for a bezier function (t in [0,1])
void NvBezierCalcMinMaxEx(double p0, double p1, double p2, double p3,
                          double &minVal, double &maxVal);

// solve roots for cubic bezier curve equation B(t)=value, t is in the range [0,1]
// return the root count, negative return value indicate B(t) is a constant value equal to value
// roots value will be returned in pRoots (at most 3 roots), they are store in ascending order
int NvBezierSolveRoots(const double *coords, double value, double *roots);

