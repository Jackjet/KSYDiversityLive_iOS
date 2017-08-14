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
#include "NvAudioSamplesAllocator.h"
#include <NvAudioEffectDef.h>


///////////////////////////////////////////////////////////////////////////////////
// Audio effect render flag
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
// Audio effect parameter id
///////////////////////////////////////////////////////////////////////////////////

// Audio copier effect
#define NV_AUDIO_COPIER_PARAM_ID_OUTPUT_SAMPLE_FORMAT  100
#define NV_AUDIO_COPIER_PARAM_ID_OUTPUT_CHANNEL_COUNT  200

// Audio mixer effect
#define NV_AUDIO_MIXER_PARAM_ID_LEFT_TO_LEFT            -100
#define NV_AUDIO_MIXER_PARAM_ID_LEFT_TO_RIGHT           -200
#define NV_AUDIO_MIXER_PARAM_ID_RIGHT_TO_RIGHT          -300
#define NV_AUDIO_MIXER_PARAM_ID_RIGHT_TO_LEFT           -400

// Audio fading effect
#define NV_AUDIO_FADING_PARAM_ID_PROGRESS               100

// Audio echo effect
#define NV_AUDIO_ECHO_PARAM_ID_DELAY                    100
#define NV_AUDIO_ECHO_PARAM_ID_DECAY                    200

// Audio sound touch effect
#define NV_AUDIO_SOUNDTOUCH_PARAM_ID_TEMPO_DELTA        100
#define NV_AUDIO_SOUNDTOUCH_PARAM_ID_PITCH_DELTA        200
#define NV_AUDIO_SOUNDTOUCH_PARAM_ID_RATE_DELTA         300
#define NV_AUDIO_SOUNDTOUCH_PARAM_ID_SPEECH             400

// Audio reverb effect
#define NV_AUDIO_REVERB_PARAM_ID_REVERBERANCE           100
#define NV_AUDIO_REVERB_PARAM_ID_HF_DAMPING             200
#define NV_AUDIO_REVERB_PARAM_ID_ROOM_SCALE             300
#define NV_AUDIO_REVERB_PARAM_ID_STEREO_DEPTH           400
#define NV_AUDIO_REVERB_PARAM_ID_PRE_DELAY              500
#define NV_AUDIO_REVERB_PARAM_ID_WET_GAIN               600
#define NV_AUDIO_REVERB_PARAM_ID_WET_ONLY               700

// Audio Wahwah effect
#define NV_AUDIO_WAHWAH_PARAM_ID_FREQ                   100
#define NV_AUDIO_WAHWAH_PARAM_ID_PHASE                  200
#define NV_AUDIO_WAHWAH_PARAM_ID_DEPTH                  300
#define NV_AUDIO_WAHWAH_PARAM_ID_RESONANCE              400
#define NV_AUDIO_WAHWAH_PARAM_ID_FREQ_OFFSET            500

