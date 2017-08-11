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

#include "INvAudioSamples.h"


interface INvAudioSamplesAllocator : public IUnknown
{
    virtual HRESULT __stdcall AllocateAudioSamples(
        ENvAudioSampleFormat in_audSmpFmt,
        unsigned int in_sampleRate,
        unsigned int in_maxSampleCount,
        unsigned int in_channelCount,
        INvAudioSamples **out_ppIAudioSamples) = 0;

    virtual HRESULT __stdcall Cleanup() = 0;
};

