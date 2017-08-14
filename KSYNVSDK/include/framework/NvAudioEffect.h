//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Mar 10. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvFrameworkDef.h"
#include "NvAudioEffectParams.h"
#include "NvAudioSamplesAllocator.h"
#include <NvAudioEffectDef.h>


///////////////////////////////////////////////////////////////////////////////////
// Audio effect name list
///////////////////////////////////////////////////////////////////////////////////
#define NV_AUDIOEFFECT_NAME_COPIER                  "audio_copier"

#define NV_AUDIOEFFECT_NAME_MIXER                   "audio_mixer"

#define NV_AUDIOEFFECT_NAME_FADING                  "audio_fading"

#define NV_AUDIOEFFECT_NAME_ECHO                    "audio_echo"

#define NV_AUDIOEFFECT_NAME_SOUNDTOUCH              "audio_soundtouch"

#define NV_AUDIOEFFECT_NAME_REVERB                  "audio_reverb"

#define NV_AUDIOEFFECT_NAME_WAHWAH                  "audio_wahwah"


// Create a audio effect descriptor object
HRESULT NvCreateAudioEffectDescriptor(const char *in_effectName,
                                      INvEffectDescriptor **out_ppIEffectDescriptor);

// Create an audio effect object
HRESULT NvCreateAudioEffect(const char *in_effectName,
                            INvAudioSamplesAllocator *in_pIAudioSamplesAllocator,
                            INvAudioEffect **out_ppIAudioEffect);

// Create an audio effect resource manager
HRESULT NvCreateAudioEffectResourceManager(INvAudioSamplesAllocator *in_pIAudioSamplesAllocator,
                                           INvAudioEffectResourceManager **out_ppIResourceManager);

// Helper function to render audio effect
HRESULT NvAudioEffectRenderHelper(INvAudioEffect *in_pIAudioEffect,
                                  INvAudioEffectResourceManager *in_pIResourceManager,
                                  INvAudioSamples *in_inputSamples[],
                                  unsigned int in_inputSamplesCount,
                                  INvEffectSettings *in_pIEffectSettings,
                                  INvEffectContext *in_pIEffectContext,
                                  unsigned int in_renderFlags,
                                  INvAudioSamples **out_ppIOutputSamples);

