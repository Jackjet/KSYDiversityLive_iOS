//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 16. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvLightUnknown.h"
#include "INvVideoFrame.h"
#include "NvSmartPtr.h"
#include <QReadWriteLock>
#include <QMap>
#include <QVariantMap>


class CNvBaseVideoFrame : public CNvLightUnknown,
                          public INvVideoFrame
{
public:
    CNvBaseVideoFrame(const wchar_t *in_objName = 0);
    ~CNvBaseVideoFrame();

public:
    NV_DECLARE_IUNKNOWN

    virtual HRESULT __stdcall NonDelegatingQueryInterface(REFIID in_rIID, void **out_ppv);

public:
    // Get pixel format of the video frame
    virtual ENvPixelFormat __stdcall GetPixelFormat();

    // Get memory location of the video frame
    virtual ENvMemoryLocation __stdcall GetMemoryLocation();

    // Get image position of the video frame
    virtual HRESULT __stdcall GetImagePosition(SNvImagePosition *out_imgPos);

    // Get pixel aspect ratio of the video frame
    virtual HRESULT __stdcall GetPixelAspectRatio(SNvRational *out_pixelAspectRatio);

    // Get proxy scale of the video frame
    virtual HRESULT __stdcall GetProxyScale(SNvRational *out_proxyScale);

    virtual HRESULT __stdcall SetDisplayRotation(ENvVideoRotation in_videoRotation);
    virtual ENvVideoRotation __stdcall DisplayRotation();

    virtual const QImage * __stdcall GetInternalQImage();

    virtual HRESULT __stdcall SetStreamTime(int64_t in_streamTime);
    virtual int64_t __stdcall GetStreamTime();

    // Set/Get Duration time in 1/NV_TIME_BASE unit
    virtual HRESULT __stdcall SetDurationTime(int64_t in_durationTime);
    virtual int64_t __stdcall GetDurationTime();

    // Set/Get meta data
    virtual void __stdcall SetMetaData(const char *key, const QVariant &value);
    virtual QVariant __stdcall GetMetaData(const char *key);

    // Set/Get custom object
    virtual HRESULT __stdcall SetCustomObject(const GUID &in_objId, IUnknown *in_pIUnk);
    virtual HRESULT __stdcall GetCustomObject(const GUID &in_objId, IUnknown **out_ppIUnk);

protected:
    ENvPixelFormat m_pixFmt;
    ENvMemoryLocation m_memLocation;
    SNvImagePosition m_imgPos;      // In current proxy level
    SNvRational m_pixelAspectRatio;
    SNvRational m_proxyScale;

protected:
    ENvVideoRotation m_displayRotation;
    int64_t m_streamTime;
    int64_t m_duration;

    QMap<QLatin1String, QVariant> m_mapMetaData;
    QMap<GUID, TNvSmartPtr<IUnknown> > m_mapCustomObject;

    QReadWriteLock m_lock;
};

