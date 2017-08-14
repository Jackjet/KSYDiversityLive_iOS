//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2017. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    May 13. 2017
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvJpegTurboReader.h"
#include <INvVideoFrameAllocator.h>
#include <INvVideoFrame.h>
#include <NvSmartPtr.h>

#ifdef Q_OS_IOS
#define NV_HAS_LIBJPEG
#endif


class CNvJpegReader
{
public:
    CNvJpegReader();
    ~CNvJpegReader();

public:
    void ReadFrame(const QString &jpegFilePath, INvVideoFrame **ppIVideoFrame);
    void ReadFrame(const QByteArray &imageData, INvVideoFrame **ppIVideoFrame);

private:
    TNvSmartPtr<INvVideoFrameAllocator> m_pJHostVideoFrameAllocator;

#if defined(NV_HAS_LIBJPEG_TURBO)
    CNvJpegTurboReader m_jpegTurboReader;
#endif

private:
#ifdef NV_HAS_LIBJPEG
    void ReadFrameByLibJpeg(const QString &jpegFilePath, INvVideoFrame **ppIVideoFrame);
    void ReadFrameByLibJpeg(const QByteArray &jpegImageData, INvVideoFrame **ppIVideoFrame);
#endif
};

