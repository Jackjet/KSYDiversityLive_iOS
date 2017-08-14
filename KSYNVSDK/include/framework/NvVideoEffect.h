//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 7. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvFrameworkDef.h"
#include "NvVideoEffectParams.h"
#include "NvVideoFrameAllocator.h"
#include <NvVideoEffectDef.h>


///////////////////////////////////////////////////////////////////////////////////
// Video effect name list
///////////////////////////////////////////////////////////////////////////////////
#define NV_VIDEOEFFECT_NAME_HOST_GPU_COPIER         "host_gpu_copier"
#define NV_VIDEOEFFECT_NAME_HOST_COPIER             "host_copier"
#define NV_VIDEOEFFECT_NAME_GPU_RESIZER             "gpu_resizer"
#define NV_VIDEOEFFECT_NAME_GPU_TRANSFORM2D         "gpu_transform2d"
#define NV_VIDEOEFFECT_NAME_GPU_COMPOSITOR          "gpu_compositor"

#define NV_VIDEOEFFECT_NAME_TRACKER                 "tracker"

#define NV_VIDEOEFFECT_NAME_CAPTION                 "caption"

#define NV_VIDEOEFFECT_NAME_HISTOGRAM               "histogram"
#define NV_VIDEOEFFECT_NAME_BASIC_CC                "basic cc"
#define NV_VIDEOEFFECT_NAME_BEAUTY                  "beauty"
#define NV_VIDEOEFFECT_NAME_FAST_BLUR               "fast blur"
#define NV_VIDEOEFFECT_NAME_TILTSHIFT_LENS          "tilt-shift lens"
#define NV_VIDEOEFFECT_NAME_FISHEYE                 "fish eye"
#define NV_VIDEOEFFECT_NAME_BORDER                  "border"
#define NV_VIDEOEFFECT_NAME_MASTER_KEYER            "master keyer"
#define NV_VIDEOEFFECT_NAME_VIDEO_ECHO              "video echo"
#define NV_VIDEOEFFECT_NAME_PANORAMIC_MAPPING       "panoramic mapping"

#define NV_VIDEOEFFECT_NAME_PUSH                    "push"
#define NV_VIDEOEFFECT_NAME_PAGECURL                "page curl"
#define NV_VIDEOEFFECT_NAME_WIPE                    "wipe"
#define NV_VIDEOEFFECT_NAME_FADE                    "video fade"
#define NV_VIDEOEFFECT_NAME_MONTION_WIPE            "montion wipe"

#define NV_VIDEOEFFECT_NAME_FOREGROUND_ANIMATION    "foreground animation"

#define NV_VIDEOEFFECT_NAME_STORYBOARD              "storyboard"
#define NV_VIDEOEFFECT_NAME_STORYBOARD_WIPE         "storyboard wipe"
#define NV_VIDEOEFFECT_NAME_STORYBOARD3D            "storyboard3d"

#define NV_VIDEOEFFECT_NAME_CUSTOM_GPU_EFFECT       "custom gpu"

#define NV_VIDEOEFFECT_NAME_FACE_UNITY_EFFECT       "face unity"


// Create a video effect descriptor object
HRESULT NvCreateVideoEffectDescriptor(const char *in_effectName,
                                      INvEffectDescriptor **out_ppIEffectDescriptor);

// Create a video effect object
// NOTE: When calling this function, an OpenGL context is not required even if the
// effect being created is a GPU effect
HRESULT NvCreateVideoEffect(const char *in_effectName,
                            INvVideoEffectResourceManager *in_pIResourceManager,
                            INvVideoEffect **out_ppIVideoEffect);


// Create a video effect resource manager
HRESULT NvCreateVideoEffectResourceManager(INvVideoFrameAllocator *in_pIHostVideoFrameAllocator,
                                           INvVideoFrameAllocator *in_pIOpenGLVideoFrameAllocator,
                                           INvVideoEffectResourceManager **out_ppIResourceManager);

// Helper function to render video effect
HRESULT NvVideoEffectRenderHelper(INvVideoEffect *in_pIVideoEffect,
                                  INvVideoFrame *in_inputFrames[],
                                  unsigned int in_inputFrameCount,
                                  INvEffectSettings *in_pIEffectSettings,
                                  INvEffectContext *in_pIEffectContext,
                                  unsigned int in_renderFlags,
                                  const SNvVideoResolution *in_outputResolution,
                                  INvVideoFrame **out_outputFrame);

// Helper function to upload a host frame to GPU frame
HRESULT NvUploadVideoFrameToGPUHelper(INvVideoFrame *in_pIHostFrame,
                                      INvVideoEffectResourceManager *in_pIResourceManager,
                                      INvVideoFrame **out_ppIGPUFrame);

// Helper function to download a video frame from GPU to host
HRESULT NvDownloadVideoFrameFromGPUHelper(INvVideoFrame *in_pIGPUFrame,
                                          INvVideoEffectResourceManager *in_pIResourceManager,
                                          ENvPixelFormat in_hostPixFmt,
                                          INvVideoFrame **out_ppIHostFrame);

