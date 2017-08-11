//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 26. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <QThread>
#include <QEvent>
#include <QWaitCondition>
#include <QMutex>
#include <QVector>
#include <QElapsedTimer>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#elif defined(Q_OS_ANDROID)
#include <time.h>
#include <pthread.h>
#endif


// Help funtion to get processor count configured in the system
unsigned int NvGetConfiguredProcessorCount();

//
// Class to simulate a win32 event sync object
//
class CNvSyncEvent
{
public:
    CNvSyncEvent(bool manualReset = false, bool initialState = false);
    ~CNvSyncEvent();

public:
    bool Wait(unsigned long time = ULONG_MAX);
    void Set();
    void Reset();

private:
#ifdef Q_OS_WIN
    HANDLE m_event;
#else
    bool m_manualReset;
    volatile bool m_signaled;

#ifndef Q_OS_ANDROID
    QWaitCondition m_cond;
    QMutex m_mutex;
    QElapsedTimer m_timer;
#else
    // NOTE: due to a bug of Qt on android 5.0+,
    // We have to implment CNvSyncEvent using pthread_cond_xxx directly
    pthread_cond_t m_cond;
    pthread_mutex_t m_mutex;
#endif // Q_OS_ANDROID
#endif // Q_OS_WIN
};


//
// Class to implement a reply QEvent
//
class CNvReplyEvent : public QEvent
{
public:
    CNvReplyEvent(QEvent::Type type, CNvSyncEvent *syncEvent);
    ~CNvReplyEvent();

public:
    void Reply();
    CNvSyncEvent * SyncEvent() const { return m_syncEvent; }

private:
    CNvSyncEvent *m_syncEvent;
};


//
// Thread synchronization manager
//
class CNvThreadSyncManager
{
public:
    CNvThreadSyncManager();
    ~CNvThreadSyncManager();

public:
    bool Initialize(unsigned int threadCount);
    void Sync(unsigned int threadIndex);

private:
    unsigned int m_threadCount;
    QVector<int> m_vecFinishFlag;
    QVector<int> m_vecNextWorkFlag;

    QWaitCondition m_finishWorkCond;
    QWaitCondition m_nextWorkCond;
    QMutex m_mutex;
};

