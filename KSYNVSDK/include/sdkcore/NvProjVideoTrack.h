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
#include "NvProjVideoTransition.h"

#include <QVector>


class CNvTimelineDesc;
class CNvThemeDesc;
class __CNvAudioClipCallbackForVideoTrack;
class __CNvAudioTransitionCallbackForVideoTrack;

class CNvProjVideoTrack : public CNvProjTrack
{
    friend class CNvProjTimeline;
    friend class CNvProjTrack;
    friend class __CNvAudioClipCallbackForVideoTrack;
    friend class __CNvAudioTransitionCallbackForVideoTrack;

protected:
    CNvProjVideoTrack(CNvProjTimeline *timeline, CNvProjContext *projContext);
    ~CNvProjVideoTrack();

public:
    bool HasThemeTitle() const;
    bool HasThemeTrailer() const;

    // Pass an empty video transition name will remove the video transition
    // NOTE: The caller will obtain a reference count of the created video transition
    bool SetBuiltinVideoTransition(unsigned int srcClipIndex,
                                   const QByteArray &videoTransitionName,
                                   CNvProjVideoTransition **videoTransition = nullptr);

    // NOTE: The caller will obtain a reference count of the created transition
    // Pass an empty transitionPackageId will remove the video transition
    // transitionPackageId of value "theme" means video transition shipped in current theme
    bool SetPackagedVideoTransition(unsigned int srcClipIndex,
                                    const QString &transitionPackageId = QString(),
                                    CNvProjVideoTransition **videoTransition = nullptr);

protected:
    virtual void Destroy();

protected:
    void PopulateTimelineDesc(CNvTimelineDesc *timelineDesc,
                              unsigned int videoTrackIndex,
                              unsigned int audioTrackIndex);

private:
    bool IsValidCutPoint(unsigned int srcClipIndex) const;

    bool DoSetBuiltinVideoTransition(unsigned int srcClipIndex,
                                     const QByteArray &videoTransitionName,
                                     CNvProjVideoTransition **videoTransition);

    bool DoApplyThemeVideoTransition(unsigned int srcClipIndex,
                                     unsigned int cutPointCount,
                                     const CNvThemeDesc *themeDesc);

    void ApplyTheme(const CNvThemeDesc *themeDesc);
    void CleanupTheme();

    void SetupThemeVideoTransitions(const CNvThemeDesc *themeDesc,
                                    unsigned int startSrcClipIndex,
                                    unsigned int endSrcClipIndex = -1);
    void ResetThemeVideoTransitions(unsigned int startSrcClipIndex,
                                    unsigned int endSrcClipIndex = -1);

    void AdjustThemeOnClipInsertion(unsigned int insertedClipIndex);
    void AdjustThemeOnClipRemoval(unsigned int removedClipIndex);
    void AdjustThemeOnClipMovement(unsigned int sourceClipIndex, unsigned int targetClipIndex);

private:
    QVector<__CNvAudioClipCallbackForVideoTrack *> m_audioClipCallbackArray;
    QVector<__CNvAudioTransitionCallbackForVideoTrack *> m_audioTransitionCallbackArray;
};

