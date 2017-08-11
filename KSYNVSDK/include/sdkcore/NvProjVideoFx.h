//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2016. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Dec 16. 2016
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvProjClipFilter.h"


class CNvProjClip;
class CNvProjVideoClip;

class CNvProjVideoFx : public CNvProjClipFilter
{
    friend class CNvProjClip;
    friend class CNvProjVideoClip;

public:
    enum VideoFxType
    {
        BuiltinVideoFx = 0,
        PackagedVideoFx,
        CustomVideoFx
    };

protected:
    CNvProjVideoFx(CNvProjClip *ownerClip,
                   VideoFxType videoFxType,
                   CNvAppFxInstance *fxInstance,
                   CNvProjContext *projContext);
    ~CNvProjVideoFx();

public:
    VideoFxType GetVideoFxType() const { return m_videoFxType; }
    QString GetPackageId() const { return m_videoFxPackageId; }

protected:
    virtual void Destroy();

protected:
    VideoFxType m_videoFxType;
    QString m_videoFxPackageId;
    TNvSmartPtr<INvCustomGpuVideoEffect> m_pJCustomGpuVideoEffect;
};

