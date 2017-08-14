//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2015. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 24. 2015
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <INvVideoFrame.h>
#include <NvSmartPtr.h>


//
// INvGLSyncObject interface
//
interface INvGLSyncObject : public IUnknown
{
    // Wait sync object (prefer server side waiting)
    // @timeout: timeout time, in nanoseconds, -1 to wait infinitely
    // Return false indicate timeout
    virtual bool __stdcall Wait(uint64_t timeout) = 0;

    // Wait sync object (wait at client side)
    // @timeout: timeout time, in nanoseconds, -1 to wait infinitely
    // Return false indicate timeout
    virtual bool __stdcall ClientWait(uint64_t timeout) = 0;

    virtual void __stdcall DestorySyncObject() = 0;
};


bool NvInitGLSync();

void NvGLCreateSyncObject(INvGLSyncObject **ppIGlSyncObject, bool flush = true);

void NvGLCreateSyncObjectForVideoFrame(INvVideoFrame *pIVideoFrame);
void NvGLCreateSyncObjectForVideoFrame2(INvVideoFrame *pIVideoFrame1, INvVideoFrame *pIVideoFrame2);
void NvGLCreateSyncObjectForVideoFrameArray(const QVector<TNvSmartPtr<INvVideoFrame> > &videoFrameArray);

void NvGLWaitSyncObjectForVideoFrame(INvVideoFrame *pIVideoFrame, bool forceClientWait = false);

void NvGLForceFlush();

