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
#include <NvStoryboardCaptionUtils.h>


class CNvProjVideoTrack;

class CNvProjTimelineCaption : public CNvProjTimelineFilter
{
    friend class CNvProjTimeline;
    friend class CNvProjVideoTrack;

public:
    enum Category
    {
        DefaultCategory = 0,
        UserCategory,
        ThemeCategory
    };

    enum RoleInTheme
    {
        RoleInThemeGeneral,
        RoleInThemeTitle,
        RoleInThemeTrailer
    };

protected:
    CNvProjTimelineCaption(CNvProjTimeline *ownerTimeline,
                           CNvAppFxInstance *fxInstance,
                           const QString &captionText,
                           bool isPanoramic,
                           CNvProjContext *projContext);
    ~CNvProjTimelineCaption();

public:
    bool IsPanoramic() const { return m_isPanoramic; }
    Category GetCategory() const { return m_category; }
    RoleInTheme GetRoleInTheme() const { return m_roleInTheme; }
    QString CaptionStylePackageId() const { return m_captionStylePackageId; }

    // An empty captionStylePackageId means the default builtin caption style
    // captionStylePackageId of value "Theme" means caption style shipped in current theme
    bool ApplyCaptionStyle(const QString &captionStylePackageId);

public:
    void SetText(const QString &text);
    QString GetText() const;

    void SetTextAlignment(ENvAlignment textAlign);
    ENvAlignment GetTextAlignment() const;

    void SetBold(bool bold);
    bool GetBold() const;

    void SetItalic(bool italic);
    bool GetItalic() const;

    void SetTextColor(const SNvColor &textColor);
    SNvColor GetTextColor() const;

    void SetDrawOutline(bool drawOutline);
    bool GetDrawOutline() const;
    void SetOutlineColor(const SNvColor &outlineColor);
    SNvColor GetOutlineColor() const;
    void SetOutlineWidth(float outlineWidth);
    float GetOutlineWidth() const;

    void SetDrawShadow(bool drawShadow);
    bool GetDrawShadow() const;
    void SetShadowColor(const SNvColor &shadowColor);
    SNvColor GetShadowColor() const;
    void SetShadowOffset(const QPointF &shadowOffset);
    QPointF GetShadowOffset() const;

    void SetFontSize(float fontSize);
    float GetFontSize() const;

    void SetFontByFilePath(QString filePath);
    QString GetFontFilePath();

    void SetCaptionTranslation(const QPointF &translation);
    QPointF GetCaptionTranslation() const;
    void TranslateCaption(const QPointF &translationOffset);

    SNvRectF GetTextBoundingRect() const;

    void ScaleCaption(float scaleFactor, const QPointF &anchor);

public:
    //
    // Panoramic caption specific methods
    //
    void SetCenterPolarAngle(float centerPolarAngle);
    float GetCenterPolarAngle() const;

    void SetCenterAzimuthAngle(float centerAzimuthAngle);
    float GetCenterAzimuthAngle() const;

    void SetPolarAngleRange(float polarAngleRange);
    float GetPolarAngleRange() const;

    float GetOrthoAngleRange() const;

    void SetPanoramicScaleX(float scaleX);
    float GetPanoramicScaleX() const;
    void SetPanoramicScaleY(float scaleY);
    float GetPanoramicScaleY() const;
    void SetPanoramicRotation(float rotationAngle);
    float GetPanoramicRotation() const;

protected:
    virtual void Destroy();

    virtual void OnChangeEffectIn(int64_t newEffectIn);
    virtual void OnChangeEffectOut(int64_t newEffectOut);

private:
    bool DoApplyCaptionStyle(const QString &captionStylePackageId);
    bool DoApplyCaptionStyle(Category category,
                             const QString &captionStylePackageId,
                             const QString &captionStyleDescStringTemplate,
                             const QString &captionStyleResourceDirPath);

    void HandleDurationChange(int64_t newDuration);

    void UpdateOriginalTextBoundingRect();
    SNvRectF GetTextBoundingRectInStoryboardCoord() const;

    void ApplyOutlineInfo();
    void ApplyShadowInfo();

private:
    bool m_isPanoramic;

    Category m_category;
    RoleInTheme m_roleInTheme;

    QString m_captionStylePackageId;

private:
    //
    // Caption information, all geometry related value are in storyboard space
    //
    QString m_captionText;
    QString m_fontFamily;
    QString m_fontFilePath;
    float m_fontHeight;
    ENvAlignment m_textAlign;
    bool m_bold;
    bool m_italic;
    SNvColor m_textColor;
    bool m_drawOutline;
    SNvColor m_outlineColor;
    float m_outlineWidth; // In storyboard coordinates
    bool m_drawShadow;
    SNvColor m_shadowColor;
    QPointF m_shadowOffset;
    float m_textPosX, m_textPosY;
    ENvStoryboardTextAlignment m_alignX, m_alignY;
    bool m_hasCharAnimation;
    bool m_isPositionSensitive;

    QTransform m_posterTransform;
    SNvRectF m_origTextBounding; // In storyboard space

    float m_storyboardToEditResScaleFactor;

    int m_textAlignChanged:1;
    int m_boldChanged:1;
    int m_italicChanged:1;
    int m_textColorChanged:1;
    int m_drawOutlineChanged:1;
    int m_outlineColorChanged:1;
    int m_outlineWidthChanged:1;
    int m_drawShadowChanged:1;
    int m_shadowColorChanged:1;
    int m_shadowOffsetChanged:1;
    int m_fontScaleFactorChanged:1;
    int m_captionScaleChanged:1;
    int m_captionTranslationChanged:1;
    int m_fontFamilyChanged:1;
};

