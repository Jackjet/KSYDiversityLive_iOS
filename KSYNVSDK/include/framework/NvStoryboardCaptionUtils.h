//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2015. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    May 19. 2015
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <NvDef.h>
#include <NvVideoEffectParams.h>

#include <QString>
#include <QSizeF>
#include <QPointF>
#include <QTransform>
#include <QFontMetricsF>
#include <QFont>
#include <QUuid>


enum ENvStoryboardTextAlignment
{
    keNvStoryboardTextAlignment_Center = 0,
    keNvStoryboardTextAlignment_Left,
    keNvStoryboardTextAlignment_Right,
    keNvStoryboardTextAlignment_Bottom = keNvStoryboardTextAlignment_Left,
    keNvStoryboardTextAlignment_Top = keNvStoryboardTextAlignment_Right,
};

enum ENvCaptionStoryboardFillMode
{
    keNvCaptionStoryboardFillMode_PanAndScan = 0,
    keNvCaptionStoryboardFillMode_LetterBox,
    keNvCaptionStoryboardFillMode_Stretch
};


QString NvStoryboardCaptionSetText(const QString &storyboardCaptionXml, const QString &textString);
QString NvStoryboardCaptionGetText(const QString &storyboardCaptionXml);

bool NvStoryboardCaptionHasCharAnimation(const QString &storyboardCaptionXml);

QString NvStoryboardCaptionSetFontFamily(const QString &storyboardCaptionXml, const QString &fontFamily);
QString NvStoryboardCaptionGetFontFamily(const QString &storyboardCaptionXml);

QString NvStoryboardCaptionSetTextAlignment(const QString &storyboardCaptionXml, ENvAlignment textAlign);
ENvAlignment NvStoryboardCaptionGetTextAlignment(const QString &storyboardCaptionXml);

QString NvStoryboardCaptionSetBold(const QString &storyboardCaptionXml, bool isBold);
bool NvStoryboardCaptionGetBold(const QString &storyboardCaptionXml);

QString NvStoryboardCaptionSetItalic(const QString &storyboardCaptionXml, bool isItalic);
bool NvStoryboardCaptionGetItalic(const QString &storyboardCaptionXml);

QString NvStoryboardCaptionSetTextColor(const QString &storyboardCaptionXml, const SNvColor &textColor);
SNvColor NvStoryboardCaptionGetTextColor(const QString &storyboardCaptionXml);

QString NvStoryboardCaptionSetOutlineInfo(const QString &storyboardCaptionXml,
                                          bool drawOutline,
                                          const SNvColor &outlineColor,
                                          float outlineWidth);

void NvStoryboardCaptionGetOutlineInfo(const QString &storyboardCaptionXml,
                                       bool *drawOutline = 0,
                                       SNvColor *outlineColor = 0,
                                       float *outlineWidth = 0);

QString NvStoryboardCaptionSetShadowInfo(const QString &storyboardCaptionXml,
                                         bool drawShadow,
                                         const SNvColor &shadowColor,
                                         float shadowOffsetX,
                                         float shadowOffsetY);

void NvStoryboardCaptionGetShadowInfo(const QString &storyboardCaptionXml,
                                      bool *drawShadow = 0,
                                      SNvColor *shadowColor = 0,
                                      float *shadowOffsetX = 0,
                                      float *shadowOffsetY = 0);

QString NvStoryboardCaptionSetDuration(const QString &storyboardCaptionXml, int durationMs);

void NvStoryboardCaptionGetBasicInfo(const QString &storyboardCaptionXml,
                                     QSize &sceneSize,
                                     QString &fontFamily,
                                     float &fontHeight,
                                     ENvAlignment &textAlign,
                                     bool &bold,
                                     bool &italic,
                                     float &textPosX,
                                     float &textPosY,
                                     ENvStoryboardTextAlignment &alignX,
                                     ENvStoryboardTextAlignment &alignY,
                                     bool &hasCharAnimation,
                                     bool &posSensitive);

QSizeF NvStoryboardCaptionCalcTextSize(const QString &textString,
                                       QFont &font,
                                       bool hasCharAnimation,
                                       ENvAlignment align);

QSizeF NvStoryboardCaptionCalcTextSize(const QString &textString,
                                       const QString &fontFamily,
                                       int fontSizeInPixel,
                                       bool isBold,
                                       bool isItalic,
                                       bool hasCharAnimation,
                                       ENvAlignment align);

// Adjust text height according to font line spacing
qreal NvStoryboardCaptionAdjustTextHeight(qreal textHeight, const QFontMetricsF &fm);

void NvStoryboardCaptionCalcTextBounding(const QSizeF &textSize,
                                         float textPosX,
                                         float textPosY,
                                         ENvStoryboardTextAlignment alignX,
                                         ENvStoryboardTextAlignment alignY,
                                         SNvRectF *bounding);

void NvStoryboardCaptionTextBoundingWithScaleFactor(const SNvRectF &origBounding,
                                                    ENvStoryboardTextAlignment alignX,
                                                    ENvStoryboardTextAlignment alignY,
                                                    float scaleFactor,
                                                    SNvRectF *newBounding);

void NvStoryboardCaptionTextCalcTransformedBounding(const SNvRectF &origBounding,
                                                    const QTransform &transform,
                                                    SNvRectF *newBounding);

// NOTE: This function is very time consuming, use it only when absolutely necessary!
QTransform NvStoryboardCaptionCalcPosterTransformation(const QString &storyboardCaptionXml);

void NvStoryboardCaptionCalcPosterTextBounding(const SNvRectF &origTextBounding,
                                               ENvStoryboardTextAlignment alignX,
                                               ENvStoryboardTextAlignment alignY,
                                               const QTransform &posterTransformation,
                                               float fontScaleFactor,
                                               float anchorX,
                                               float anchorY,
                                               float scaleX,
                                               float scaleY,
                                               float transX,
                                               float transY,
                                               SNvRectF *textBounding);

void NvCalcStoryboardToVideoEditScaleFactor(const QSize &storyboardSceneSize,
                                            ENvCaptionStoryboardFillMode fillMode,
                                            const QSize &videoEditSize,
                                            const SNvRational &videoEditPAR,
                                            qreal &scaleFactorX,
                                            qreal &scaleFactorY);

QPointF NvStoryboardCaptionConvertStoryboardCoordsToVideoEditCoords(const QSize &storyboardSceneSize,
                                                                    ENvCaptionStoryboardFillMode fillMode,
                                                                    const QSize &videoEditSize,
                                                                    const SNvRational &videoEditPAR,
                                                                    const QPointF &pt);

QPointF NvStoryboardCaptionConvertVideoEditCoordsToStoryboardCoords(const QSize &videoEditSize,
                                                                    const SNvRational &videoEditPAR,
                                                                    const QSize &storyboardSceneSize,
                                                                    ENvCaptionStoryboardFillMode fillMode,
                                                                    const QPointF &pt);

//
// Caption style information file related
//
struct SNvCaptionStyleInfo {
    QUuid uuid;
    int version;
    QString minSdkString;
    QString supportedAspectRatio;

    SNvCaptionStyleInfo()
    {
        version = 0;
    }
};

bool NvStoryboardCaptionReadStyleInfo(const QString &captionStyleInfoFilePath,
                                      SNvCaptionStyleInfo &info);

