//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 15. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"
#include "IUnknown.h"

#include <QString>
#include <QVariant>
#include <QThread>


//
// Predefined meta data key of video frame
//

// NOTE: This meta data only affect host video frame with alpha channel,
// all GPU video frame should be premultiplied
#define NV_VIDEOFRAME_META_KEY_NON_PREMULTIPLIED        "non-premul"

// NOTE: This meta data only affect host YUV video frame
#define NV_VIDEOFRAME_META_KEY_LUMA_601                 "luma601"

// NOTE: This meta data only affect host YUV video frame
#define NV_VIDEOFRAME_META_KEY_FULL_RANGE_YUV           "full-range-yuv"

// NOTE: This meta data only affect GPU video frame
#define NV_VIDEOFRAME_META_KEY_UPSIDE_DOWN_TEXTURE      "upside-down-tex"


//
// INvVideoFrame interface
//
class QImage;

interface INvVideoFrame : public IUnknown
{
    // Get pixel format of the video frame
    virtual ENvPixelFormat __stdcall GetPixelFormat() = 0;

    // Get memory location of the video frame
    virtual ENvMemoryLocation __stdcall GetMemoryLocation() = 0;

    // Get image position of the video frame
    virtual HRESULT __stdcall GetImagePosition(SNvImagePosition *out_imgPos) = 0;

    // Get pixel aspect ratio of the video frame
    virtual HRESULT __stdcall GetPixelAspectRatio(SNvRational *out_pixelAspectRatio) = 0;

    // Get proxy scale of the video frame
    virtual HRESULT __stdcall GetProxyScale(SNvRational *out_proxyScale) = 0;

    // The angle that the video image needs to be rotated clockwise
    // so it shows correctly on the display in its natural orientation
    virtual HRESULT __stdcall SetDisplayRotation(ENvVideoRotation in_videoRotation) = 0;
    virtual ENvVideoRotation __stdcall DisplayRotation() = 0;

    // Get image buffer pointers, can only be used when memory location is host
    virtual HRESULT __stdcall GetImageBuffer(SNvImageBuffer *out_imgBuffer) = 0;

    // Return internal QImage object pointer,
    // return null pointer if the video frame is not backed by QImage
    virtual const QImage * __stdcall GetInternalQImage() = 0;

    // Get internal handle of the video frame
    // return 0 if memory location is host
    // return texture id if memory location is openGL texture
    virtual uintptr_t __stdcall GetHandle() = 0;

    // Set/Get stream timestamp in 1/NV_TIME_BASE unit
    virtual HRESULT __stdcall SetStreamTime(int64_t in_streamTime) = 0;
    virtual int64_t __stdcall GetStreamTime() = 0;

    // Set/Get Duration time in 1/NV_TIME_BASE unit
    virtual HRESULT __stdcall SetDurationTime(int64_t in_durationTime) = 0;
    virtual int64_t __stdcall GetDurationTime() = 0;

    // Set/Get meta data
    virtual void __stdcall SetMetaData(const char *key, const QVariant &value) = 0;
    virtual QVariant __stdcall GetMetaData(const char *key) = 0;

    // Set/Get custom object
    virtual HRESULT __stdcall SetCustomObject(const GUID &in_objId, IUnknown *in_pIUnk) = 0;
    virtual HRESULT __stdcall GetCustomObject(const GUID &in_objId, IUnknown **out_ppIUnk) = 0;
};


//
// Helper funtions to set/get predefined meta data of video frame
//
static inline void NvSetVideoFrameNonPremultiplied(INvVideoFrame *pIVideoFrame, bool nonPremultiplied)
{
    pIVideoFrame->SetMetaData(NV_VIDEOFRAME_META_KEY_NON_PREMULTIPLIED, nonPremultiplied);
}

static inline bool NvGetVideoFrameNonPremultiplied(INvVideoFrame *pIVideoFrame)
{
    return pIVideoFrame->GetMetaData(NV_VIDEOFRAME_META_KEY_NON_PREMULTIPLIED).toBool();
}

static inline void NvSetVideoFrameLuma601(INvVideoFrame *pIVideoFrame, bool luma601)
{
    pIVideoFrame->SetMetaData(NV_VIDEOFRAME_META_KEY_LUMA_601, luma601);
}

static inline bool NvGetVideoFrameLuma601(INvVideoFrame *pIVideoFrame)
{
    return pIVideoFrame->GetMetaData(NV_VIDEOFRAME_META_KEY_LUMA_601).toBool();
}

static inline void NvSetVideoFrameFullRangeYUV(INvVideoFrame *pIVideoFrame, bool fullRangeYUV)
{
    pIVideoFrame->SetMetaData(NV_VIDEOFRAME_META_KEY_FULL_RANGE_YUV, fullRangeYUV);
}

static inline bool NvGetVideoFrameFullRangeYUV(INvVideoFrame *pIVideoFrame)
{
    return pIVideoFrame->GetMetaData(NV_VIDEOFRAME_META_KEY_FULL_RANGE_YUV).toBool();
}

static inline void NvSetVideoFrameUpsideDownTexture(INvVideoFrame *pIVideoFrame, bool upsideDown)
{
    pIVideoFrame->SetMetaData(NV_VIDEOFRAME_META_KEY_UPSIDE_DOWN_TEXTURE, upsideDown);
}

static inline bool NvGetVideoFrameUpsideDownTexture(INvVideoFrame *pIVideoFrame)
{
    return pIVideoFrame->GetMetaData(NV_VIDEOFRAME_META_KEY_UPSIDE_DOWN_TEXTURE).toBool();
}


//
// INvVideoFrameTextureControl interface
//
interface INvVideoFrameTextureControl : public IUnknown
{
    virtual int __stdcall GetTextureInternalFormat() = 0;

    virtual QThread * __stdcall GetTextureThread() = 0;
};

