//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2015. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    July 28 2015
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"


// control point coordinate adjustment mode
enum ENvIntpCurvesCtlPtAdjMode
{
    keNvIntpCurvesCtlPtAdjMode_KeepDirection,
    keNvIntpCurvesCtlPtAdjMode_KeepYCoord
};


// interpolation curves object handle
typedef void * NV_HINTPCURVES;

// iterator to iterate end points in an interpolation curves object
typedef void * NV_INTPCURVES_ITERATOR;


// struct to hold all the coordinates information of an end point in an interpolation curves object
struct SNvIntpCurvesPtCoords {
    SNvPointD sEndPoint;                // end point coordinates
    SNvPointD sBackwardCtlPoint;        // backward control point coordinates
    SNvPointD sForwardCtlPoint;         // forward control point coordinates
    SNvPointD sValidBackwardCtlPoint;   // adjusted backward control point coordinates
    SNvPointD sValidForwardCtlPoint;    // adjusted forward control point coordinates
};


// create an interpolation curves object, return the handle of the object
// @hMemAllocator: memory allocator provided to this interpolation curves object
// if hMemAllocator is set to NULL, it will create internal allocator
// NOTE: if you provide an memory allocator when creating an interpolation curves object,
// you must explicitly destroy the memory allocator after the interpolation curves object has been destroyed.
NV_HINTPCURVES NvIntpCurvesCreate();

// destroy an interpolation curves object
void NvIntpCurvesDestroy(NV_HINTPCURVES hIntpCurves);

// copy a source interpolation curves to a destination interpolation curves
int NvIntpCurvesCopy(NV_HINTPCURVES hSrcIntpCurves, NV_HINTPCURVES hDstIntpCurves);

// clear all the end/control points in an interpolation curves object
void NvIntpCurvesClear(NV_HINTPCURVES hIntpCurves);

// add an end point with its two control points to an interpolation curves object
// if there are already some end points with the same x coordinate, we use nCollisionIndex to
// determine the relative position in the sequence, if nCollisionIndex is -1, we always add
// this end point after the last end point which has the same x coordinate.
// return the iterator of the newly added end point
NV_INTPCURVES_ITERATOR NvIntpCurvesAddPoint(NV_HINTPCURVES hIntpCurves,
                                            const SNvPointD *pEndPoint,
                                            const SNvPointD *pBackwardCtlPoint,
                                            const SNvPointD *pForwardCtlPoint);

// move the position of the control point(s) of one end point in an interpolation curves object
int NvIntpCurvesMoveControlPoint(NV_HINTPCURVES hIntpCurves,
                                 NV_INTPCURVES_ITERATOR itr,
                                 const SNvPointD *pBackwardCtlPoint,
                                 ENvIntpCurvesCtlPtAdjMode eBackwardAdjMode,
                                 const SNvPointD *pForwardCtlPoint,
                                 ENvIntpCurvesCtlPtAdjMode eForwardAdjMode);

// return the end point count in an interpolation curves object
int NvIntpCurvesGetPointCount(NV_HINTPCURVES hIntpCurves);

// get coordinates information of an end point in an interpolation curves object
int NvIntpCurvesGetPointCoords(NV_HINTPCURVES hIntpCurves,
                               NV_INTPCURVES_ITERATOR itr,
                               SNvIntpCurvesPtCoords *pPtCoords);

// get the iterator of the first(leftmost) end point in an interpolation curves object
NV_INTPCURVES_ITERATOR NvIntpCurvesGetFirstPoint(NV_HINTPCURVES hIntpCurves);

// get the iterator of the last(rightmost) end point in an interpolation curves object
NV_INTPCURVES_ITERATOR NvIntpCurvesGetLastPoint(NV_HINTPCURVES hIntpCurves);

// get the iterator of the end point prior(left) to the specified end point
NV_INTPCURVES_ITERATOR NvIntpCurvesGetPrevPoint(NV_HINTPCURVES hIntpCurves,
                                                NV_INTPCURVES_ITERATOR itr);

// get the iterator of the end point next(right) to the specified end point
NV_INTPCURVES_ITERATOR NvIntpCurvesGetNextPoint(NV_HINTPCURVES hIntpCurves,
                                                NV_INTPCURVES_ITERATOR itr);

// interpolate a y value given a x coordinate value with constant wrapping
double NvIntpCurvesInterpolate(NV_HINTPCURVES hIntpCurves, double xCoord);

// interpolate a y value given a x coordinate value with tangent wrapping
double NvIntpCurvesInterpolateTangentWrapping(NV_HINTPCURVES hIntpCurves, double xCoord);

