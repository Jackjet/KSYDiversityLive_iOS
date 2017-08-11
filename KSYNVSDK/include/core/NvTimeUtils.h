//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2017. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 14. 2017
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"


// Convert 1/NV_TIME_BASE time unit to millisecond
double NvConvertNvTimeToMillisecond(int64_t nvTime);
int64_t NvConvertNvTimeToMillisecondInt64(int64_t nvTime);

// Convert millisecond to 1/NV_TIME_BASE time unit
int64_t NvConvertMillisecondToNvTime(double ms);
int64_t NvConvertMillisecondToNvTimeInt64(int64_t ms);

