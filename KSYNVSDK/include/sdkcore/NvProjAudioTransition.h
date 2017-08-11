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
//   Author:        NewAuto Video team
//================================================================================
#pragma once

#include "NvProjTransition.h"


class CNvProjTrack;

class CNvProjAudioTransition : public CNvProjTransition
{
    friend class CNvProjTrack;

protected:
    CNvProjAudioTransition(CNvProjTrack *ownerTrack,
                           unsigned int srcClipIndex,
                           CNvAppFxInstance *fxInstance,
                           CNvProjContext *projContext);
    ~CNvProjAudioTransition();

protected:
    virtual void Destroy();
};

