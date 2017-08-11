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

#include "NvDef.h"
#include "IUnknown.h"
#include <QString>
#include <QVariant>


//
// INvAudioSamples interface
//
interface INvAudioSamples : public IUnknown
{
    // Get audio sample format of the audio samples
    virtual ENvAudioSampleFormat __stdcall GetAudioSampleFormat() = 0;

    // Get sample rate of the audio samples
    virtual unsigned int __stdcall GetSampleRate() = 0;

    // Get maximum audio sample count maybe stored in this object
    virtual unsigned int __stdcall GetMaxSampleCount() = 0;

    // Get Channel count of the audio samples
    virtual unsigned int __stdcall GetChannelCount() = 0;

    // Get audio samples buffer
    virtual HRESULT __stdcall GetAudioSamplesBuffer(SNvAudioSamplesBuffer *out_samplesBuffer) = 0;

    // Set/Get actual audio sample count stored in this object
    virtual HRESULT __stdcall SetActualSampleCount(unsigned int in_sampleCount) = 0;
    virtual unsigned int __stdcall GetActualSampleCount() = 0;

    // Set/Get stream timestamp in 1/NV_TIME_BASE unit
    virtual HRESULT __stdcall SetStreamTime(int64_t in_streamTime) = 0;
    virtual int64_t __stdcall GetStreamTime() = 0;

    // Set/Get meta data
    virtual void __stdcall SetMetaData(const char *key, const QVariant &value) = 0;
    virtual QVariant __stdcall GetMetaData(const char *key) = 0;

    // Set/Get custom object
    virtual HRESULT __stdcall SetCustomObject(const GUID &in_objId, IUnknown *in_pIUnk) = 0;
    virtual HRESULT __stdcall GetCustomObject(const GUID &in_objId, IUnknown **out_ppIUnk) = 0;
};


//
// Predefined meta data key of audio samples
//

// NOTE: This meta data should be only used by AV File writer
#define NV_AUDIOSAMPLES_META_KEY_PIPELINE_RESOURCE_COUNT    "pipe-res-cnt"


//
// Helper funtions to set/get predefined meta data of audio samples
//
static inline void NvSetAudioSamplesPipelineResourceCount(INvAudioSamples *pIAudioSamples, int pipelineResourceCount)
{
    pIAudioSamples->SetMetaData(NV_AUDIOSAMPLES_META_KEY_PIPELINE_RESOURCE_COUNT, pipelineResourceCount);
}

static inline int NvGetAudioSamplesPipelineResourceCount(INvAudioSamples *pIAudioSamples)
{
    return pIAudioSamples->GetMetaData(NV_AUDIOSAMPLES_META_KEY_PIPELINE_RESOURCE_COUNT).toInt();
}

