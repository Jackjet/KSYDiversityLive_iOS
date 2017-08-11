//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2017. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 4. 2017
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <NvDef.h>


#define NV_STREAMING_SDK_MAJOR_VERSION      1
#define NV_STREAMING_SDK_MINOR_VERSION      8
#define NV_STREAMING_SDK_REVISION           0

static inline uint64_t NvMakeSdkVersionNumber(int major, int minor, int revision)
{
    return ((uint64_t)major << 48) | ((uint64_t)minor << 32) | ((uint64_t)revision << 16);
}

static inline uint64_t NvSdkCurrentVersionNumber()
{
    return NvMakeSdkVersionNumber(NV_STREAMING_SDK_MAJOR_VERSION,
                                  NV_STREAMING_SDK_MINOR_VERSION,
                                  NV_STREAMING_SDK_REVISION);
}

