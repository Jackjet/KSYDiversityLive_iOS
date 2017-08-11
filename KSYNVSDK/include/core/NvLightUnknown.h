//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 6. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "IUnknown.h"
#include <QAtomicInt>
#include <QString>


//
// Macro used to implement IUnknown interface
//
#define NV_DECLARE_IUNKNOWN \
    inline virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv) { \
        return NonDelegatingQueryInterface(riid,ppv);                          \
    };                                                                         \
    inline virtual unsigned long __stdcall AddRef() {                          \
        return NonDelegatingAddRef();                                          \
    };                                                                         \
    inline virtual unsigned long __stdcall Release() {                         \
        return NonDelegatingRelease();                                         \
    };

static inline HRESULT NvGetInterface(IUnknown *in_pIUnk, void **out_ppv)
{
    if (!out_ppv)
        return NV_E_INVALID_POINTER;

    *out_ppv = in_pIUnk;
    in_pIUnk->AddRef();
    return NV_NOERROR;
}


interface INvNonDelegatingUnknown
{
    virtual HRESULT __stdcall NonDelegatingQueryInterface(REFIID, void **) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};


class CNvLightUnknown : public INvNonDelegatingUnknown
{
public:
    CNvLightUnknown(const wchar_t *in_objName = 0);

protected:
    virtual ~CNvLightUnknown();

public:
    virtual HRESULT __stdcall NonDelegatingQueryInterface(REFIID in_rIID, void **out_ppv);
    virtual unsigned long __stdcall NonDelegatingAddRef();
    virtual unsigned long __stdcall NonDelegatingRelease();

protected:
    QAtomicInt m_refCount;
#ifndef QT_NO_DEBUG
    QString m_objName;
#endif
};

