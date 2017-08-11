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


class CNvProjAudioClip;

class CNvProjAudioFx : public CNvProjClipFilter
{
    friend class CNvProjAudioClip;

protected:
    CNvProjAudioFx(CNvProjClip *ownerClip,
                   CNvAppFxInstance *fxInstance,
                   CNvProjContext *projContext);
    ~CNvProjAudioFx();

protected:
    virtual void Destroy();
};

