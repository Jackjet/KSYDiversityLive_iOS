//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 7. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "INvPoolManager.h"
#include "NvLightUnknown.h"
#include <QAtomicInt>
#include <QMutexLocker>
#include <QLinkedList>
#include <new>


//
// Base class of a object class which is managable by pool manager
//
class CNvBasePoolManager;

class CNvPoolElement : public CNvLightUnknown
{
    friend class CNvBasePoolManager;

protected:
    virtual ~CNvPoolElement();

public:
    CNvPoolElement();

public:
    unsigned long __stdcall NonDelegatingRelease();

protected:
    virtual void OnReturnToPool();

private:
    void SetPoolManager(CNvBasePoolManager *in_poolManager);

private:
    CNvBasePoolManager *m_poolManager;
};

//
// Base class for pool manager
//
class CNvBasePoolManager : public CNvLightUnknown,
                           public INvPoolManager
{
    friend class CNvPoolElement;

public:
    CNvBasePoolManager(const wchar_t *in_poolName = 0);
    ~CNvBasePoolManager();

public:
    NV_DECLARE_IUNKNOWN

    virtual HRESULT __stdcall NonDelegatingQueryInterface(REFIID in_rIID, void **out_ppv);

public:
    virtual HRESULT __stdcall GetElement(IUnknown **out_ppIElement);

protected:
    void ReturnElement(CNvPoolElement *in_element);
    void AddElements(unsigned int count);

private:
    virtual CNvPoolElement * CreateElement(HRESULT &hr) = 0;

private:
    QLinkedList<CNvPoolElement *> m_elements;
    QMutex m_lock;
#ifndef QT_NO_DEBUG
    QAtomicInt m_usedElements; // Element count which have leaved from pool
#endif
};


//
// Template class to implement pool manager for a specific object class
//
template<class T>
class TNvPoolManager : public CNvBasePoolManager
{
public:
    TNvPoolManager(const wchar_t *in_poolName = 0) : CNvBasePoolManager(in_poolName)
    {

    }

    ~TNvPoolManager()
    {

    }

public:
    static HRESULT CreatePoolManager(const wchar_t *in_poolName,
                                     unsigned int in_initialElementCount,
                                     INvPoolManager **out_ppIPoolManager)
    {
        if (!out_ppIPoolManager) {
            Q_ASSERT(false);
            return NV_E_INVALID_POINTER;
        }

        TNvPoolManager<T> *poolManager = new(std::nothrow) TNvPoolManager<T>(in_poolName);
        if (!poolManager) {
            *out_ppIPoolManager = 0;
            return NV_E_OUT_OF_MEMORY;
        }

        poolManager->AddElements(in_initialElementCount);

        *out_ppIPoolManager = static_cast<INvPoolManager *>(poolManager);
        return NV_NOERROR;
    }

private:
    virtual CNvPoolElement * CreateElement(HRESULT &hr)
    {
        T *element = new(std::nothrow) T;
        if (!element) {
            hr = NV_E_OUT_OF_MEMORY;
            return 0; // Out of memory!
        }

        hr = NV_NOERROR;
        return static_cast<CNvPoolElement *>(element);
    }
};

