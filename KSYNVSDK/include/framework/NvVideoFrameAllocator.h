//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 17. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvFrameworkDef.h"
#include <INvVideoFrameAllocator.h>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QImage>


HRESULT NvCreateHostVideoFrameAllocator(const wchar_t *in_allocatorName, // Can be null
                                        INvVideoFrameAllocator **out_ppIAllocator);

// Create an OpenGL video frame allocator
// NOTE: OpenGL video frame allocator has a strong thread affinity to the thread it was created,
// you can only call AllocateVideoFrame() ,Cleanup() and Shutdown() method in the allocator's thread!
HRESULT NvCreateOpenGLVideoFrameAllocator(const wchar_t *in_allocatorName, // Can be null
                                          uint64_t in_maxPoolSizeInBytes,
                                          INvVideoFrameAllocator **out_ppIAllocator);


// Create a host video frame from a QImage object
// NOTE: the video frame created from QImage is read only!
HRESULT NvCreateVideoFrameFromQImage(const QImage &img, INvVideoFrame **out_ppIVideoFrame);

// Create a host video frame from a QImage object with a given display rotation
// NOTE: the video frame created from QImage is read only!
HRESULT NvCreateVideoFrameFromQImageEx(const QImage &img,
                                       ENvVideoRotation displayRotation,
                                       INvVideoFrame **out_ppIVideoFrame);

