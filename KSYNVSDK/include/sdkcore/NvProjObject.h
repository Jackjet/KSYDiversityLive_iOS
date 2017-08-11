//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2016. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Dec 13. 2016
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvProjDef.h"
#include <NvAppFx.h>

#include <QAtomicInt>


class CNvProjContext;
class CNvProjTimeline;

class CNvProjObject
{
public:
    enum ProjObjectType
    {
        ProjObjectTypeTimeline = 0,
        ProjObjectTypeVideoTrack,
        ProjObjectTypeAudioTrack,
        ProjObjectTypeVideoClip,
        ProjObjectTypeAudioClip,
        ProjObjectTypeVideoFx,
        ProjObjectTypeAudioFx,
        ProjObjectTypeVideoTransition,
        ProjObjectTypeAudioTransition,
        ProjObjectTypeCaptureVideoFx,
        ProjObjectTypeTimelineCaption,
        ProjObjectTypeTimelineAnimatedSticker,
        ProjObjectTypeTimelineVideoFx,
        ProjObjectTypeCount
    };

protected:
    CNvProjObject(CNvProjContext *projContext, ProjObjectType projObjType);
    virtual ~CNvProjObject();

public:
    bool IsValid() const { return m_projContext != nullptr; }
    ProjObjectType ProjObjType() const { return m_projObjType; }

public:
    void AddRef();
    void Release();

protected:
    void InvalidateTimelineFromEngine(CNvProjTimeline *timeline);

    // NOTE: The caller will obtain a reference count of the created video transition
    CNvAppFxInstance * CreateDefaultVideoTransitionFxInstance();
    CNvAppFxInstance * CreateDefaultAudioTransitionFxInstance();

    // NOTE: The caller will obtain a reference count of the created AppFx instance
    CNvAppFxInstance * CreateVideoAppFxInstance(const char *appFxName);
    CNvAppFxInstance * CreateAudioAppFxInstance(const char *appFxName);

protected:
    // Use this method to release internal resource of this project object
    // and unlink it from its parent and children
    virtual void Destroy();

private:
    QAtomicInt m_refCount;

protected:
    CNvProjContext *m_projContext;
    ProjObjectType m_projObjType;
};

