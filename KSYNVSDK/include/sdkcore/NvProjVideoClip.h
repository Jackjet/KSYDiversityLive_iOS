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
#include "NvProjVideoFx.h"


class CNvProjTrack;
class CNvProjVideoTrack;
class CNvThemeDesc;

class CNvProjVideoClip : public CNvProjClip
{
    friend class CNvProjTrack;
    friend class CNvProjVideoTrack;

public:
    enum VideoClipType
    {
        VideoClipFromAV,
        VideoClipFromImage
    };

    enum RoleInTheme
    {
        RoleInThemeGeneral,
        RoleInThemeTitle,
        RoleInThemeTrailer
    };

protected:
    CNvProjVideoClip(CNvProjTrack *track,
                     VideoClipType videoClipType,
                     CNvProjContext *projContext);
    ~CNvProjVideoClip();

public:
    VideoClipType GetVideoClipType() const { return m_videoClipType; }
    RoleInTheme GetRoleInTheme() const { return m_roleInTheme; }

    void SetPlayInReverse(bool playInReverse);
    bool GetPlayInReverse() const;

    void SetPanAndScan(float pan, float scan);
    bool GetPanAndScan(float *pan, float *scan) const;

    void SetSourceBackgroundMode(ENvSourceBackgroundMode sourceBackgroundMode);
    ENvSourceBackgroundMode GetSourceBackgroundMode() const;

    void SetImageMotionMode(ENvStreamingEngineImageClipMotionMode mode);
    ENvStreamingEngineImageClipMotionMode ImageMotionMode() const;

    void SetImageMotionROI(const SNvRectF &startROI, const SNvRectF &endROI);
    void GetImageMotionROI(SNvRectF *startROI, SNvRectF *targetROI) const;

    void SetImageMotionAnimationEnabled(bool enable);
    bool IsImageMotionAnimationEnabled() const;

    // NOTE: The caller will obtain a reference count of the created video FX
    bool AppendBuiltinVideoFx(const QByteArray &videoFxName,
                              CNvProjVideoFx **videoFx = nullptr);

    // NOTE: The caller will obtain a reference count of the created video FX
    bool InsertBuiltinVideoFx(const QByteArray &videoFxName,
                              unsigned int insertPosition,
                              CNvProjVideoFx **videoFx = nullptr);

    // NOTE: The caller will obtain a reference count of the created video FX
    bool AppendPackagedVideoFx(const QString &videoFxPackageId,
                               CNvProjVideoFx **videoFx = nullptr);

    // NOTE: The caller will obtain a reference count of the created video FX
    bool InsertPackagedVideoFx(const QString &videoFxPackageId,
                               unsigned int insertPosition,
                               CNvProjVideoFx **videoFx = nullptr);

    // NOTE: The caller will obtain a reference count of the created video FX
    bool AppendCustomVideoFx(INvCustomGpuVideoEffect *pICustomGpuVideoEffect,
                             CNvProjVideoFx **videoFx = nullptr);

    // NOTE: The caller will obtain a reference count of the created video FX
    bool InsertCustomVideoFx(INvCustomGpuVideoEffect *pICustomGpuVideoEffect,
                             unsigned int insertPosition,
                             CNvProjVideoFx **videoFx = nullptr);

    bool RemoveVideoFx(unsigned int videoFxIndex);
    void RemoveAllVideoFx();

protected:
    virtual void SetClipFilePath(const QString &clipFilePath);

private:
    void ApplyTheme(const CNvThemeDesc *themeDesc,
                    unsigned int totalClipCount,
                    unsigned int clipIndex);
    void CleanupTheme();

private:
    VideoClipType m_videoClipType;
    RoleInTheme m_roleInTheme;

    bool m_playInReverse;

    bool m_usePanAndScan;
    float m_pan, m_scan;

    ENvSourceBackgroundMode m_sourceBackgroundMode;

    // The following memebers are used only when this clip is image based
    ENvStreamingEngineImageClipMotionMode m_imageMotionMode;
    SNvRectF m_imageMotionStartROI;
    SNvRectF m_imageMotionTargetROI;
    bool m_imageMotionAnimationEnabled;
};

