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

#include "NvProjAppFx.h"


class CNvProjClip;

class CNvProjClipFilter : public CNvProjAppFx
{
    friend class CNvProjClip;

public:
    enum ClipFilterType
    {
        VideoFx,
        AudioFx
    };

protected:
    CNvProjClipFilter(CNvProjClip *ownerClip,
                      ClipFilterType type,
                      CNvAppFxInstance *fxInstance,
                      CNvProjContext *projContext,
                      ProjObjectType projObjType);
    ~CNvProjClipFilter();

public:
    CNvProjClip * OwnerClip() const { return m_ownerClip; }
    ClipFilterType GetClipFilterType() const { return m_type; }
    unsigned int FilterIndex() const { return m_filterIndex; }

protected:
    virtual void Destroy();

protected:
    CNvProjClip *m_ownerClip;
    ClipFilterType m_type;

    unsigned int m_filterIndex;
};

