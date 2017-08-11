//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2017. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 18. 2017
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvProjAppFx.h"


class CNvStreamingContext;

class CNvProjCaptureVideoFx : public CNvProjAppFx
{
    friend class CNvStreamingContext;

public:
    enum VideoFxType
    {
        BuiltinVideoFx = 0,
        PackagedVideoFx,
        CustomVideoFx
    };

protected:
    CNvProjCaptureVideoFx(VideoFxType videoFxType,
                          CNvAppFxInstance *fxInstance,
                          CNvProjContext *projContext);
    ~CNvProjCaptureVideoFx();

public:
    VideoFxType GetVideoFxType() const { return m_videoFxType; }
    QString VideoFxPakcageId() const { return m_videoFxPackageId; }
    unsigned int VideoFxIndex() const { return m_videoFxIndex; }

protected:
    virtual void Destroy();

protected:
    VideoFxType m_videoFxType;
    QString m_videoFxPackageId;
    TNvSmartPtr<INvCustomGpuVideoEffect> m_pJCustomGpuVideoEffect;

    unsigned int m_videoFxIndex;
};

