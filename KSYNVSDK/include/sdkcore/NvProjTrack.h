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

#include "NvProjObject.h"
#include "NvProjClip.h"
#include "NvProjTransition.h"
#include "NvProjVideoTransition.h"
#include <NvSmartPtr.h>

#include <QList>
#include <QMap>


class CNvProjTimeline;

class CNvProjTrack : public CNvProjObject
{
    friend class CNvProjTimeline;
    friend class CNvProjClip;

public:
    enum TrackType
    {
        VideoTrack,
        AudioTrack
    };

protected:
    CNvProjTrack(CNvProjTimeline *timeline,
                 TrackType type,
                 CNvProjContext *projContext,
                 ProjObjectType projObjType);
    ~CNvProjTrack();

public:
    TrackType GetTrackType() const { return m_type; }

    CNvProjTimeline * OwnerTimeline() const;

    unsigned int TrackIndex() const { return m_trackIndex; }
    int64_t TrackDuration() const { return m_trackDuration; }

    // NOTE: The caller will obtain a reference count of the created clip
    // Return the index of the created clip, -1 indicate error
    int InsertClip(const QString &clipFilePath,
                   unsigned int insertPoint,
                   CNvProjClip **createdClip = nullptr);
    int InsertClip(const QString &clipFilePath,
                   int64_t trimIn,
                   int64_t trimOut,
                   unsigned int insertPoint,
                   CNvProjClip **createdClip = nullptr);
    int AppendClip(const QString &clipFilePath,
                   CNvProjClip **createdClip = nullptr);
    int AppendClip(const QString &clipFilePath,
                   int64_t trimIn,
                   int64_t trimOut,
                   CNvProjClip **createdClip = nullptr);
    int AddClip(const QString &clipFilePath,
                int64_t inPoint,
                CNvProjClip **createdClip = nullptr);
    int AddClip(const QString &clipFilePath,
                int64_t inPoint,
                int64_t trimIn,
                int64_t trimOut,
                CNvProjClip **createdClip = nullptr);

    // Return the actual in point after the change action
    int64_t ChangeInPoint(unsigned int clipIndex, int64_t newInPoint);
    // Return the actual out point after the change action
    int64_t ChangeOutPoint(unsigned int clipIndex, int64_t newOutPoint);

    bool SplitClip(unsigned int clipIndex, int64_t splitPoint);
    bool MoveClip(unsigned int sourceClipIndex, unsigned int targetClipIndex);
    bool RemoveClip(unsigned int clipIndex, bool keepSpace = false);
    bool RemoveRange(int64_t startTimelinePos, int64_t endTimelinePos, bool keepSpace);
    void ClearAll();

    unsigned int ClipCount() const { return m_clipList.size(); }
    CNvProjClip * GetClipByIndex(unsigned int clipIndex) const;
    CNvProjClip * GetClipByTimelinePosition(int64_t timelinePos) const;
    CNvProjTransition * GetTransitionBySourceClipIndex(unsigned int srcClipIndex) const;

    void SetVolumeGain(float leftVolumeGain, float rightVolumeGain);
    void GetVolumeGain(float *leftVolumeGain, float *rightVolumeGain) const;

protected:
    int DoInsertClip(const QString &clipFilePath,
                     unsigned int insertPoint,
                     CNvProjClip **createdClip);
    int DoInsertClip(const QString &clipFilePath,
                     int64_t trimIn,
                     int64_t trimOut,
                     unsigned int insertPoint,
                     CNvProjClip **createdClip);
    void InsertClipInternal(CNvProjClip *clip, unsigned int insertPosition);
    bool DoRemoveClip(unsigned int clipIndex, bool keepSpace);
    bool DoMoveClip(unsigned int sourceClipIndex, unsigned int targetClipIndex);

    // Return the actual trim in point after the change action
    int64_t ChangeTrimInPoint(CNvProjClip *clip, int64_t newTrimInPoint, bool affectSibling);
    // Return the actual trim out point after the change action
    int64_t ChangeTrimOutPoint(CNvProjClip *clip, int64_t newTrimOutPoint, bool affectSibling);

    // Return the actual in point after the change action
    int64_t DoChangeInPoint(CNvProjClip *clip, int64_t newInPoint);
    // Return the actual out point after the change action
    // NOTE: This function does not update track duration
    int64_t DoChangeOutPoint(CNvProjClip *clip, int64_t newOutPoint);

    void HandleClipSpeedChange(CNvProjClip *clip);

    // NOTE: The caller will obtain a reference count of the created transition
    CNvProjTransition * CreateVideoTransition(unsigned int srcClipIndex,
                                              CNvAppFxInstance *fxInstance,
                                              CNvProjVideoTransition::VideoTransitionType videoTransitionType,
                                              CNvProjVideoTransition::Category videoTransitionCategory);

    // NOTE: The caller will obtain a reference count of the created transition
    CNvProjTransition * CreateAudioTransition(unsigned int srcClipIndex,
                                              CNvAppFxInstance *fxInstance);

    // NOTE: The caller will obtain a reference count of the created transition
    CNvProjTransition * CreateDefaultTransition(unsigned int srcClipIndex);
    void DestoryTransition(unsigned int srcClipIndex);

private:
    void DisplaceClips(unsigned int startClipIndex,
                       int clipCount, // -1 indicate displace to the last clip
                       int indexDelta,
                       int64_t inOutPointDelta);

    void DisplaceTransition(unsigned int startSrcClipIndex,
                            unsigned int endSrcClipIndex, // Inclusive, -1 indicate last clip
                            int delta);

    void UpdateTrackDuration(int64_t newDuration);

    bool ShouldAdjustTimelineFilters() const;

protected:
    virtual void Destroy();

protected:
    CNvProjTimeline *m_timeline;
    TrackType m_type;

    unsigned int m_trackIndex;
    int64_t m_trackDuration;

    // Ordered list of all clips in this track
    QList<TNvSmartPtr<CNvProjClip> > m_clipList;
    // Ordered map of all clips in this track, key is clip's in point
    QMap<int64_t, CNvProjClip *> m_clipMap;

    // NOTE: Key is the transition's source clip's index
    QMap<unsigned int, TNvSmartPtr<CNvProjTransition> > m_transitionMap;

    mutable QReadWriteLock m_rwLock;

    float m_leftVolumeGain, m_rightVolumeGain;
};

