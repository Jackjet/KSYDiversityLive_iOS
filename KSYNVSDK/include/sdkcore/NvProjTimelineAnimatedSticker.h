//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2017. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 6. 2017
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvProjTimelineFilter.h"
#include <NvStoryboardAnimatedStickerUtils.h>


class CNvProjTimelineAnimatedSticker : public CNvProjTimelineFilter
{
    friend class CNvProjTimeline;

protected:
    CNvProjTimelineAnimatedSticker(CNvProjTimeline *ownerTimeline,
                                   CNvAppFxInstance *fxInstance,
                                   const SNvStoryboardAnimatedStickerInfo &stickerInfo,
                                   bool isPanoramic,
                                   CNvProjContext *projContext);
    ~CNvProjTimelineAnimatedSticker();

public:
    bool IsPanoramic() const { return m_isPanoramic; }

    QString AnimatedStickerPackageId() const { return m_animatedStickerPackageId; }
    const SNvStoryboardAnimatedStickerInfo & StickerInfo() const { return m_stickerInfo; }

    void SetScale(float scale);
    float GetScale() const;

    void SetHorizontalFlip(bool flip);
    bool GetHorizontalFlip() const;
    void SetVerticalFlip(bool flip);
    bool GetVerticalFlip() const;

    void SetRotationZ(float angle);
    float GetRotationZ() const;

    void SetTranslation(const QPointF &translation);
    QPointF GetTranslation() const;

    SNvRectF GetOriginalBoundingRect() const;

    // The first vertex will always be the top left vertex of the original vertices,
    // They are always ordered in counter-clock wise manner in the original space
    QVector<QPointF> GetBoundingRectangleVertices() const;

public:
    //
    // Panoramic animated sticker specific methods
    //
    void SetCenterPolarAngle(float centerPolarAngle);
    float GetCenterPolarAngle() const;

    void SetCenterAzimuthAngle(float centerAzimuthAngle);
    float GetCenterAzimuthAngle() const;

    void SetPolarAngleRange(float polarAngleRange);
    float GetPolarAngleRange() const;

    float GetOrthoAngleRange() const;

protected:
    virtual void Destroy();

private:
    bool m_isPanoramic;

    QString m_animatedStickerPackageId;
    SNvStoryboardAnimatedStickerInfo m_stickerInfo;

    float m_storyboardToEditResScaleFactor;
};

