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

#include "NvProjTrack.h"
#include "NvProjAudioTransition.h"


class CNvTimelineDesc;

class CNvProjAudioTrack : public CNvProjTrack
{
    friend class CNvProjTimeline;

protected:
    CNvProjAudioTrack(CNvProjTimeline *timeline, CNvProjContext *projContext);
    ~CNvProjAudioTrack();

public:
    // Pass an empty audio transition name will remove the audio transition
    // NOTE: The caller will obtain a reference count of the created audio transition
    bool SetBuiltinAudioTransition(unsigned int srcClipIndex,
                                   const QByteArray &audioTransitionName,
                                   CNvProjAudioTransition **audioTransition = nullptr);

protected:
    void PopulateTimelineDesc(CNvTimelineDesc *timelineDesc,
                              unsigned int audioTrackIndex);
};

