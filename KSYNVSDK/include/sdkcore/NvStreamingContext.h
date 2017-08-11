//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2016. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Dec 14. 2016
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvProjTimeline.h"
#include "NvProjCaptureVideoFx.h"
#include "NvAssetPackageManager.h"
#include "NvAssetPackage.h"
#include <NvStreamingEngineDef.h>
#include <NvIconGetter.h>

#if defined(Q_OS_IOS)
Q_FORWARD_DECLARE_OBJC_CLASS(NvsLiveWindow);
Q_FORWARD_DECLARE_OBJC_CLASS(NvsVideoFrameReceiver);
Q_FORWARD_DECLARE_OBJC_CLASS(EAGLSharegroup);
#elif defined(Q_OS_ANDROID)
class CNvsLiveWindow;
#elif defined(Q_OS_WIN) || defined(Q_OS_OSX)
class CNvLiveWindowWidget;
#endif

#include <QReadWriteLock>
#include <QThread>

#include <functional>


class QGuiApplication;
class CNvProjContext;
class CNvStreamingEngine;
class CNvIconEngine;
class CNvLiveWindowContainer;
class CNvVideoFrameSynchronizer;
class CNvStreamingCaptureFxDesc;


struct INvStreamingContextCallback
{
    virtual void NotifyPlaybackPreloadingCompletion(CNvProjTimeline *timeline) = 0;
    virtual void NotifyFirstVideoFramePresented(CNvProjTimeline *timeline) = 0;
    virtual void NotifyCaptureDeviceCapsReady(unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyCaptureDevicePreviewResolutionReady(unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyCaptureDevicePreviewStarted(unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyCaptureDeviceError(unsigned int in_captureDeviceIndex, HRESULT in_errorCode) = 0;
    virtual void NotifyCaptureDeviceStopped(unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyCaptureDeviceAutoFocusComplete(unsigned int in_captureDeviceIndex, bool in_succeeded) = 0;
    virtual void NotifyCaptureRecordingFinished(unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyCaptureRecordingError(unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyPlaybackTimelinePosition(CNvProjTimeline *in_timeline, int64_t in_position) = 0;
    virtual void NotifyPlaybackStopped(CNvProjTimeline *in_timeline) = 0;
    virtual void NotifyPlaybackEOF(CNvProjTimeline *in_timeline) = 0;
    virtual void NotifyCompileProgress(CNvProjTimeline *in_timeline, int in_progress) = 0;
    virtual void NotifyCompileFinished(CNvProjTimeline *in_timeline) = 0;
    virtual void NotifyCompileCompleted(CNvProjTimeline *in_timeline, bool in_isCanceled) = 0;
    virtual void NotifyCompileFailed(CNvProjTimeline *in_timeline) = 0;
    virtual void NotifyStreamingEngineStateChanged(ENvStreamingEngineState in_state) = 0;
};


class CNvStreamingContext : public INvStreamingEngineCallback

{
    friend class CNvProjContext;
    friend class CNvLiveWindowContainer;
    friend class CNvVideoFrameSynchronizer;
#if defined(Q_OS_ANDROID)
    friend class CNvsLiveWindow;
#endif

public:
    enum StreamingContextFlag
    {
        StreamingContextFlagNone = 0,
        StreamingContextFlagSupport4KEdit = 1
    };

    enum CompileVideoResolutionGrade
    {
        CompileVideoResolutionGrade360 = 0,
        CompileVideoResolutionGrade480,
        CompileVideoResolutionGrade720,
        CompileVideoResolutionGrade1080,
        CompileVideoResolutionGrade2160,
        CompileVideoResolutionGradeCustom = 256
    };

    enum CompileVideoBitrateGrade
    {
        CompileBitrateGradeLow = 0,
        CompileBitrateGradeMedium,
        CompileBitrateGradeHigh
    };

    enum VideoCaptureResolutionGrade
    {
        VideoCaptureResolutionGradeLow = 0,
        VideoCaptureResolutionGradeMedium,
        VideoCaptureResolutionGradeHigh,
        VideoCaptureResolutionGradeSuperHigh,
    };

    enum VideoPreviewSizeMode
    {
        VideoPreviewSizeModeFullSize = 0,
        VideoPreviewSizeModeLiveWindowSize
    };

    enum CaptureFlag
    {
        CaptureFlag_None = 0,
        CaptureFlag_GrabCapturedVideoFrame = 1,
        CaptureFlag_LiveStreaming = 2,
        CaptureFlag_DontUseSystemRecorder = 4,
        CaptureFlag_StrictPreviewVideoSize = 8,
        CaptureFlag_DontCaptureAudio = 16
    };

    enum RecordingFlag
    {
        RecordingFlag_None = 0,
        RecordingFlag_RecordWithFx = 1,
        RecordingFlag_VideoIntraFrameOnly = 2
    };

    enum CompileFlag
    {
        CompileFlag_None = 0,
        CompileFlag_DisableHardwareEncoder = 1
    };

private:
    CNvStreamingContext(StreamingContextFlag flags);
    virtual ~CNvStreamingContext();

public:
    // Return nullptr on error
    static CNvStreamingContext * Init(std::function<void(CNvProjObject *)> projObjDestructionCallback = nullptr,
                                      StreamingContextFlag flags = StreamingContextFlagNone);
    static void Close();
    static void SetupLocale();

    static CNvStreamingContext * GetInstance();

public:
    void SetCallback(INvStreamingContextCallback *pIStreamingContextCallback);

    void SetUserData(uintptr_t userData) { m_userData = userData; }
    uintptr_t GetUserData() const { return m_userData; }

    bool SetDefaultThemeEndingLogoImageFilePath(const QString &logoImageFilePath);
    QString GetDefaultThemeEndingLogoImageFilePath() const;

    CNvAssetPackageManager * GetAssetPackageManager();
    CNvAppFxDesc * GetVideoAppFxDesc(const QByteArray &appFxName);
    CNvAppFxDesc * GetAudioAppFxDesc(const QByteArray &appFxName);
    QVector<QByteArray> GetAllBuiltinVideoFxNames();
    QVector<QByteArray> GetAllBuiltinAudioFxNames();
    QVector<QByteArray> GetAllBuiltinVideoTransitionNames();
    QVector<QByteArray> GetAllBuiltinCaptureVideoFxNames();
    const char * GetDefaultVideoTransitionName();
    const char * GetDefaultAudioTransitionName();

    // NOTE: This method is thread-safe
    bool GetAVFileInfo(const QString &avFilePath, SNvAVFileInfo &avFileInfo);

    // Return 0 means detection failed
    unsigned int DetectVideoFileKeyframeInterval(const QString &videoFilePath) const;

#ifdef Q_OS_IOS
    EAGLSharegroup * GetEAGLSharegroup();
#endif

    // NOTE: The caller will obtain a reference count of the created timeline
    bool CreateTimeline(const SNvVideoResolution &videoEditRes,
                        const SNvRational &videoFps,
                        const SNvAudioResolution &audioEditRes,
                        CNvProjTimeline **createdTimeline = nullptr);
    bool DestroyTimeline(CNvProjTimeline *timeline);

    ENvStreamingEngineState StreamingEngineState() const;

    int64_t GetTimelineCurrentPosition(CNvProjTimeline *timeline);

    bool SeekTimeline(CNvProjTimeline *timeline,
                      int64_t timestamp,
                      VideoPreviewSizeMode videoSizeMode,
                      int flags = 0 /* Bitwise OR of keNvStreamingEngineSeekFlag_XXX */);

    bool SeekTimeline(CNvProjTimeline *timeline,
                      int64_t timestamp,
                      const SNvRational &proxyScale,
                      int flags = 0 /* Bitwise OR of keNvStreamingEngineSeekFlag_XXX */);

    QImage GrabImageFromTimline(CNvProjTimeline *timeline,
                                int64_t timestamp,
                                int proxyScaleNumerator,
                                int proxyScaleDenominator);

    bool PlaybackTimeline(CNvProjTimeline *timeline,
                          int64_t startTime,
                          int64_t endTime,
                          VideoPreviewSizeMode videoSizeMode,
                          bool preloadResources,
                          int flags = 0/* Bitwise OR of keNvStreamingEnginePlaybackFlag_XXX */);

    bool PlaybackTimelineWithProxyScale(CNvProjTimeline *timeline,
                                        int64_t startTime,
                                        int64_t endTime,
                                        const SNvRational &proxyScale,
                                        bool preloadResources,
                                        int flags = 0/* Bitwise OR of keNvStreamingEnginePlaybackFlag_XXX */);

    bool CompileTimeline(CNvProjTimeline *timeline,
                         int64_t startTime,
                         int64_t endTime,
                         const QString &outputFilePath,
                         CompileVideoResolutionGrade videoResolutionGrade,
                         CompileVideoBitrateGrade videoBitrateGrade,
                         float videoBitrateMultiplier,
                         int flags); // Bitwise OR of CompileFlag_XXX

    void SetCustomCompileVideoHeight(unsigned int videoHeight);
    unsigned int CustomCompileVideoHeight() const { return m_customCompileVideoHeight; }

    void Stop();

    void ClearCacheInStreamingEngine(int components, // Bitwise OR of keNvStreamingEngineComponent_XXX
                                     int resourceTypes, // Bitwise OR of keNvStreamingEngineResourceType_XXX
                                     bool async);

    void ClearCacheInIconEngine();

    void ClearAVFileInfoCache(const QString &avFilePath = QString());

public:
    //
    // Capture device related methods
    //
    unsigned int CaptureDeviceCount();
    bool IsCaptureDeviceBackFacing(unsigned int captureDeviceIndex);

    // NOTE: This methods will only return valid capture device capabilities
    // when INvStreamingContextCallback::NotifyCaptureDeviceCapsReady has been signaled
    const SNvCaptureDeviceCaps * GetCaptureDeviceCaps(unsigned int captureDeviceIndex);

    QSize GetCapturePreviewVideoSize(unsigned int captureDeviceIndex);

    bool SetLiveStreamingEndPoint(const QString &liveStreamingEndPoint);

    bool StartCapturePreview(unsigned int captureDeviceIndex,
                             VideoCaptureResolutionGrade resolutionGrade,
                             int flags = 0, // Bitwise OR of CaptureFlag_XXX
                             int fixedPreviewAspectRatioNum = 0,
                             int fixedPreviewAspectRatioDen = 0);

    bool StartRecording(const QString &outputFilePath,
                        float videoBitrateMultiplier,
                        int flags); // Bitwise OR of RecordingFlag_XXX
    void StopRecording();

    // NOTE: The caller will obtain a reference count of the created capture video FX
    bool AppendBuiltinCaptureVideoFx(const QByteArray &videoFxName,
                                     CNvProjCaptureVideoFx **videoFx = nullptr);
    bool InsertBuiltinCaptureVideoFx(const QByteArray &videoFxName,
                                     unsigned int insertPosition,
                                     CNvProjCaptureVideoFx **videoFx = nullptr);
    bool AppendPackagedCaptureVideoFx(const QString &videoFxPackageId,
                                      CNvProjCaptureVideoFx **videoFx = nullptr);
    bool InsertPackagedCaptureVideoFx(const QString &videoFxPackageId,
                                      unsigned int insertPosition,
                                      CNvProjCaptureVideoFx **videoFx = nullptr);
    bool AppendCustomCaptureVideoFx(INvCustomGpuVideoEffect *pICustomGpuVideoEffect,
                                    CNvProjCaptureVideoFx **videoFx = nullptr);
    bool InsertCustomCaptureVideoFx(INvCustomGpuVideoEffect *pICustomGpuVideoEffect,
                                    unsigned int insertPosition,
                                    CNvProjCaptureVideoFx **videoFx = nullptr);
    bool RemoveCaptureVideoFx(unsigned int videoFxIndex);
    void RemoveAllCaptureVideoFx();
    unsigned int CaptureVideoFxCount() const { return m_captureVideoFxArray.size(); }
    CNvProjCaptureVideoFx * GetCaptureVideoFxByIndex(unsigned int CaptureVideoFxIndex) const;

    bool ApplyCaptureScene(const QString &captureSceneId);
    QString CaptureSceneId() const { return m_captureSceneId; }
    void RemoveCurrentCaptureScene();

    void SampleColorFromCapturedVideoFrame(const SNvRectF *sampleRect, // In live window's coordinates
                                           SNvColor *sampledColor,
                                           int64_t *sampledFrameStreamTime = nullptr);

    // Specify focus rectangle with left, top, right, bottom in preview live window's own coorindates
    void StartAutoFocus(const SNvRectF *focusArea);
    void CancelAutoFocus();
    void StartAutoExposure(const SNvRectF *exposureArea);
    void SetZoom(int zoom);
    int GetZoom() const;
    void SetZoomRatio(float zoomRatio);
    float GetZoomRatio() const;
    void ToggleFlash(bool on);
    bool IsFlashOn() const;
    void SetExposureCompensation(int exposureCompensation);
    int GetExposureCompensation() const;
    void SetExposureCompensationValue(float exposureCompensationValue);
    float GetExposureCompensationValue() const;

public:
    //
    // Live window control related methods
    //
#if defined(Q_OS_IOS)
    // NOTE: Pass liveWindow with nil to disconnect timeline from live window
    bool ConnectTimelineWithLiveWindowIOS(CNvProjTimeline *timeline,
                                          NvsLiveWindow *liveWindow);
    // NOTE: Pass receiver with nil to disconnect timeline from live window
    bool ConnectTimelineWithVideoFrameReceiverIOS(CNvProjTimeline *timeline,
                                                  NvsVideoFrameReceiver *receiver);
    // NOTE: Pass liveWindow with nil to disconnect timeline from capture preview
    bool ConnectCapturePreviewWithLiveWindowIOS(NvsLiveWindow *liveWindow);
    // NOTE: Pass receiver with nil to disconnect timeline from capture preview
    bool ConnectCapturePreviewWithVideoFrameReceiverIOS(NvsVideoFrameReceiver *receiver);
    void DisconnectLiveWindowIOS(NvsLiveWindow *liveWindow);
#elif defined(Q_OS_ANDROID)
    // NOTE: Pass liveWindow with nullptr to disconnect timeline from live window
    bool ConnectTimelineWithLiveWindowAndroid(CNvProjTimeline *timeline, CNvsLiveWindow *liveWindow);
    // NOTE: Pass liveWindow with nullptr to disconnect timeline from capture preview
    bool ConnectCapturePreviewWithLiveWindowAndroid(CNvsLiveWindow *liveWindow);
    void DisconnectLiveWindowAndroid(CNvsLiveWindow *liveWindow);
#elif defined(Q_OS_WIN) || defined(Q_OS_OSX)
    // NOTE: Pass liveWindow with nullptr to disconnect timeline from live window
    bool ConnectTimelineWithLiveWindow(CNvProjTimeline *timeline,
                                       CNvLiveWindowWidget *liveWindow);
    // NOTE: Pass liveWindow with nullptr to disconnect timeline from capture preview
    bool ConnectCapturePreviewWithLiveWindow(CNvLiveWindowWidget *liveWindow);
    void DisconnectLiveWindow(CNvLiveWindowWidget *liveWindow);
#endif

public:
    //
    // Icon engine related
    //
    int64_t GetIcon(CNvIconGetter *iconGetter, const QString &mediaFilePath, int64_t timestamp);
    bool GetIconFromCache(const QString &mediaFilePath, int64_t timestamp, CNvUiImage **outImage);
    int64_t GetThumbnail(CNvIconGetter *iconGetter, const QString &mediaFilePath, bool bypassCache);

    void PauseIconEngine();
    void ResumeIconEngine();

    void CancelIconTask(int64_t taskId);
    void CancelIconTaskByGetter(CNvIconGetter *iconGetter);
    void CancelAllIconTasks();

private:
    // liveWindowNativeHandle is a platform specific live window handle
    // iOS: NvsLiveWindow object pointer
    // Android: TODO:
    // Windows and macOS: CNvLiveWindowWidget object pointer
    bool DoConnectTimelineWithLiveWindow(CNvProjTimeline *timeline,
                                         uintptr_t liveWindowNativeHandle);
    bool DoConnectCapturePreviewWithLiveWindow(uintptr_t liveWindowNativeHandle);
    void DoDisconnectLiveWindow(uintptr_t liveWindowNativeHandle);

private:
    //
    // INvStreamingEngineCallback callback
    //
    virtual void VideoFrameArrived(INvVideoFrame *in_pIVideoFrame,
                                   int64_t in_streamTime,
                                   int in_liveWindowId);

    virtual void NotifyPlaybackPreloadingCompletion(int in_timelineId);
    virtual void NotifyFirstVideoFramePresented(int in_timelineId);
    virtual void NotifyCaptureDeviceCaps(unsigned int in_captureDeviceIndex);
    virtual void NotifyCaptureDevicePreviewResolutionReady(unsigned int in_captureDeviceIndex);
    virtual void NotifyCaptureDevicePreviewStarted(unsigned int in_captureDeviceIndex);
    virtual void NotifyCaptureDeviceError(unsigned int in_captureDeviceIndex, HRESULT in_errorCode);
    virtual void NotifyCaptureDeviceStopped(unsigned int in_captureDeviceIndex);
    virtual void NotifyCaptureDeviceAutoFocusComplete(bool in_success, unsigned int in_captureDeviceIndex);
    virtual void NotifyCaptureRecordingFinished(unsigned int in_captureDeviceIndex);
    virtual void NotifyCaptureRecordingError(unsigned int in_captureDeviceIndex);
    virtual void NotifyPlaybackTimelinePosition(int in_timelineId, int64_t in_position);
    virtual void NotifyPlaybackStopped(int in_timelineId);
    virtual void NotifyPlaybackEOF(int in_timelineId);
    virtual void NotifyCompileProgress(int in_timelineId, int in_progress);
    virtual void NotifyCompileFinished(int in_timelineId);
    virtual void NotifyCompileCompleted(int in_timelineId, bool in_isCanceled);
    virtual void NotifyCompileFailed(int in_timelineId);
    virtual void NotifyStateChanged(ENvStreamingEngineState in_state);

private:
    void InvalidateTimelineFromEngine(CNvProjTimeline *timeline);
    void NotifyProjObjectDestruction(CNvProjObject *projObject);

    const CNvCaptureSceneDesc * GetCaptureSceneDesc(const QString &captureSceneId,
                                                    ENvStreamingEngineTimelineAspectRatio requestedAspectRatio) const;

private:
    INvStreamingContextCallback *m_pIStreamingContextCallback;
    std::function<void(CNvProjObject *)> m_projObjDestructionCallback;

    StreamingContextFlag m_flags;
    uintptr_t m_userData;

    CNvStreamingEngine *m_streamingEngine;
    CNvIconEngine *m_iconEngine;
    CNvProjContext *m_projContext;

    unsigned int m_customCompileVideoHeight;

    QString m_liveStreamingEndPoint;

    bool m_captureDevicePreviewResolutionReady;

    // NOTE: You can't get any GPU related information before streaming engine has been created
    CNvHardwareInfo *m_hardwareInfo;

    QReadWriteLock m_avFileInfoLock;
    QHash<QString, SNvAVFileInfo *> m_avFileInfoTable;

    struct __SNvTimelineInfo {
        int engineTimelineId; // 0 indicate we need to rebuild it from engine
        CNvLiveWindowContainer *liveWindow; // Currently connected live window
        int64_t cachedTimelinePos;

        __SNvTimelineInfo()
        {
            engineTimelineId = 0;
            liveWindow = nullptr;
            cachedTimelinePos = 0;
        }
    };
    QHash<TNvSmartPtr<CNvProjTimeline>, __SNvTimelineInfo *> m_timelineTable;

    //
    // Live window related
    //

    // Currently in used live window table, Key is live window native handle
    QHash<uintptr_t, CNvLiveWindowContainer *> m_liveWindowHandleTable;

    CNvLiveWindowContainer *m_liveWindowConnectedWithCapturePreview;

    QReadWriteLock m_liveWindowLock;
    // Currently in used live window table, Key is live window indentifier
    QHash<int, CNvLiveWindowContainer *> m_liveWindowIdTable;

    CNvVideoFrameSynchronizer *m_videoFrameSynchronizer;
    QThread *m_liveWindowHandlerThread;

private:
    static CNvStreamingContext *m_instance;

private:
    bool EnsureStreamingEngine();
    bool EnsureIconEngine();
    bool EnsureProjContext();

    __SNvTimelineInfo * GetTimelineInfo(CNvProjTimeline *timeline) const;
    CNvProjTimeline * GetTimelineByEngineTimelineId(int engineTimelineId) const;

    // Return positive engine timeline identifier on success, negative value on failure
    int PrepareTimelineForEngine(CNvProjTimeline *timeline);

    bool DoSeekTimeline(CNvProjTimeline *timeline,
                        int64_t timestamp,
                        const SNvVideoResolution &videoOutputResolution,
                        int flags /* Bitwise OR of keNvStreamingEngineSeekFlag_XXX */);

    bool DoPlaybackTimeline(CNvProjTimeline *timeline,
                            qint64 startTime,
                            qint64 endTime,
                            const SNvVideoResolution &videoOutputResolution,
                            bool preloadResources,
                            int flags /* keNvStreamingEnginePlaybackFlag_XXX */);

    bool DetermineVideoOutputResolution(CNvProjTimeline *timeline,
                                        const __SNvTimelineInfo *timelineInfo,
                                        VideoPreviewSizeMode videoSizeMode,
                                        SNvVideoResolution &videoOutputRes) const;
    void DetermineVideoOutputResolution(CNvProjTimeline *timeline,
                                        const SNvRational &proxyScale,
                                        SNvVideoResolution &videoOutputRes) const;

    // NOTE: the live window size should be the physical pixel size (with device pixel ratio multiplied)
    void CalcBestVideoOutputResolutionFromLiveWindowSize(const SNvVideoResolution &videoEditRes,
                                                         const QSize &liveWindowSize,
                                                         SNvVideoResolution &videoOutputRes) const;
    qreal GetVideoOutputSizeAmendmentFactor() const;

    void DetermineCompileVideoResolution(const SNvVideoResolution &videoEditRes,
                                         ENvStreamingEngineTimelineAspectRatio aspectRatio,
                                         CompileVideoResolutionGrade videoResolutionGrade,
                                         SNvVideoResolution &compileVideoRes);

    void CalcCompileVideoResolutionProxyScale(const SNvVideoResolution &videoEditRes,
                                              SNvVideoResolution &compileVideoRes);

    void CalcVideoSizeAccordingToVideoResolutionGrade(CompileVideoResolutionGrade videoResolutionGrade,
                                                      const SNvVideoResolution &videoEditRes,
                                                      unsigned int &width,
                                                      unsigned int &height) const;

    void AlignVideoOutputSize(unsigned int &width, unsigned int &height) const;

private:
    //
    // Capture Fx related
    //
    QVector<TNvSmartPtr<CNvProjCaptureVideoFx> > m_captureVideoFxArray;

    QString m_captureSceneId;

private:
    CNvStreamingCaptureFxDesc * CreateCaptureFxDesc();

    void SyncCaptureFxWithEngine();

#ifdef Q_OS_ANDROID
public:
    int GetEngineFlagsForAndroid();

private:
    enum AndroidDevicePowerGrade
    {
        AndroidDevicePowerGradeLow = 0,
        AndroidDevicePowerGradeMedium,
        AndroidDevicePowerGradeHigh
    };
    AndroidDevicePowerGrade m_androidDevicePowerGrade;
    AndroidDevicePowerGrade DetectAndroidDevicePowerGrade() const;

    bool DetermineCompileVideoResolutionByAnroidExceptionList(const SNvVideoResolution &videoEditRes,
                                                              ENvStreamingEngineTimelineAspectRatio aspectRatio,
                                                              CompileVideoResolutionGrade videoResolutionGrade,
                                                              SNvVideoResolution &compileVideoRes);

    CompileVideoResolutionGrade AdjustCompileVideoResolutionGradeAndroid(CompileVideoResolutionGrade videoResolutionGrade);

    QSize CheckAndroidVideoSizeExceptionList(const SNvVideoResolution &editVideoRes,
                                             ENvStreamingEngineTimelineAspectRatio aspectRatio,
                                             CompileVideoResolutionGrade videoResolutionGrade,
                                             const QString &hardwareManufacturer,
                                             const QString &hardwareModel);
    void EnsureAndroidVideoSizeExceptionList(ENvStreamingEngineTimelineAspectRatio aspectRatio);
    void LoadAndroidVideoSizeExceptionList(const QString &exceptionFilePath,
                                           QHash<QString, QVector<QSize> > &videoSizeTable);
    QString GenerateAndroidVideoSizeExceptionListDeviceKey(const QString &hardwareManufacturer,
                                                           const QString &hardwareModel) const;

    // Key is the lower-case version of manufacturer + model string, value is supported video size array
    QHash<QString, QVector<QSize> > m_androidVideoSizeTable;
    // Key is the lower-case version of manufacturer + model string, value is supported video size array
    QHash<QString, QVector<QSize> > m_androidVideoSizeTable1v1;

    struct SNvSurfaceTextureLiveWindow {
        QAndroidJniObject jSurfaceTexture;
        QAndroidJniObject jSurface;
        TNvSmartPtr<CNvsLiveWindow> liveWindow;
        SNvRational proxyScale;
        bool usedByCapturePreivew;
    };

private:
    QVector<SNvSurfaceTextureLiveWindow> m_surfaceTextureLiveWindowArray;

public:
    void RegisterSurfaceTextureLiveWindow(jobject jSurfaceTexture,
                                          jobject jSurface,
                                          CNvsLiveWindow *liveWindow,
                                          const SNvRational &proxyScale,
                                          bool usedByCapturePreivew);
    void UnregisterSurfaceTextureLiveWindow(CNvsLiveWindow *liveWindow);
    CNvsLiveWindow * FindSurfaceTextureLiveWindow(jobject jSurfaceTexture) const;
    SNvRational GetSurfaceTextureLiveWindowProxyScale(CNvsLiveWindow *liveWindow) const;
    void FinalizeSurfaceTextureLiveWindowConnectedWithCapturePreview(unsigned int videoWidth, unsigned int videoHeight);

private:
    CNvsLiveWindow * GetSurfaceTextureLiveWindowConnectedWithTimeline(const __SNvTimelineInfo *timelineInfo) const;

#endif

#ifdef Q_OS_IOS
private:
    static bool InitIOS();
    static void CloseIOS();
    CNvVideoFrameSynchronizer * CreateVideoFrameSynchronizerIOS();
    static QGuiApplication *m_qtApp;

    CompileVideoResolutionGrade AdjustCompileVideoResolutionGradeIOS(CompileVideoResolutionGrade videoResolutionGrade);
#endif
};

