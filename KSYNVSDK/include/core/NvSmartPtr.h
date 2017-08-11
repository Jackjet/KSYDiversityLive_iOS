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

#include <qglobal.h>
#include <qhash.h>


//
// Smart pointer class
// NOTE: we don't implement an overloaded operator of &
// because we want to make it usable as key of QMap
//
template <class T>
class TNvSmartPtr
{
public:
    TNvSmartPtr() noexcept
    {
        m_ptr = nullptr;
    }

    TNvSmartPtr(T *ptr) noexcept
    {
        m_ptr = ptr;
        if (m_ptr)
            m_ptr->AddRef();
    }

    // Copy constructor
    TNvSmartPtr(const TNvSmartPtr<T> &smartPtr) noexcept
    {
        m_ptr = smartPtr.m_ptr;
        if (m_ptr)
            m_ptr->AddRef();
    }

    // Move constructor
    TNvSmartPtr(TNvSmartPtr<T> &&smartPtr) noexcept : m_ptr(smartPtr.m_ptr)
    {
        smartPtr.m_ptr = nullptr;
    }

    // NOTE: we don't use virtual destructor here,
    // We want to keep an identical ABI to a pure T * pointer
    ~TNvSmartPtr()
    {
        Release();
    }

    void Release()
    {
        if (m_ptr) {
            m_ptr->Release();
            m_ptr = nullptr;
        }
    }

    T ** GetPtrAddr()
    {
        Q_ASSERT(!m_ptr);
        return &m_ptr;
    }

    T * AssignNew(T *ptr)
    {
        Release();

        // Here we assign the pointer directly without adding a ref count
        m_ptr = ptr;
        return m_ptr;
    }

    operator T*() const
    {
        return m_ptr;
    }

    T * operator->() const
    {
        return m_ptr;
    }

    T * operator=(T *ptr)
    {
        if (ptr)
            ptr->AddRef();

        Release();
        m_ptr = ptr;
        return m_ptr;
    }

    T * operator=(const TNvSmartPtr<T> &smartPtr) noexcept
    {
        return operator=(smartPtr.m_ptr);
    }

    // Move assignment
    T * operator=(TNvSmartPtr<T> &&smartPtr) noexcept
    {
        std::swap(m_ptr, smartPtr.m_ptr);
        return m_ptr;
    }

    bool operator!() const
    {
        return m_ptr == nullptr;
    }

    bool operator==(T * ptr) const
    {
        return m_ptr == ptr;
    }

    bool operator<(const T *ptr) const
    {
        return m_ptr < ptr;
    }

    bool operator>(T * ptr) const
    {
        return m_ptr > ptr;
    }

    bool operator==(const TNvSmartPtr<T>& smartPtr) const
    {
        return operator==(smartPtr.m_ptr);
    }

    bool operator<(const TNvSmartPtr<T>& smartPtr) const
    {
        return operator<(smartPtr.m_ptr);
    }

    bool operator>(const TNvSmartPtr<T>& smartPtr) const
    {
        return operator>(smartPtr.m_ptr);
    }

protected:
    // Any class pointer which has AddRef() and Release() method
    T *m_ptr;
};


template <class T> inline uint qHash(const TNvSmartPtr<T> &sptr, uint seed = 0)
{
    return qHash((T *)sptr, seed);
}

