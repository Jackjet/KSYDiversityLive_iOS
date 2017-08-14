//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Aug 8. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <NvDef.h>


enum ENvCafImageFormat
{
    keNvCafImageFormat_Invalid = -1,
    keNvCafImageFormat_JPG = 0,
    keNvCafImageFormat_PNG,
    keNvCafImageFormat_Count
};

enum ENvCafLoopMode
{
    keNvCafLoopMode_Invalid = -1,
    keNvCafLoopMode_None = 0,
    keNvCafLoopMode_Repeat,
    keNvCafLoopMode_Mirror,
    keNvCafLoopMode_RepeatLastFrame,
    keNvCafLoopMode_Count
};

