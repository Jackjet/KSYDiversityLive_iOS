//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2015. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 20. 2015
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <qglobal.h>

#if defined(Q_OS_WIN) || defined(Q_OS_OSX)
#define NV_HAS_LIBJPEG_TURBO
#elif defined(Q_OS_ANDROID) && defined(Q_PROCESSOR_ARM)
#define NV_HAS_LIBJPEG_TURBO
#endif

#ifdef NV_HAS_LIBJPEG_TURBO

#include <INvVideoFrame.h>
#include <QString>
#include <turbojpeg.h>

class CNvJpegTurboReader
{
public:
    CNvJpegTurboReader();
    ~CNvJpegTurboReader();

public:
    void ReadFrame(const QString &jpegFilePath, INvVideoFrame **ppIVideoFrame);
    void ReadFrame(const QByteArray &imageData, INvVideoFrame **ppIVideoFrame);

private:
    tjhandle m_tjDecoder;
    void *m_tjTmpBuffer;
    unsigned int m_tjTmpBufferSize;
};

#endif

