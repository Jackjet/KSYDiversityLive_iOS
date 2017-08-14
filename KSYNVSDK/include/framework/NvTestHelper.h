//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 13. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvFrameworkDef.h"
#include <NvVideoEffectDef.h>
#include <INvVideoFrame.h>
#include <QImage>


// Create a QImage object from a video frame
// NOTE: now we can only create QImage from a RGBA8, BGRA8(in little endian machine) or ARGB8(in big endian machine) host video frame!
QImage NvTestHelper_CreateQImageFromVideoFrame(INvVideoFrame *pIVideoFrame);

QImage NvTestHelper_CreateQImageFromVideoFrameEx(INvVideoFrameAllocator *pIHostVideoFrameAllocator,
                                                 INvVideoFrame *pIVideoFrame);

bool NvTestHelper_SaveVideoFrameToDisk(INvVideoFrameAllocator *pIHostVideoFrameAllocator,
                                       INvVideoFrame *pIVideoFrame,
                                       const QString &fileName,
                                       QString *savedFilePath = nullptr);

