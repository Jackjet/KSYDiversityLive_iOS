//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2015. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jul 30. 2015
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"
#include "NvListNode.h"
#include "NvDebug.h"

#include <QHash>


//
// Template class to implement a hash table whose object was controlled by a LRU list
//
template <class Key, class T>
class CNvLruHash
{
public:
    CNvLruHash(int maxObject);
    ~CNvLruHash();

public:
    // If acquired successfully, the object will be moved to the head of the LRU list
    T * Acquire(const Key &key);

    // CNvLruHash will take the ownership of the object T
    // If there is aready an object with the same key, the old one will be destroyed
    // If the object count surpass MaxObject, it will remove the least recently used object
    void Insert(const Key &key, T *obj);

    bool Remove(const Key &key);

    void Clear();

private:
    int m_maxObject;

    struct SNvLruHashData {
        Key key;
        T *obj;
        SNvListNode lruListNode;
    };

    QHash<Key, SNvLruHashData *> m_hashTable;
    SNvListNode m_lruListHead;

private:
    void Reclaim();
};

template <class Key, class T>
Q_INLINE_TEMPLATE CNvLruHash<Key, T>::CNvLruHash(int maxObject)
{
    m_maxObject = qMax(maxObject, 1);
    NvInitListNode(&m_lruListHead);
}

template <class Key, class T>
Q_INLINE_TEMPLATE CNvLruHash<Key, T>::~CNvLruHash()
{
    Clear();
}

template <class Key, class T>
T * CNvLruHash<Key, T>::Acquire(const Key &key)
{
    typename QHash<Key, SNvLruHashData *>::const_iterator itr = m_hashTable.constFind(key);
    if (itr == m_hashTable.constEnd())
        return 0;

    // Cache hit, move the object to the head of the LRU list
    SNvLruHashData *data = itr.value();
    NvListDelete(&data->lruListNode);
    NvListAddAfter(&m_lruListHead, &data->lruListNode);
    return data->obj;
}

template <class Key, class T>
void CNvLruHash<Key, T>::Insert(const Key &key, T *obj)
{
    if (!obj) {
        NvError("Try to insert a null object!");
        return;
    }

    typename QHash<Key, SNvLruHashData *>::iterator itr = m_hashTable.find(key);
    if (itr != m_hashTable.end()) {
        // Remove the old object with the same key
        SNvLruHashData *data = itr.value();
        m_hashTable.erase(itr);
        NvListDelete(&data->lruListNode);
        delete data->obj;
        delete data;
    }

    SNvLruHashData *data = new SNvLruHashData;
    data->key = key;
    data->obj = obj;
    // Insert this new object to the head of the LRU list
    NvListAddAfter(&m_lruListHead, &data->lruListNode);
    // Insert it to hash table
    m_hashTable.insert(key, data);

    if (m_hashTable.size() > m_maxObject)
        Reclaim();
}

template <class Key, class T>
bool CNvLruHash<Key, T>::Remove(const Key &key)
{
    typename QHash<Key, SNvLruHashData *>::iterator itr = m_hashTable.find(key);
    if (itr == m_hashTable.end())
        return false;

    SNvLruHashData *data = itr.value();
    m_hashTable.erase(itr);
    NvListDelete(&data->lruListNode);
    delete data->obj;
    delete data;
    return true;
}

template <class Key, class T>
void CNvLruHash<Key, T>::Clear()
{
    foreach (SNvLruHashData *data, m_hashTable) {
        delete data->obj;
        delete data;
    }
    m_hashTable.clear();

    NvInitListNode(&m_lruListHead);
}

template <class Key, class T>
void CNvLruHash<Key, T>::Reclaim()
{
    if (m_hashTable.size() <= m_maxObject)
        return;

    // Object count has surpass its limit, reclaim some objects from the tail of LRU list
    SNvListNode *listNode = m_lruListHead.prev;
    while (listNode != &m_lruListHead && m_hashTable.size() > m_maxObject) {
        SNvLruHashData *data = NV_LIST_ENTRY(listNode, SNvLruHashData, lruListNode);
        listNode = listNode->prev;

        typename QHash<Key, SNvLruHashData *>::iterator itr = m_hashTable.find(data->key);
        if (itr != m_hashTable.end()) {
            m_hashTable.erase(itr);
        } else {
            Q_ASSERT(false);
        }

        NvListDelete(&data->lruListNode);
        delete data->obj;
        delete data;
    }
}

