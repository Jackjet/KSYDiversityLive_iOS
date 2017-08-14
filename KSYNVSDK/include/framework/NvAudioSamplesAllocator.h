//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Mar 7. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvFrameworkDef.h"
#include <INvAudioSamplesAllocator.h>


HRESULT NvCreateHostAudioSamplesAllocator(const wchar_t *in_allocatorName, // Can be null
                                          INvAudioSamplesAllocator **out_ppIAllocator);

