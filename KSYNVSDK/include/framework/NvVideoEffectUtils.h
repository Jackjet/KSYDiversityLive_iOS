//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Mar 21. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvFrameworkDef.h"
#include <INvVideoFrameAllocator.h>
#include <NvVideoEffectDef.h>
#include <NvLightUnknown.h>
#include <NvSmartPtr.h>

#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QMutex>


class CNvHardwareInfo;

class CNvVideoEffectContext
{
public:
    CNvVideoEffectContext();
    ~CNvVideoEffectContext();

public:
    bool NeedEglSyncWorkaround() const { return m_needEglSyncWorkaround; }
    GLuint DummyTextureId() const { return m_dummyTexture; }

protected:
    bool InitVideoEffectContext(QOpenGLContext *sharedCtx,
                                QOffscreenSurface *fakeSurface,
                                uint64_t maxGPUVideoFramePoolSize,
                                CNvHardwareInfo *hardwareInfo);

    bool InitVideoEffectContext2(QOpenGLContext *glCtx,
                                 QOffscreenSurface *fakeSurface,
                                 uint64_t maxGPUVideoFramePoolSize,
                                 CNvHardwareInfo *hardwareInfo);

    void CleanupVideoEffectContext();

    bool IsValidVideoEffectContext() const;

    void ReclaimVideoEffectContextResources();

private:
    bool DoInitVideoEffectContext(QOpenGLContext *glCtx,
                                  bool createGLContext,
                                  QOffscreenSurface *fakeSurface,
                                  uint64_t maxGPUVideoFramePoolSize,
                                  CNvHardwareInfo *hardwareInfo);
protected:
    QOpenGLContext *m_glCtx;
    QOffscreenSurface *m_fakeSurface;
    bool m_ownFakeSurface;
    GLuint m_fbo;
    GLuint m_readFbo;

    TNvSmartPtr<INvVideoFrameAllocator> m_pJHostVideoFrameAllocator;
    TNvSmartPtr<INvVideoFrameAllocator> m_pJGPUVideoFrameAllocator;
    TNvSmartPtr<INvVideoEffectResourceManager> m_pJVideoEffectResourceManager;

    bool m_needEglSyncWorkaround;
    GLuint m_dummyTexture;
};


///////////////////////////////////////////////////////////////////////////////////
// Base class to implement custom video GPU effect
///////////////////////////////////////////////////////////////////////////////////
class CNvCustomGpuVideoEffectCleaner;

class CNvBaseCustomGpuVideoEffect : public CNvLightUnknown,
                                    public INvCustomGpuVideoEffect
{
    friend class CNvCustomGpuVideoEffectCleaner;

public:
    CNvBaseCustomGpuVideoEffect(const wchar_t *in_objName = nullptr);
    ~CNvBaseCustomGpuVideoEffect();

public:
    NV_DECLARE_IUNKNOWN

    virtual HRESULT __stdcall NonDelegatingQueryInterface(REFIID in_rIID, void **out_ppv);

public:
    virtual void __stdcall PreloadResources();

    virtual void __stdcall Render(INvVideoFrame *in_pIInputFrame,
                                  INvVideoFrame *io_pIOutputFrame,
                                  INvCustomGpuVideoEffectCallback *in_pICallback,
                                  int64_t in_effectTime,
                                  int64_t in_effectStartTime,
                                  int64_t in_effectEndTime);

    virtual void __stdcall Cleanup();

protected:
    virtual void OnInit() {}
    virtual void OnCleanup() {}

    virtual void OnPreloadResources() {}

    virtual void OnRender(INvVideoFrame *pIInputFrame,
                          INvVideoFrame *pIOutputFrame,
                          INvCustomGpuVideoEffectCallback *pICallback,
                          int64_t effectTime,
                          int64_t effectStartTime,
                          int64_t effectEndTime) = 0;

private:
    bool m_initialized;

    QMutex m_mutex;
    CNvCustomGpuVideoEffectCleaner *m_cleaner;

private:
    void Init();
};

