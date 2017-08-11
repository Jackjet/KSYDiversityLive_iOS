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

#include "NvLightUnknown.h"
#include "INvAudioSamples.h"
#include "NvSmartPtr.h"
#include <QReadWriteLock>
#include <QMap>
#include <QVariantMap>


class CNvBaseAudioSamples : public CNvLightUnknown,
                            public INvAudioSamples
{
public:
    CNvBaseAudioSamples(const wchar_t *in_objName = 0);
    ~CNvBaseAudioSamples();

public:
    NV_DECLARE_IUNKNOWN

    virtual HRESULT __stdcall NonDelegatingQueryInterface(REFIID in_rIID, void **out_ppv);

public:
    virtual ENvAudioSampleFormat __stdcall GetAudioSampleFormat();

    virtual unsigned int __stdcall GetSampleRate();

    virtual unsigned int __stdcall GetMaxSampleCount();

    virtual unsigned int __stdcall GetChannelCount();

    virtual HRESULT __stdcall SetActualSampleCount(unsigned int in_sampleCount);
    virtual unsigned int __stdcall GetActualSampleCount();

    virtual HRESULT __stdcall SetStreamTime(int64_t in_streamTime);
    virtual int64_t __stdcall GetStreamTime();

    virtual void __stdcall SetMetaData(const char *key, const QVariant &value);
    virtual QVariant __stdcall GetMetaData(const char *key);

    virtual HRESULT __stdcall SetCustomObject(const GUID &in_objId, IUnknown *in_pIUnk);
    virtual HRESULT __stdcall GetCustomObject(const GUID &in_objId, IUnknown **out_ppIUnk);

protected:
    ENvAudioSampleFormat m_audSmpFmt;
    unsigned int m_sampleRate;
    unsigned int m_maxSampleCount;
    unsigned int m_channelCount;

protected:
    unsigned int m_actualSampleCount;
    int64_t m_streamTime;

    QMap<QLatin1String, QVariant> m_mapMetaData;
    QMap<GUID, TNvSmartPtr<IUnknown> > m_mapCustomObject;

    QReadWriteLock m_lock;
};

