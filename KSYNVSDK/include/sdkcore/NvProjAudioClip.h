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

#include "NvProjClip.h"
#include "NvProjAudioFx.h"


class CNvProjTrack;

class CNvProjAudioClip : public CNvProjClip
{
    friend class CNvProjTrack;

protected:
    CNvProjAudioClip(CNvProjTrack *track, CNvProjContext *projContext);
    ~CNvProjAudioClip();

public:
    // NOTE: The caller will obtain a reference count of the created audio FX
    bool AppendAudioFx(const QByteArray &audioFxName,
                       CNvProjAudioFx **audioFx = nullptr);

    // NOTE: The caller will obtain a reference count of the created audio FX
    bool InsertAudioFx(const QByteArray &audioFxName,
                       unsigned int insertPosition,
                       CNvProjAudioFx **audioFx = nullptr);

    bool RemoveAudioFx(unsigned int audioFxIndex);
};

