//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 29. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvEffectDef.h"
#include "INvVideoFrame.h"
#include <QVector>


#define NV_MAX_VIDEOEFFECT_INPUTFRAMES      8

///////////////////////////////////////////////////////////////////////////////////
// Video effect flag
///////////////////////////////////////////////////////////////////////////////////

// Indicate the effect may require GPU resources, especially an OpenGL context
#define NV_VIDEOEFFECT_FLAG_GPU                             (1 << 0)

// Indicate the effect require an effect settings object to work
#define NV_VIDEOEFFECT_FLAG_NEED_SETTINGS                   (1 << 1)

// Indicate the effect require an effect context object to work
#define NV_VIDEOEFFECT_FLAG_NEED_CONTEXT                    (1 << 2)

// Indicate the effect accept null input frame
#define NV_VIDEOEFFECT_FLAG_ALLOW_NULL_INPUT_FRAME          (1 << 3)

// Indicate the effect don't need a resource manager to work
#define NV_VIDEOEFFECT_FLAG_NO_RESOURCE_MANAGER             (1 << 4)

// Indicate the effect require that input/output frames should have identical image position
#define NV_VIDEOEFFECT_FLAG_IDENTICAL_POSITION              (1 << 5)

// Indicate the effect require that input/output frames should have identical pixel aspect ratio
#define NV_VIDEOEFFECT_FLAG_IDENTICAL_PIXEL_ASPECT_RATIO    (1 << 6)

// Indicate the effect require that input/output frames should have identical proxy scale
#define NV_VIDEOEFFECT_FLAG_IDENTICAL_PROXY_SCALE           (1 << 7)


//
// Video effect resource manager interface
// NOTE: This interface is NOT thread-safe! it can only be called from the the thread is was created!
//
interface INvVideoFrameAllocator;
interface INvVideoEffect;
class CNvParallelRunner;
class CNvHardwareInfo;

interface INvVideoEffectResourceManager : public IUnknown
{
    virtual HRESULT __stdcall GetHostVideoFrameAllocator(INvVideoFrameAllocator **out_ppIHostVideoFrameAllocator) = 0;
    virtual HRESULT __stdcall GetOpenGLVideoFrameAllocator(INvVideoFrameAllocator **out_ppIOpenGLVideoFrameAllocator) = 0;

    virtual HRESULT __stdcall GetHostGPUCopier(INvVideoEffect **out_ppIHostGPUCopier) = 0;
    virtual HRESULT __stdcall GetHostCopier(INvVideoEffect **out_ppIHostCopier) = 0;
    virtual HRESULT __stdcall GetGPUResizer(INvVideoEffect **out_ppIGPUResizer) = 0;
    virtual HRESULT __stdcall GetGPUTransform2D(INvVideoEffect **out_ppIGPUTransform2D) = 0;

    virtual HRESULT __stdcall GetFastBlur(INvVideoEffect **out_ppIFastBlur) = 0;
    virtual HRESULT __stdcall GetGPUCompositor(INvVideoEffect **out_ppIGPUCompositor) = 0;

    virtual CNvParallelRunner * __stdcall GetParallelRunner() = 0;
    virtual CNvHardwareInfo * __stdcall GetHardwareInfo() = 0;

    virtual void __stdcall ReleaseResources() = 0;

    // NOTE: This function will not be called automaticly when the resource manager is being destroyed,
    // you must call it explicitly on exit, otherwise there may be resource leaks!
    virtual void __stdcall Cleanup() = 0;
};

//
// Video effect inteface
//
interface INvVideoEffect : public IUnknown
{
    ///////////////////////////////////////////////////////////////////////////////////
    // The following methods should be thread-safe
    ///////////////////////////////////////////////////////////////////////////////////
    virtual HRESULT __stdcall GetEffectDescriptor(INvEffectDescriptor **out_ppIEffectDescriptor) = 0;
    virtual HRESULT __stdcall GetEffectDescription(SNvEffectDescription *out_effectDesc) = 0;
    virtual HRESULT __stdcall GetResourceManager(INvVideoEffectResourceManager **out_ppIResourceManager) = 0;
    virtual HRESULT __stdcall GetEffectSettings(INvEffectSettings **out_ppIEffectSettings) = 0;
    virtual HRESULT __stdcall GetEffectContext(INvEffectContext **out_ppIEffectContext) = 0;

    virtual bool __stdcall IsIdentity(INvVideoFrame *in_inputFrames[],
                                      unsigned int in_inputFrameCount,
                                      INvEffectSettings *in_pIEffectSettings,
                                      unsigned int in_renderFlags,
                                      unsigned int *out_identityInputIndex) = 0;

    virtual QVector<ENvPixelFormat> __stdcall QuerySupportedPixFmtOfInputFrame(unsigned int in_inputFrameIndex) = 0;

    virtual HRESULT __stdcall QueryOutputFramePixelAspectRatio(INvVideoFrame *in_inputFrames[],
                                                               unsigned int in_inputFrameCount,
                                                               INvEffectSettings *in_pIEffectSettings,
                                                               unsigned int in_renderFlags,
                                                               SNvRational *out_outputImgPAR) = 0;

    virtual HRESULT __stdcall QueryOutputFramePosition(INvVideoFrame *in_inputFrames[],
                                                       unsigned int in_inputFrameCount,
                                                       INvEffectSettings *in_pIEffectSettings,
                                                       unsigned int in_renderFlags,
                                                       const SNvRational *in_outputImgPAR,
                                                       const SNvRational *in_outputImgProxyScale,
                                                       SNvImagePosition *out_outputImgPos) = 0;

    // NOTE: you MUST make sure all input frames's pixel format is supported before calling this function
    virtual ENvPixelFormat __stdcall QueryOutputFramePixelFormat(INvVideoFrame *in_inputFrames[],
                                                                 unsigned int in_inputFrameCount,
                                                                 INvEffectSettings *in_pIEffectSettings,
                                                                 unsigned int in_renderFlags) = 0;

    ///////////////////////////////////////////////////////////////////////////////////
    // The following methods are NOT thread-safe!
    ///////////////////////////////////////////////////////////////////////////////////

    // Preload resource of the video effect
    // NOTE: you must call PreloadResources() in the thread which has the OpenGL context!
    virtual void __stdcall PreloadResources() = 0;
    // Preload resource of a context of the video effect
    // NOTE: you must call PreloadResources() in the thread which has the OpenGL context!
    virtual void __stdcall PreloadContextResources(INvEffectContext *in_pIEffectContext,
                                                   INvEffectSettings *in_pIEffectSettings,
                                                   const SNvVideoResolution *in_videoOutputResolution,
                                                   bool in_isTransition) = 0;

    // NOTE: you must call Render() in the thread at which you have created its resource manager!
    // NOTE: for GPU effect you should call Render() in the thread which has the OpenGL context!
    // and you should make sure a valid FBO is currently available!
    virtual HRESULT __stdcall Render(INvVideoFrame *in_inputFrames[],
                                     unsigned int in_inputFrameCount,
                                     INvVideoFrame *io_outputFrame,
                                     INvEffectSettings *in_pIEffectSettings,
                                     INvEffectContext *in_pIEffectContext,
                                     unsigned int in_renderFlags) = 0;

    // NOTE: you should call ReleaseResources() in the same thread as you call Render()
    virtual void __stdcall ReleaseResources() = 0;

    // NOTE: you should call Cleanup() in the same thread as you call Render(),
    // this function will not be called automaticly when the video effect is being destroyed,
    // you must call it explicitly on exit, otherwise there may be resource leaks!
    virtual void __stdcall Cleanup() = 0;
};


//
// Custom GPU video effect callback interface
//
struct INvCustomGpuVideoEffectCallback
{
public:
    virtual int AllocateRGBATexture(unsigned int in_width, unsigned int in_height) = 0;
    virtual void ReclaimTexture(int texId) = 0;
};


//
// Custom GPU video effect interface
//
interface INvCustomGpuVideoEffect : public IUnknown
{
public:
    // Preload resource of the custom GPU video effect
    // NOTE: You must call this method in the thread which has the OpenGL context!
    virtual void __stdcall PreloadResources() = 0;

    // NOTE: You must call this method in the thread which has the OpenGL context!
    virtual void __stdcall Render(INvVideoFrame *in_pIInputFrame,
                                  INvVideoFrame *io_pIOutputFrame,
                                  INvCustomGpuVideoEffectCallback *in_pICallback,
                                  int64_t in_effectTime,
                                  int64_t in_effectStartTime,
                                  int64_t in_effectEndTime) = 0;

    // NOTE: This can be called in any thread
    virtual void __stdcall Cleanup() = 0;
};

//
// Custom GPU video effect context interface
//
interface INvCustomGpuVideoEffectContext : public IUnknown
{
public:
    virtual void __stdcall SetCustomGpuEffect(INvCustomGpuVideoEffect *in_pICustomGpuVideoEffect) = 0;

    virtual void __stdcall GetCustomGpuEffect(INvCustomGpuVideoEffect **out_ppICustomGpuVideoEffect) = 0;
};

