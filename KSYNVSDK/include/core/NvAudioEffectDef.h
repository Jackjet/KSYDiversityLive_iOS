//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Mar 10 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvEffectDef.h"
#include "INvAudioSamples.h"
#include <QVector>


#define NV_MAX_AUDIOEFFECT_INPUTSAMPLES     32

///////////////////////////////////////////////////////////////////////////////////
// Video effect flag
///////////////////////////////////////////////////////////////////////////////////

// Indicate the effect require an effect settings object to work
#define NV_AUDIOEFFECT_FLAG_NEED_SETTINGS                   (1 << 0)

// Indicate the effect require an effect context object to work
#define NV_AUDIOEFFECT_FLAG_NEED_CONTEXT                    (1 << 1)


//
// Audio effect resource manager interface
// NOTE: This interface is NOT thread-safe!
//
interface INvAudioSamplesAllocator;
interface INvAudioEffect;

interface INvAudioEffectResourceManager : public IUnknown
{
    virtual HRESULT __stdcall GetAudioSamplesAllocator(INvAudioSamplesAllocator **out_ppIAudioSamplesAllocator) = 0;

    virtual HRESULT __stdcall GetAudioCopier(INvAudioEffect **out_ppIAudioCopier) = 0;

    virtual HRESULT __stdcall GetAudioMixer(INvAudioEffect **out_ppIAudioMixer) = 0;
};


//
// Audio effect inteface
//
interface INvAudioEffect : public IUnknown
{
    ///////////////////////////////////////////////////////////////////////////////////
    // The following methods should be thread-safe
    ///////////////////////////////////////////////////////////////////////////////////
    virtual HRESULT __stdcall GetEffectDescriptor(INvEffectDescriptor **out_ppIEffectDescriptor) = 0;
    virtual HRESULT __stdcall GetEffectDescription(SNvEffectDescription *out_effectDesc) = 0;
    virtual HRESULT __stdcall GetEffectSettings(INvEffectSettings **out_ppIEffectSettings) = 0;
    virtual HRESULT __stdcall GetEffectContext(INvEffectContext **out_ppIEffectContext) = 0;

    virtual bool __stdcall IsIdentity(INvAudioSamples *in_inputSamples[],
                                      unsigned int in_inputSamplesCount,
                                      INvEffectSettings *in_pIEffectSettings,
                                      unsigned int in_renderFlags,
                                      unsigned int *out_identityInputIndex) = 0;

    virtual QVector<ENvAudioSampleFormat> __stdcall QuerySupportedInputAudioSampleFormats() = 0;
    virtual QVector<unsigned int> __stdcall QuerySupportedInputAudioChannelCount() = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // The following methods are NOT thread-safe!
    ///////////////////////////////////////////////////////////////////////////////////
    virtual HRESULT __stdcall Render(INvAudioSamples *in_inputFrames[],
                                     unsigned int in_inputSamplesCount,
                                     INvEffectSettings *in_pIEffectSettings,
                                     INvEffectContext *in_pIEffectContext,
                                     unsigned int in_renderFlags,
                                     INvAudioSamples **out_ppIOutputSamples) = 0;

    // NOTE: you should call Flush() in the same thread as you call Render()
    // NOTE: you should call this method only if the effect reqiure context
    virtual void __stdcall Flush(INvEffectContext *in_pIEffectContext,
                                 unsigned int in_renderFlags,
                                 INvAudioSamples **out_ppIOutputSamples) = 0;

    // NOTE: you should call ReleaseResources() in the same thread as you call Render()
    virtual void __stdcall ReleaseResources() = 0;
};

