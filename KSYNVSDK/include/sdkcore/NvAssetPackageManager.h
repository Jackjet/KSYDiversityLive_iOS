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

#include "NvStreamingEngineDef.h"
#include "NvPlatformEventHandler.h"

#include <QThread>
#include <QString>
#include <QUuid>
#include <QHash>
#include <QVector>
#include <QList>


class CNvProjContext;
class CNvAssetPackage;
class CNvVideoFxAssetPackage;
class CNvVideoTransitionAssetPackage;
class CNvCaptionStyleAssetPackage;
class CNvAnimatedStickerAssetPackage;
class CNvThemeAssetPackage;
class CNvCaptureSceneAssetPackage;
class CNvAssetPackageManagerWorker;
struct SNvAssetPackageManagerCallback;

///////////////////////////////////////////////////////////////////////////////////
// Class to manage asset package
///////////////////////////////////////////////////////////////////////////////////
class CNvAssetPackageManager : public CNvPlatformEventHandler
{
    friend class CNvAssetPackageManagerWorker;
    friend class CNvThemeAssetPackage;
    friend class CNvCaptureSceneAssetPackage;

public:
    enum AssetType
    {
        VideoFx = 0,
        VideoTransition,
        CaptionStyle,
        AnimatedSticker,
        Theme,
        CaptureScene,
        MaxType
    };

    enum AssetPackageError
    {
        NoError,
        NameError,
        AlreadyInstalledError,
        WorkingInProgress,
        NotInstalledError,
        ImproperStatusError,
        DecompressionError,
        InvalidPackageError,
        AssetTypeError,
        PermissionError,
        MetaContentError,
        SdkVersionError,
        UpgradeVersionError,
        IOError,
        ResourceError
    };

    enum AssetPackageStatus
    {
        StatusNotInstalled,
        StatusInstalling,
        StatusReady,
        StatusUpgrading
    };

public:
    CNvAssetPackageManager(CNvProjContext *projContext);
    ~CNvAssetPackageManager();

public:
    void SetCallbackInterface(SNvAssetPackageManagerCallback *pICallback);
    void SetAppId(const QString &appId);

    // Return empty string if the asset package file path doesn't contain a a valid name
    QString GetAssetPackageIdFromAssetPackageFilePath(const QString &assetPackageFilePath) const;

    // Return an error code:
    // NoError means the installation has succeeded
    // AlreadyInstalledError means an asset package with the same asset package identifier has alreay been installed
    // WorkingInProgress means installaltion was in progress (The caller must have set synchronous to true)
    // other error codes means installation failure
    //
    // assetPackageId will be set with the corresponding asset package identifier
    AssetPackageError InstallAssetPackage(const QString &assetPackageFilePath,
                                          const QString &assetPackageLicenseFilePath,
                                          AssetType assetType,
                                          bool synchronous,
                                          QString *assetPackageIdentifier = nullptr);

    // Return an error code:
    // NoError means the upgradation has succeeded
    // NotInstalledError means asset package with the same asset package identifier was not found
    // WorkingInProgress means upgradation was in progress (The caller must have set synchronous to true)
    // other error codes means upgradation failure
    //
    // assetPackageId will be set with the corresponding asset package identifier
    AssetPackageError UpgradeAssetPackage(const QString &assetPackageFilePath,
                                          const QString &assetPackageLicenseFilePath,
                                          AssetType assetType,
                                          bool synchronous,
                                          QString *assetPackageIdentifier = nullptr);

    AssetPackageError UninstallAssetPackage(const QString &assetPackageId,
                                            AssetType assetType);

    AssetPackageStatus GetAssetPackageStatus(const QString &assetPackageId, AssetType assetType);
    int GetAssetPackageVersion(const QString &assetPackageId, AssetType assetType);

    QStringList GetAssetPackageListOfType(AssetType assetType);

    CNvVideoFxAssetPackage * GetVideoFxAssetPackage(const QString &assetPackageId);
    CNvVideoTransitionAssetPackage * GetVideoTransitionAssetPackage(const QString &assetPackageId);
    CNvCaptionStyleAssetPackage * GetCaptionStyleAssetPackage(const QString &assetPackageId);
    CNvAnimatedStickerAssetPackage * GetAnimatedStickerAssetPackage(const QString &assetPackageId);
    CNvThemeAssetPackage * GetThemeAssetPackage(const QString &assetPackageId);
    CNvCaptureSceneAssetPackage * GetCaptureSceneAssetPackage(const QString &assetPackageId);

protected:
    virtual void ProcessEvent(QEvent *e);

private:
    CNvProjContext *m_projContext;

    QString m_packageRootDirPath;
    QString m_tmpPackageRootDirPath;

    SNvAssetPackageManagerCallback *m_pICallback;

    QString m_appId;

    CNvAssetPackageManagerWorker *m_worker;
    QThread *m_workerThread;

    QHash<QUuid, CNvAssetPackage *> m_assetPackageTable;
    QHash<QUuid, CNvAssetPackage *> m_assetPackageTypeTable[MaxType];
    bool m_installedAssetPackageLoaded[MaxType];

    // The following members are only used when we extract package synchronously
    QString m_extractedAssetPackageDirPath;
    AssetPackageError m_extractionError;

private:
    QString GetAssetPackageRootDirPath(AssetType assetType) const;
    QString GetTmpAssetPackageRootDirPath(AssetType assetType) const;
    QString GetAssetPackageDirName(AssetType assetType) const;

    CNvAssetPackage * FindAssetPackageById(const QString &assetPackageId) const;
    CNvAssetPackage * FindAssetPackageByUuid(const QUuid &assetPackageUuid) const;

    // NOTE: The following two methods will load installed asset package in local storage
    CNvAssetPackage * GetAssetPackageById(const QString &assetPackageId,
                                          AssetType assetType,
                                          AssetPackageError *error = nullptr);
    CNvAssetPackage * GetAssetPackageByUuid(const QUuid &assetPackageUuid,
                                            AssetType assetType,
                                            AssetPackageError *error = nullptr);

    CNvAssetPackage * CreateAssetPackageByType(AssetType assetType,
                                               AssetPackageStatus status,
                                               const QUuid &assetPackageUuid,
                                               const QString &assetPackageId) const;

    void LoadInstalledAssetPackage(AssetType assetType);
    void LoadInstalledAssetPackageFromDirPath(const QString &assetPackageDirPath,
                                              AssetType assetType);

    bool RegisterAssetPackage(CNvAssetPackage *assetPackage);
    void UnregisterAssetPackage(CNvAssetPackage *assetPackage);

    // NOTE: After calling this method, 'assetPackage' may have been deleted and/or unregistered!!!
    AssetPackageError UpdateAssetPackage(CNvAssetPackage *assetPackage,
                                         const QString &tmpAssetPackageDirPath);
    AssetPackageError FinishInstallingAssetPackage(CNvAssetPackage *assetPackage,
                                                   const QString &tmpAssetPackageDirPath);
    AssetPackageError FinishUpgradingAssetPackage(CNvAssetPackage *assetPackage,
                                                  const QString &tmpAssetPackageDirPath);

    bool EnsureWorker();
};


///////////////////////////////////////////////////////////////////////////////////
// Streaming video track callback interface
///////////////////////////////////////////////////////////////////////////////////
struct SNvAssetPackageManagerCallback
{
    virtual void AssetPackageInstallationFinished(
            const QString &in_assetPackageId,
            const QString &in_assetPackageFilePath,
            CNvAssetPackageManager::AssetType in_assetPackageType,
            CNvAssetPackageManager::AssetPackageError in_error) = 0;

    virtual void AssetPackageUpgradingFinished(
            const QString &in_assetPackageId,
            const QString &in_assetPackageFilePath,
            CNvAssetPackageManager::AssetType in_assetPackageType,
            CNvAssetPackageManager::AssetPackageError in_error) = 0;
};

