//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2017. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 13. 2017
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <QObject>
#include <QEvent>
#include <QMutex>


#ifdef Q_OS_ANDROID
class CNvAndroidEventNotifier;
#endif

class CNvPlatformEventHandler
#ifndef Q_OS_ANDROID
        : public QObject
#endif
{
public:
    CNvPlatformEventHandler();
    virtual ~CNvPlatformEventHandler();

public:
    void PostEvent(QEvent *e, int priority = Qt::NormalEventPriority);
    void SendPostedEvent(int eventType = 0);

protected:
    virtual void ProcessEvent(QEvent *) {}

protected:
#ifndef Q_OS_ANDROID
    virtual void customEvent(QEvent *);
#endif

private:
#ifdef Q_OS_ANDROID
    QMutex m_eventLock;
    QList<QEvent *> m_eventList;
    CNvAndroidEventNotifier *m_androidEventNotifier;
#endif
};

