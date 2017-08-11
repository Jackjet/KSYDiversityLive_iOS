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
#include "NvProjClipFilter.h"
#include <NvStreamingEngineDef.h>
#include <NvSmartPtr.h>

#include <QVector>
#include <QString>
#include <QReadWriteLock>


class CNvProjTimeline;
class CNvProjTrack;
class CNvProjVideoTrack;
class CNvProjAudioTrack;

class CNvProjClip : public CNvProjObject,
                    public SNvClipCallback
{
    friend class CNvProjTrack;
    friend class CNvProjVideoTrack;
    friend class CNvProjAudioTrack;

public:
    enum ClipType
    {
        VideoClip,
        AudioClip
    };

protected:
    CNvProjClip(CNvProjTrack *track,
                ClipType type,
                CNvProjContext *projContext,
                ProjObjectType projObjType);
    ~CNvProjClip();

public:
    ClipType GetClipType() const { return m_type; }

    CNvProjTimeline * OwnerTimeline() const;
    CNvProjTrack * OwnerTrack() const;

    unsigned int ClipIndex() const { return m_clipIndex; }
    QString ClipFilePath() const { return m_clipFilePath; }
    int64_t InPoint() const { return m_inPoint; }
    int64_t OutPoint() const { return m_outPoint; }
    int64_t TrimIn() const { return m_trimIn; }
    int64_t TrimOut() const { return m_trimOut; }
    double Speed() const { return m_speed; }

    // Return the actual trim in point after the change action
    int64_t ChangeTrimInPoint(int64_t newTrimInPoint, bool affectSibling = true);
    // Return the actual trim out point after the change action
    int64_t ChangeTrimOutPoint(int64_t newTrimOutPoint, bool affectSibling = true);

    void ChangeSpeed(double newSpeed);

    unsigned int ClipFilterCount() const { return m_filterArray.size(); }
    CNvProjClipFilter * GetClipFilterByIndex(unsigned int clipFilterIndex) const;

    void SetVolumeGain(float leftVolumeGain, float rightVolumeGain);
    void GetVolumeGain(float *leftVolumeGain, float *rightVolumeGain) const;

private:
    //
    // SNvClipCallback interface
    //
    virtual bool GetAVFileInfo(SNvAVFileInfo *out_avFileInfo);

    virtual bool GetEffectDesc(unsigned int in_filterEffectIndex, INvEffectDescriptor **out_ppIEffectDesc);
    virtual bool IsRangedFilter(unsigned int in_filterEffectIndex);
    virtual bool GetFilterEffectTimeRange(unsigned int in_filterEffectIndex, int64_t *out_startTime, int64_t *out_endTime);
    virtual bool FillEffectSettings(unsigned int in_filterEffectIndex, INvEffectSettings *io_pIEffectSettings);

    virtual void GetCustomGpuEffect(unsigned int in_filterEffectIndex, INvCustomGpuVideoEffect **out_ppICustomGpuVideoEffect);

    virtual unsigned int ExternalSourceCount(unsigned int in_filterEffectIndex);
    virtual QString ExternalSourcePath(unsigned int in_filterEffectIndex, unsigned int in_externalSourceIndex);

    // Used only by audio clip
    virtual void GetVolumeGain(float *out_gainLeft, float *out_gainRight, int64_t in_streamTime);

    // Used only by image clip
    virtual ENvStreamingEngineImageClipMotionMode GetImageMotionMode();
    virtual bool IsImageMotionAnimationDisabled();
    virtual void GetImageMotionROIs(SNvRectF *in_startROI, SNvRectF *in_targetROI);

    // Used only by video and image clip
    virtual ENvVideoRotation GetExtraVideoRotation();

    // Used only by video and image clip, return false if we don't apply pan and scan to this video clip
    virtual bool GetPanAndScanParameters(float *out_pan, float *out_scan);
    // Used only by video and image clip, return false if we don't apply sourceBackgroundMode to this video clip
    virtual ENvSourceBackgroundMode GetSourceBackgroundModeParameter();

protected:
    virtual void SetClipFilePath(const QString &clipFilePath);

    int64_t CalcTrimPosFromTimelinePos(int64_t timelinePos) const;
    int64_t CalcTimelinePosFromTrimPos(int64_t trimPos) const;
    int64_t CalcTrimLengthFromTimelineLength(int64_t timelineLength) const;
    int64_t CalcTimelineLengthFromTrimLength(int64_t trimLength) const;

    void AppendClipFilter(CNvProjClipFilter *clipFilter);
    void InsertClipFilter(CNvProjClipFilter *clipFilter, unsigned int insertPosition);
    bool RemoveClipFilter(unsigned int clipFilterIndex);
    void RemoveAllClipFilters();

protected:
    virtual void Destroy();

protected:
    CNvProjTrack *m_track;
    ClipType m_type;

    unsigned int m_clipIndex;
    QString m_clipFilePath;
    int64_t m_inPoint, m_outPoint;
    int64_t m_trimIn, m_trimOut;
    double m_speed;

    QVector<TNvSmartPtr<CNvProjClipFilter> > m_filterArray;
    QVector<TNvSmartPtr<CNvAppFxInstance> > m_themeFilterInstanceArray;

    mutable QReadWriteLock m_rwLock;

    float m_leftVolumeGain, m_rightVolumeGain;
};

