//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2017. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jun 16. 2017
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvProjTimelineFilter.h"


class CNvProjTimeline;

class CNvProjTimelineVideoFx : public CNvProjTimelineFilter
{
    friend class CNvProjTimeline;

public:
    enum VideoFxType
    {
        BuiltinVideoFx = 0,
        PackagedVideoFx,
        CustomVideoFx
    };

protected:
    CNvProjTimelineVideoFx(CNvProjTimeline *ownerTimeline,
                           CNvAppFxInstance *fxInstance,
                           VideoFxType videoFxType,
                           CNvProjContext *projContext);
    ~CNvProjTimelineVideoFx();

public:
    VideoFxType GetVideoFxType() const { return m_videoFxType; }
    QString GetPackageId() const { return m_packageId; }

protected:
    virtual void Destroy();

protected:
    VideoFxType m_videoFxType;
    QString m_packageId;
    TNvSmartPtr<INvCustomGpuVideoEffect> m_pJCustomGpuVideoEffect;
};

