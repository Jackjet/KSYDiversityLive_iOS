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
#include "NvProjVideoTrack.h"
#include "NvProjAudioTrack.h"
#include "NvProjTimelineCaption.h"
#include "NvProjTimelineAnimatedSticker.h"
#include "NvProjTimelineVideoFx.h"
#include <NvStreamingEngineDef.h>
#include <NvSmartPtr.h>

#include <QReadWriteLock>
#include <QVector>


class CNvTimelineDesc;
class CNvStreamingContext;
class CNvProjTimelineFilterGroup;
class __CNvAudioClipCallbackForThemeMusicTrack;
class CNvThemeDesc;

class CNvProjTimeline : public CNvProjObject,
                        public SNvTimelineCallback
{
    friend class CNvProjContext;
    friend class CNvProjTrack;
    friend class CNvProjVideoTrack;
    friend class CNvProjTimelineCaption;
    friend class CNvStreamingContext;
    friend class __CNvAudioClipCallbackForThemeMusicTrack;

protected:
    CNvProjTimeline(const SNvVideoResolution &videoEditRes,
                    const SNvRational &videoFps,
                    const SNvAudioResolution &audioEditRes,
                    CNvProjContext *projContext);
    ~CNvProjTimeline();

public:
    const SNvVideoResolution & VideoEditRes() const { return m_videoEditRes; }
    ENvStreamingEngineTimelineAspectRatio TimelineAspectRatio() const { return m_timelineAspectRatio; }
    const SNvRational & VideoFps() const { return m_videoFps; }
    const SNvAudioResolution & AudioEditRes() const { return m_audioEditRes; }

    int64_t TimelineDuration() const { return m_timelineDuration; }

    unsigned int VideoTrackCount() const { return m_videoTrackArray.size(); }
    unsigned int AudioTrackCount() const { return m_audioTrackArray.size(); }

    CNvProjVideoTrack * GetVideoTrackByIndex(unsigned int trackIndex) const;
    CNvProjAudioTrack * GetAudioTrackByIndex(unsigned int trackIndex) const;

    // NOTE: The caller will obtain a reference count of the created track
    // Return the track index of the newly created track, -1 indicate error
    int AppendVideoTrack(CNvProjVideoTrack **createdTrack = nullptr);
    int AppendAudioTrack(CNvProjAudioTrack **createdTrack = nullptr);

    bool RemoveVideoTrack(unsigned int trackIndex);
    bool RemoveAudioTrack(unsigned int trackIndex);

    //
    // Timeline caption related
    //
    CNvProjTimelineCaption * GetFirstCaption() const;
    CNvProjTimelineCaption * GetLastCaption() const;
    CNvProjTimelineCaption * GetPrevCaption(CNvProjTimelineCaption *caption) const;
    CNvProjTimelineCaption * GetNextCaption(CNvProjTimelineCaption *caption) const;
    QVector<CNvProjTimelineCaption *> GetCaptionsByTimelinePosition(int64_t timelinePos) const;

    // NOTE: The caller will obtain a reference count of the created timeline caption
    // An empty captionStylePackageId means the default builtin caption style
    // captionStylePackageId of value "theme" means caption style shipped in current theme
    bool AddCaption(const QString &captionText,
                    int64_t inPoint,
                    int64_t duration,
                    bool isPanoramic,
                    const QString &captionStylePackageId = QString(),
                    CNvProjTimelineCaption **createdCaption = nullptr);

    CNvProjTimelineCaption * RemoveCaption(CNvProjTimelineCaption *caption);

    //
    // Animated sticker related
    //
    CNvProjTimelineAnimatedSticker * GetFirstAnimatedSticker() const;
    CNvProjTimelineAnimatedSticker * GetLastAnimatedSticker() const;
    CNvProjTimelineAnimatedSticker * GetPrevAnimatedSticker(CNvProjTimelineAnimatedSticker *sticker) const;
    CNvProjTimelineAnimatedSticker * GetNextAnimatedSticker(CNvProjTimelineAnimatedSticker *sticker) const;
    QVector<CNvProjTimelineAnimatedSticker *> GetAnimatedStickersByTimelinePosition(int64_t timelinePos) const;

    // NOTE: The caller will obtain a reference count of the created timeline animated sticker
    bool AddAnimatedSticker(int64_t inPoint,
                            int64_t duration,
                            const QString &animatedStickerPackageId,
                            bool isPanoramic,
                            CNvProjTimelineAnimatedSticker **createdSticker = nullptr);

    CNvProjTimelineAnimatedSticker * RemoveAnimatedSticker(CNvProjTimelineAnimatedSticker *sticker);

    //
    // Timeline video Fx related
    //
    CNvProjTimelineVideoFx * GetFirstTimelineVideoFx() const;
    CNvProjTimelineVideoFx * GetLastTimelineVideoFx() const;
    CNvProjTimelineVideoFx * GetPrevTimelineVideoFx(CNvProjTimelineVideoFx *videoFx) const;
    CNvProjTimelineVideoFx * GetNextTimelineVideoFx(CNvProjTimelineVideoFx *videoFx) const;
    QVector<CNvProjTimelineVideoFx *> GetTimelineVideoFxByTimelinePosition(int64_t timelinePos) const;

    // NOTE: The caller will obtain a reference count of the created timeline video Fx
    bool AddBuiltinTimelineVideoFx(int64_t inPoint,
                                   int64_t duration,
                                   const QByteArray &videoFxName,
                                   CNvProjTimelineVideoFx **createdVideoFx = nullptr);

    // NOTE: The caller will obtain a reference count of the created timeline video Fx
    bool AddPackagedTimelineVideoFx(int64_t inPoint,
                                    int64_t duration,
                                    const QString &videoFxPackageId,
                                    CNvProjTimelineVideoFx **createdVideoFx = nullptr);

    // NOTE: The caller will obtain a reference count of the created timeline video Fx
    bool AddCustomTimelineVideoFx(int64_t inPoint,
                                  int64_t duration,
                                  INvCustomGpuVideoEffect *pICustomGpuVideoEffect,
                                  CNvProjTimelineVideoFx **createdVideoFx = nullptr);

    CNvProjTimelineVideoFx * RemoveTimelineVideoFx(CNvProjTimelineVideoFx *videoFx);

    //
    // Theme related
    //
    QString ThemeId() const { return m_themeId; }
    bool ApplyTheme(const QString &themeId);
    void RemoveCurrentTheme();

    void SetThemeTitleCaptionText(const QString &text);
    void SetThemeTrailerCaptionText(const QString &text);

    void SetThemeMusicVolumeGain(float leftVolumeGain, float rightVolumeGain);
    void GetThemeMusicVolumeGain(float *leftVolumeGain, float *rightVolumeGain) const;

private:
    //
    // SNvTimelineCallback interface
    //
    virtual bool GetVideoEffectDesc(unsigned int in_filterEffectIndex, INvEffectDescriptor **out_ppIEffectDesc);
    virtual bool FillVideoEffectSettings(unsigned int in_filterEffectIndex, INvEffectSettings *io_pIEffectSettings);
    virtual bool IsCaptionVideoEffect(unsigned int in_filterEffectIndex);
    virtual bool IsAnimatedStickerVideoEffect(unsigned int in_filterEffectIndex);
    virtual bool GetEffectTimeRange(unsigned int in_filterEffectIndex, int64_t *out_startTime, int64_t *out_endTime);

    virtual void GetCustomGpuEffect(unsigned int in_filterEffectIndex, INvCustomGpuVideoEffect **out_ppICustomGpuVideoEffect);

    virtual bool GetAudioEffectDesc(unsigned int in_filterEffectIndex, INvEffectDescriptor **out_ppIEffectDesc);
    virtual bool FillAudioEffectSettings(unsigned int in_filterEffectIndex, INvEffectSettings *io_pIEffectSettings);

private:
    void UpdateTimelineDuration();

    void PopulateTimelineDesc(CNvTimelineDesc *timelineDesc);
    void PopulateTimelineDescForThemeMusic(CNvTimelineDesc *timelineDesc);
    void PopulateTimelineDescForAnimatedSticker(CNvTimelineDesc *timelineDesc,
                                                QVector<QByteArray> &timelineFilterArray);

    const CNvThemeDesc * GetThemeDesc(const QString &themeId) const;

    void AdjustTimelineFiltersOnInsertSpace(int64_t insertPosition, int64_t spaceLength);
    void AdjustTimelineFiltersOnRemoveSpace(int64_t removePosition, int64_t spaceLength);

    bool DoAddCaption(const QString &captionText,
                      int64_t inPoint,
                      int64_t duration,
                      CNvProjTimelineCaption::RoleInTheme roleInTheme,
                      bool isPanoramic,
                      CNvProjTimelineCaption **createdCaption);

    bool DoAddAnimatedSticker(int64_t inPoint,
                              int64_t duration,
                              const QString &animatedStickerPackageId,
                              const QString &animatedStickerDescString,
                              const QString &animatedStickerResourceDirPath,
                              bool isPanoramic,
                              CNvProjTimelineAnimatedSticker **createdSticker);

protected:
    virtual void Destroy();

private:
    void NotifyTrackDurationChanged(CNvProjTrack::TrackType trackType,
                                    unsigned int trackIndex,
                                    int64_t newTrackDuration);

private:
    SNvVideoResolution m_videoEditRes;
    ENvStreamingEngineTimelineAspectRatio m_timelineAspectRatio;
    SNvRational m_videoFps;
    SNvAudioResolution m_audioEditRes;

    QString m_themeId;
    QString m_themeTitleCaptionText;
    QString m_themeTrailerCaptionText;

    int64_t m_timelineDuration;

    QVector<TNvSmartPtr<CNvProjVideoTrack> > m_videoTrackArray;
    QVector<TNvSmartPtr<CNvProjAudioTrack> > m_audioTrackArray;

    QVector<TNvSmartPtr<CNvAppFxInstance> > m_themeTimelineFilterInstanceArray;
    CNvProjTimelineFilterGroup *m_timlineVideoFxGroup;
    CNvProjTimelineFilterGroup *m_timlineAnimatedStickerGroup;
    CNvProjTimelineFilterGroup *m_timlineCaptionGroup;
    TNvSmartPtr<CNvAppFxInstance> m_themeEndingTimelineFilterInstance;
    TNvSmartPtr<CNvAppFxInstance> m_waterMarkTimelineFilterInstance;

    QVector<__CNvAudioClipCallbackForThemeMusicTrack *> m_themeMusicClipCallbackArray;

    enum TimelineFilterCatetory {
        TimelineVideoFx,
        ThemeTimelineFilter,
        AnimatedStickerTimelineFilter,
        CaptionTimelineFilter,
        OverlayTimelineFilter
    };

    // NOTE: Keep this structure as small as possible
    struct __SNvTimelineFilterInfo {
        TNvSmartPtr<CNvAppFxInstance> fxInstance;
        TNvSmartPtr<INvCustomGpuVideoEffect> pJCustomGpuEffect;
        TimelineFilterCatetory category;
        int64_t effecIn, effectOut;
    };
    QVector<__SNvTimelineFilterInfo> m_timelineFilterInfoArray;

    mutable QReadWriteLock m_rwLock;

    float m_leftThemeMusicVolumeGain, m_rightThemeMusicVolumeGain;
};

