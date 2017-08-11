//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2016. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 14. 2016
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"
#include "NvDebug.h"

#include <QHash>
#include <QPair>


//
// Template class to implement a ref-counted hash table
//
template <class Key, class T>
class CNvRefCountedHash
{
public:
    CNvRefCountedHash();
    ~CNvRefCountedHash();

public:
    // Get object by the given key, if succeeded the object's reference count will be increased by 1
    // Return null if we can't find object with the given key
    T * Get(const Key &key);

    // Decrease object's reference count, delete it if its reference count reaches 0
    // Nothing will happen if we can't find object
    void Put(T *obj);

    // Add the object to the hash with the given key and set its reference count to 1
    // If the object already existed or there is aready an object with the same key
    // nothing will happen and the method will return false
    bool Add(const Key &key, T *obj);

private:
    QHash<Key, QPair<T *, int> > m_keyHashTable;
    QHash<T *, Key> m_objHashTable;
};

template <class Key, class T>
Q_INLINE_TEMPLATE CNvRefCountedHash<Key, T>::CNvRefCountedHash()
{

}

template <class Key, class T>
Q_INLINE_TEMPLATE CNvRefCountedHash<Key, T>::~CNvRefCountedHash()
{
    if (!m_keyHashTable.size())
        return;

    NvError() << m_keyHashTable.size() << " object(s) have not been destroyed yet!!!";

    typename QHash<Key, QPair<T *, int> >::const_iterator itr = m_keyHashTable.constBegin();
    while (itr != m_keyHashTable.constEnd()) {
        delete itr.value().first;
        ++itr;
    }

    m_keyHashTable.clear();
    m_objHashTable.clear();
}

template <class Key, class T>
T * CNvRefCountedHash<Key, T>::Get(const Key &key)
{
    typename QHash<Key, QPair<T *, int> >::iterator itr = m_keyHashTable.find(key);
    if (itr == m_keyHashTable.end())
        return 0;

    // Increase reference count
    ++(itr.value().second);
    return itr.value().first;
}

template <class Key, class T>
void CNvRefCountedHash<Key, T>::Put(T *obj)
{
    if (!obj) {
        NvError("Try to put null object!!!");
        return;
    }

    // Find object
    typename QHash<T *, Key>::iterator itrObj = m_objHashTable.find(obj);
    if (itrObj == m_objHashTable.end()) {
        NvError("Invalid object!!!");
        return;
    }

    typename QHash<Key, QPair<T *, int> >::iterator itr = m_keyHashTable.find(itrObj.value());
    if (itr == m_keyHashTable.end()) {
        NvError("Invalid key!!!");
        return;
    }

    if (!(--(itr.value().second))) {
        delete itr.value().first;
        m_keyHashTable.erase(itr);
        m_objHashTable.erase(itrObj);
    }
}

template <class Key, class T>
bool CNvRefCountedHash<Key, T>::Add(const Key &key, T *obj)
{
    if (!obj) {
        NvError("Try to add a null object!");
        return false;
    }

    if (m_keyHashTable.constFind(key) != m_keyHashTable.constEnd()) {
        NvError("Try to add an object whose key already existed!");
        return false;
    }

    if (m_objHashTable.constFind(obj) != m_objHashTable.constEnd()) {
        NvError("Try to add an object which already existed!");
        return false;
    }

    m_keyHashTable.insert(key, QPair<T *, int>(obj, 1));
    m_objHashTable.insert(obj, key);
    return true;
}

