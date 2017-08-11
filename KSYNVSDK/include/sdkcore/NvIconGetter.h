//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2017. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 20. 2017
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <NvDef.h>

#include <QObject>
#include <QImage>
#include <QAtomicInt>

#ifdef Q_OS_IOS
Q_FORWARD_DECLARE_OBJC_CLASS(UIImage);
#elif defined(Q_OS_ANDROID)
#include <QAndroidJniObject>
#endif


class CNvUiImage;

class CNvIconGetter : public QObject
{
    Q_OBJECT

public:
    CNvIconGetter(QObject *parent = nullptr);
    ~CNvIconGetter();

signals:
    void thumbnailArrived(CNvUiImage *thumbnail);
    void iconArrived(CNvUiImage *icon);
};


class CNvUiImage
{
public:
#if defined(Q_OS_WIN) || defined(Q_OS_OSX)
    CNvUiImage(const QImage &image,
               ENvVideoRotation displayRotation,
               int64_t timestamp);
#elif defined(Q_OS_IOS)
    CNvUiImage(UIImage *image, int64_t timestamp);
#elif defined(Q_OS_ANDROID)
    CNvUiImage(const QAndroidJniObject &image,
               ENvVideoRotation displayRotation,
               int64_t timestamp);
#endif

    ~CNvUiImage();

public:
#if defined(Q_OS_WIN) || defined(Q_OS_OSX)
    const QImage & Image() const { return m_image; }
#elif defined(Q_OS_IOS)
    UIImage * Image() { return m_image; }
#elif defined(Q_OS_ANDROID)
    const QAndroidJniObject & Image() const { return m_image; }
#endif

    ENvVideoRotation DisplayRotation() const { return m_displayRotation; }

    void SetTimestamp(int64_t timestamp) { m_timestamp = timestamp; }
    int64_t Timestamp() const { return m_timestamp; }

    // Caller will retain a reference count of the cloned CNvUiImage object!
    void cloneWithTimestamp(int64_t timestamp, CNvUiImage **uiImage);

public:
    void AddRef();
    void Release();

private:
#if defined(Q_OS_WIN) || defined(Q_OS_OSX)
    QImage m_image;
#elif defined(Q_OS_IOS)
    UIImage *m_image;
#elif defined(Q_OS_ANDROID)
    QAndroidJniObject m_image;
#endif

    ENvVideoRotation m_displayRotation;
    int64_t m_timestamp;

private:
    QAtomicInt m_refCount;
};

