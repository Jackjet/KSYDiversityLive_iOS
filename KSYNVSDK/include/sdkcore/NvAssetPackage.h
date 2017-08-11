//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2017. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 5. 2017
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvAssetPackageManager.h"
#include <NvStoryboardAnimatedStickerUtils.h>
#include <NvAppFx.h>
#include <NvEffectDef.h>

#include <QXmlStreamReader>
#include <QJsonArray>
#include <QHash>
#include <QSet>


//////////////////////////////////////////////////////////////////////////////////
/// Base class of all asset package type
//////////////////////////////////////////////////////////////////////////////////
class CNvAssetPackage
{
    friend class CNvAssetPackageManager;

public:
    CNvAssetPackage(CNvAssetPackageManager::AssetType type,
                    CNvAssetPackageManager::AssetPackageStatus status,
                    const QUuid &assetPackageUuid,
                    const QString &assetPackageUuidString);
    virtual ~CNvAssetPackage();

public:
    CNvAssetPackageManager::AssetType Type() const { return m_type; }
    CNvAssetPackageManager::AssetPackageStatus Status() const { return m_status; }
    const QUuid & Uuid() const { return m_uuid; }
    const QString & UuidString() const { return m_uuidString; }
    const QString & PackageDirPath() const { return m_packageDirPath; }
    int Version() const { return m_version; }

protected:
    CNvAssetPackageManager::AssetType m_type;
    CNvAssetPackageManager::AssetPackageStatus m_status;
    QUuid m_uuid;
    QString m_uuidString;
    QString m_packageDirPath;
    int m_version;
    int m_supportedAspectRatio; // Bitwise OR of keNvStreamingEngineTimelineAspectRatio_XXX

protected:
    virtual CNvAssetPackageManager::AssetPackageError ReadMetaData(const QString &packageDirPath) = 0;

protected:
    bool ParseSdkVersion(const QString &sdkVersionString,
                         int &sdkVersionMajorNum,
                         int &sdkVersionMinorNum,
                         int &sdkVersionRevision) const;

    bool ValidateMinimalSdkVersion(const QString &minSdkVersionString);

    int ParseSupportedAspectRatio(const QString &supportedAspectRatioString) const;

    ENvStreamingEngineTimelineAspectRatio SelectProperAspectRatio(ENvStreamingEngineTimelineAspectRatio requestedAspectRatio) const;
};


//////////////////////////////////////////////////////////////////////////////////
/// Video FX asset package
//////////////////////////////////////////////////////////////////////////////////
class CNvVideoFxAssetPackage : public CNvAssetPackage
{
public:
    CNvVideoFxAssetPackage(CNvAssetPackageManager::AssetPackageStatus status,
                           const QUuid &assetPackageUuid,
                           const QString &assetPackageUuidString);
    ~CNvVideoFxAssetPackage();

public:
    QString QueryDescFilePath(ENvStreamingEngineTimelineAspectRatio requestedAspectRatio) const;
    bool NoBackground() const { return m_noBackground; }

protected:
    virtual CNvAssetPackageManager::AssetPackageError ReadMetaData(const QString &packageDirPath);

private:
    bool m_noBackground;
};


//////////////////////////////////////////////////////////////////////////////////
/// Video transition asset package
//////////////////////////////////////////////////////////////////////////////////
class CNvVideoTransitionAssetPackage : public CNvAssetPackage
{
public:
    CNvVideoTransitionAssetPackage(CNvAssetPackageManager::AssetPackageStatus status,
                                   const QUuid &assetPackageUuid,
                                   const QString &assetPackageUuidString);
    ~CNvVideoTransitionAssetPackage();

public:
    bool Is3DVideoTranstion() const { return m_is3D; }

    QString Query2DWipeMaskDescFilePath(ENvStreamingEngineTimelineAspectRatio requestedAspectRatio) const;
    QString Query2DWipeOverlayDescFilePath(ENvStreamingEngineTimelineAspectRatio requestedAspectRatio) const;
    QString Query3DDescFilePath(ENvStreamingEngineTimelineAspectRatio requestedAspectRatio) const;

protected:
    virtual CNvAssetPackageManager::AssetPackageError ReadMetaData(const QString &packageDirPath);

private:
    bool m_is3D;
};


//////////////////////////////////////////////////////////////////////////////////
/// Caption style asset package
//////////////////////////////////////////////////////////////////////////////////
class CNvCaptionStyleAssetPackage : public CNvAssetPackage
{
public:
    CNvCaptionStyleAssetPackage(CNvAssetPackageManager::AssetPackageStatus status,
                                const QUuid &assetPackageUuid,
                                const QString &assetPackageUuidString);
    ~CNvCaptionStyleAssetPackage();

public:
    QString QueryDescFilePath(ENvStreamingEngineTimelineAspectRatio requestedAspectRatio) const;
    QString QueryDescXmlString(ENvStreamingEngineTimelineAspectRatio requestedAspectRatio) const;

protected:
    virtual CNvAssetPackageManager::AssetPackageError ReadMetaData(const QString &packageDirPath);
};


//////////////////////////////////////////////////////////////////////////////////
/// Animated sticker asset package
//////////////////////////////////////////////////////////////////////////////////
class CNvAnimatedStickerAssetPackage : public CNvAssetPackage
{
public:
    CNvAnimatedStickerAssetPackage(CNvAssetPackageManager::AssetPackageStatus status,
                                   const QUuid &assetPackageUuid,
                                   const QString &assetPackageUuidString);
    ~CNvAnimatedStickerAssetPackage();

public:
    QString QueryDescFilePath(ENvStreamingEngineTimelineAspectRatio requestedAspectRatio) const;
    bool QueryStickerInfo(ENvStreamingEngineTimelineAspectRatio requestedAspectRatio,
                          SNvStoryboardAnimatedStickerInfo &stickerInfo) const;
    QString QueryDescXmlString(ENvStreamingEngineTimelineAspectRatio requestedAspectRatio) const;

protected:
    virtual CNvAssetPackageManager::AssetPackageError ReadMetaData(const QString &packageDirPath);
};


//////////////////////////////////////////////////////////////////////////////////
/// Theme asset package
//////////////////////////////////////////////////////////////////////////////////
class CNvThemeAssetPackage;
class CNvProjContext;

class CNvThemeDesc
{
    friend class CNvThemeAssetPackage;

private:
    CNvThemeDesc();
    ~CNvThemeDesc();

public:
    QString ThemeDirPath() const;

    bool HasTitle() const { return m_hasTitle; }
    QString TitleFilePath() const;
    QVector<TNvSmartPtr<CNvAppFxInstance> > GenerateTitleClipAppFxInstanceArray(CNvProjContext *projContext) const;

    bool HasTrailer() const { return m_hasTrailer; }
    QString TrailerFilePath() const;
    QVector<TNvSmartPtr<CNvAppFxInstance> > GenerateTrailerClipAppFxInstanceArray(CNvProjContext *projContext) const;

    QVector<TNvSmartPtr<CNvAppFxInstance> > GenerateGeneralClipAppFxInstanceArray(CNvProjContext *projContext,
                                                                                  unsigned int generalClipCount,
                                                                                  unsigned int generalClipIndex) const;

    // NOTE: The caller will obtain a reference count of the created App FX instance
    void GenerateTransitionAppFxInstance(CNvProjContext *projContext,
                                         unsigned int cutPointCount,
                                         unsigned int cutPointIndex,
                                         CNvAppFxInstance **transitionAppFxInstance) const;

    QVector<TNvSmartPtr<CNvAppFxInstance> > GenerateTimelineAppFxInstanceArray(CNvProjContext *projContext) const;

    bool HasTitleCaption() const { return !m_titleCaptionStyleXmlString.isEmpty(); }
    QString TitleCaptionStyleXmlString() const { return m_titleCaptionStyleXmlString; }
    QString TitleCaptionDefaultText() const { return m_titleCaptionDefaultText; }
    bool HasTrailerCaption() const { return !m_trailerCaptionStyleXmlString.isEmpty(); }
    QString TrailerCaptionStyleXmlString() const { return m_trailerCaptionStyleXmlString; }
    QString TrailerCaptionDefaultText() const { return m_trailerCaptionDefaultText; }
    QString GeneralClipCaptionStyleXmlString() const { return m_generalClipCaptionStyleXmlString; }

    QStringList GetMusicFileList() const { return m_musicFileList; }
    bool RepeatLastMusic() const { return m_repeatLastMusic; }

private:
    enum Occurrence {
        OccurrenceAll,
        OccurrenceIndices,
        OccurrencePeriod
    };

    struct __NvThemeAppFxDesc {
        QByteArray appFxName;
        QHash<QByteArray, SNvFxParamVal> generalParamValSet; // Key is App FX general parameter name
        QHash<QByteArray, QString> menuParamValSet; // Key is App FX menu identifier, value is menu entry name

        Occurrence occurrenceMode;
        QSet<int> occurrenceIndices;
        unsigned int occurrencePeroidFirstIndex;
        unsigned int occurrencePeriod;

        __NvThemeAppFxDesc()
        {
            occurrenceMode = OccurrenceAll;
            occurrencePeroidFirstIndex = 0;
            occurrencePeriod = 1;
        }
    };

    struct __NvThemeSpecialClipDesc {
        QString clipFilePath;
        QVector<__NvThemeAppFxDesc> filterDescArray;
    };

    // NOTE: This theme directory path ends with a trailing slash!
    QString m_themeDirPath;
    bool m_replace;

    __NvThemeSpecialClipDesc m_titleClipDesc, m_trailerClipDesc;
    bool m_hasTitle, m_hasTrailer;

    // General video clip's filter descriptor array
    QVector<__NvThemeAppFxDesc> m_clipFilterDescArray;

    // Timeline filter descriptor array
    QVector<__NvThemeAppFxDesc> m_timelineFilterDescArray;

    // Video transition descriptor array
    QVector<__NvThemeAppFxDesc> m_transitionDescArray;

    //
    // Caption style related
    //
    QString m_titleCaptionStyleXmlString;
    QString m_titleCaptionDefaultText;
    QString m_trailerCaptionStyleXmlString;
    QString m_trailerCaptionDefaultText;
    QString m_generalClipCaptionStyleXmlString;

    //
    // Music related
    //
    QStringList m_musicFileList;
    bool m_repeatLastMusic;

private:
    bool Load(const QString &themeDirPath,
              ENvStreamingEngineTimelineAspectRatio requestedAspectRatio,
              CNvProjContext *projContext,
              const QHash<QString, QString> &translationTable);

    void ParseThemeXmlElement(QXmlStreamReader &xmlReader);
    void ParseTitleXmlElement(QXmlStreamReader &xmlReader);
    void ParseTrailerXmlElement(QXmlStreamReader &xmlReader);
    void ParseClipsXmlElement(QXmlStreamReader &xmlReader);
    void ParseTimelineFilterListXmlElement(QXmlStreamReader &xmlReader);
    void ParseTransitionListXmlElement(QXmlStreamReader &xmlReader);
    void ParseFilterListXmlElement(QXmlStreamReader &xmlReader, QVector<__NvThemeAppFxDesc> &filterDescArray);
    bool ParseAppFxXmlElement(QXmlStreamReader &xmlReader, const QString &xmlElementName, __NvThemeAppFxDesc &appFxDesc);
    void ParseAppFxGeneralParamXmlElement(QXmlStreamReader &xmlReader,
                                          CNvAppFxDesc *appFxDescriptor,
                                          QHash<QByteArray, SNvFxParamVal> &generalParamValSet);
    void ParseAppFxMenuParamXmlElement(QXmlStreamReader &xmlReader,
                                       CNvAppFxDesc *appFxDescriptor,
                                       QHash<QByteArray, QString> &menuParamValSet);
    void ParseMusicTrackXmlElement(QXmlStreamReader &xmlReader);
    QString ParseMusicXmlElement(QXmlStreamReader &xmlReader);

    QString ReadCaptionStyleXmlString(const QString &captionStyleDescFilePath,
                                      const QHash<QString, QString> &translationTable,
                                      QString *defaultText = nullptr) const;

private:
    //
    // Temporary members used only when loading from theme meta file
    //
    CNvProjContext *m_projContext;

    QString m_xmlElementTitle;
    QString m_xmlElementTrailer;
    QString m_xmlElementClips;
    QString m_xmlElementTimelineFilterList;
    QString m_xmlElementTransitionList;

private:
    QVector<TNvSmartPtr<CNvAppFxInstance> > GenerateAppFxInstanceArray(CNvProjContext *projContext,
                                                                       const QVector<__NvThemeAppFxDesc> &appFxDescArray) const;
    CNvAppFxInstance * GenerateAppFxInstance(CNvProjContext *projContext,
                                             const __NvThemeAppFxDesc &appFxDesc) const;

    bool ShouldCreateAppFxAccordingToOccurrenceRule(const __NvThemeAppFxDesc &appFxDesc,
                                                    unsigned int totalCount,
                                                    unsigned int index) const;

    int GetTransitionAppFxOccurrenceMerit(Occurrence occurrenceMode) const;
};

class CNvThemeAssetPackage : public CNvAssetPackage
{
public:
    CNvThemeAssetPackage(CNvAssetPackageManager::AssetPackageStatus status,
                         const QUuid &assetPackageUuid,
                         const QString &assetPackageUuidString);
    ~CNvThemeAssetPackage();

public:
    QString QueryMetaFilePath() const;

    const CNvThemeDesc * GetThemeDesc(CNvAssetPackageManager *assetPackageManager,
                                      ENvStreamingEngineTimelineAspectRatio requestedAspectRatio);

protected:
    virtual CNvAssetPackageManager::AssetPackageError ReadMetaData(const QString &packageDirPath);

private:
    CNvAssetPackageManager::AssetPackageError DoReadMetaData(const QByteArray &metaData);
    void ParseTranlationTable(QXmlStreamReader &xmlReader);

private:
    QHash<QString, QString> m_translationTable;
    QHash<ENvStreamingEngineTimelineAspectRatio, CNvThemeDesc *> m_themeDescTable;
};


//////////////////////////////////////////////////////////////////////////////////
/// Capture scene package
//////////////////////////////////////////////////////////////////////////////////
class CNvCaptureSceneAssetPackage;

class CNvCaptureSceneDesc
{
    friend class CNvCaptureSceneAssetPackage;

private:
    CNvCaptureSceneDesc(bool useKeyer);
    ~CNvCaptureSceneDesc();

public:
    struct SNvCaptureSceneFilterDesc {
        bool is3DStoryboard;
        bool noBackgroundIn2DStoryboard;
        QString storyboardDescFilePath;
        bool isDynamic;
        bool isRetriggerable;

        SNvCaptureSceneFilterDesc()
        {
            is3DStoryboard = false;
            noBackgroundIn2DStoryboard = false;
            isDynamic = false;
            isRetriggerable = false;
        }
    };

    struct SNvCaptureSceneTrackClipDesc {
        QString mediaFilePath;
    };

public:
    bool UseKeyer() const { return m_useKeyer; }

    QString CaptureSceneDirPath() const;

    const QVector<SNvCaptureSceneFilterDesc> & GetCaptureTrackFilterArray() const;
    QVector<TNvSmartPtr<CNvAppFxInstance> > GenerateCaptureTrackAppFxInstanceArray(CNvProjContext *projContext) const;

    const QVector<SNvCaptureSceneFilterDesc> & GetbackgroundTrackFilterArray() const;
    QVector<TNvSmartPtr<CNvAppFxInstance> > GenerateBackgroundTrackAppFxInstanceArray(CNvProjContext *projContext) const;

    const QVector<SNvCaptureSceneFilterDesc> & GetTimelineFilterArray() const;
    QVector<TNvSmartPtr<CNvAppFxInstance> > GenerateTimelineAppFxInstanceArray(CNvProjContext *projContext) const;
    const QVector<SNvCaptureSceneTrackClipDesc> & GetBackgroundTrackClipArray() const;

private:
    // NOTE: This directory path ends with a trailing slash!
    QString m_captureSceneDirPath;

private:
    bool m_useKeyer;

    QVector<SNvCaptureSceneFilterDesc> m_captureTrackFilterArray;

    QVector<SNvCaptureSceneTrackClipDesc> m_backgroundTrackClipArray;
    QVector<SNvCaptureSceneFilterDesc> m_backgroundTrackFilterArray;

    QVector<SNvCaptureSceneFilterDesc> m_timelineFilterArray;

private:
    bool Load(const QString &captureSceneDirPath,
              ENvStreamingEngineTimelineAspectRatio requestedAspectRatio);

    void LoadFilterArray(const QJsonArray &jsFilterArray,
                         QVector<SNvCaptureSceneFilterDesc> &filterDescArray);

    QVector<TNvSmartPtr<CNvAppFxInstance> > GenerateAppFxInstanceArray(
            const QVector<SNvCaptureSceneFilterDesc> &filterDescArray,
            CNvProjContext *projContext) const;
};

class CNvCaptureSceneAssetPackage : public CNvAssetPackage
{
public:
    CNvCaptureSceneAssetPackage(CNvAssetPackageManager::AssetPackageStatus status,
                                const QUuid &assetPackageUuid,
                                const QString &assetPackageUuidString);
    ~CNvCaptureSceneAssetPackage();

public:
    bool UseKeyer() const { return m_useKeyer; }

    const CNvCaptureSceneDesc * GetCaptureSceneDesc(CNvAssetPackageManager *assetPackageManager,
                                                    ENvStreamingEngineTimelineAspectRatio requestedAspectRatio);

protected:
    virtual CNvAssetPackageManager::AssetPackageError ReadMetaData(const QString &packageDirPath);

private:
    bool m_useKeyer;

    QHash<ENvStreamingEngineTimelineAspectRatio, CNvCaptureSceneDesc *> m_captureSceneDescTable;
};

