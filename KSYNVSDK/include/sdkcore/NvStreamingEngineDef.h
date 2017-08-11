//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Mar 14. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <NvDef.h>
#include <NvEffectDef.h>
#include <NvVideoEffectDef.h>
#include <INvVideoFrame.h>

#include <QList>

#include <math.h>


#define NV_STREAMING_PREROLL_TIME_MS                        400
#define NV_STREAMING_VIDEO_SOURCE_DEFAULT_PIPELINE_SIZE     6
#define NV_STREAMING_VIDEO_SOURCE_PIPELINE_SIZE_4K          4
#define NV_STREAMING_VIDEO_REF_WIDTH                        1920
#define NV_STREAMING_VIDEO_REF_HEIGHT                       1080
#define NV_STREAMING_VIDEO_REF_WIDTH_4K                     3840
#define NV_STREAMING_VIDEO_REF_HEIGHT_4K                    2160

#define NV_STREAMING_AUDIO_FRAME_SIZE                       1024
#define NV_STREAMING_AUDIO_SOURCE_PIPELINE_SIZE_MS          300
#define NV_STREAMING_AUDIO_SOURCE_PIPELINE_SIZE             (NV_STREAMING_AUDIO_SOURCE_PIPELINE_SIZE_MS * 48000 / (NV_STREAMING_AUDIO_FRAME_SIZE * 1000))

#define NV_STREAMING_EVENT_PRIORITY                         Qt::HighEventPriority


//
// Streaming engine flags
//
#define NV_STREAMING_ENGINE_FLAG_NO_HARDWARE_VIDEO_READER               1
#define NV_STREAMING_ENGINE_FLAG_DONT_SET_CAMERA_PARAM_ON_RECORDING     2
#define NV_STREAMING_ENGINE_FLAG_SUPPORT_4K_EDIT                        4

//
// Streaming engine state
//
enum ENvStreamingEngineState
{
    keNvStreamingEngineState_Stopped = 0,
    keNvStreamingEngineState_CapturePreview,
    keNvStreamingEngineState_CaptureRecording,
    keNvStreamingEngineState_Playback,
    keNvStreamingEngineState_Seeking,
    keNvStreamingEngineState_Compile
};

//
// Streaming engine component
//
enum ENvStreamingEngineComponent
{
    keNvStreamingEngineComponent_VideoSource = 1,
    keNvStreamingEngineComponent_AudioSource = 2,
    keNvStreamingEngineComponent_VideoProcessor = 4,
    keNvStreamingEngineComponent_FileWriter = 8,
    keNvStreamingEngineComponent_Output = 16
};

//
// Streaming engine resource type
//
enum ENvStreamingEngineResourceType
{
    keNvStreamingEngineResourceType_Frame = 1,
    keNvStreamingEngineResourceType_Reader = 2,
    keNvStreamingEngineResourceType_Effect = 4
};

// Streaming engine seek flag
enum ENvStreamingEngineSeekFlag
{
    keNvStreamingEngineSeekFlag_GrabVideoFrame = 1,
    keNvStreamingEngineSeekFlag_ShowCaptionPoster = 2,
    keNvStreamingEngineSeekFlag_ShowAnimatedStickerPoster = 4,
    keNvStreamingEngineSeekFlag_AccurateSeek = 8
};

// Streaming engine playback flag
enum ENvStreamingEnginePlaybackFlag
{
    keNvStreamingEnginePlaybackFlag_IgnoreTiming = 1,
    keNvStreamingEnginePlaybackFlag_GrabPreFxVideoFrame = 2,
    keNvStreamingEnginePlaybackFlag_GrabPostFxVideoFrame = 4
};

enum ENvStreamingEngineClipType
{
    keNvStreamingEngineClipTypeVideo = 0,
    keNvStreamingEngineClipTypeImage,
    keNvStreamingEngineClipTypeAudio
};

enum ENvStreamingEngineClipWrapMode
{
    keNvStreamingEngineClipWrapModeInvalid = -1,
    keNvStreamingEngineClipWrapModeRepeatLastFrame = 0,
    keNvStreamingEngineClipWrapModeRepeat
};

enum ENvStreamingEngineImageClipMotionMode
{
    keNvStreamingEngineImageClipMotionMode_LetterBoxZoomIn = 0,
    keNvStreamingEngineImageClipMotionMode_LetterBoxZoomOut,
    keNvStreamingEngineImageClipMotionMode_ROI
};

enum ENvStreamingEngineTimelineAspectRatio
{
    keNvStreamingEngineTimelineAspectRatio_16v9 = 1,
    keNvStreamingEngineTimelineAspectRatio_1v1 = 2,
    keNvStreamingEngineTimelineAspectRatio_9v16 = 4
};

ENvStreamingEngineTimelineAspectRatio NvGetStreamingEngineTimelineAspectRatio(float videoAspectRatio);


///////////////////////////////////////////////////////////////////////////////////
// Timeline callback interface
///////////////////////////////////////////////////////////////////////////////////
struct SNvTimelineCallback
{
    // Video effect related methods
    virtual bool GetVideoEffectDesc(unsigned int in_filterEffectIndex, INvEffectDescriptor **out_ppIEffectDesc) = 0;
    virtual bool FillVideoEffectSettings(unsigned int in_filterEffectIndex, INvEffectSettings *io_pIEffectSettings) = 0;
    virtual bool IsCaptionVideoEffect(unsigned int in_filterEffectIndex) = 0;
    virtual bool IsAnimatedStickerVideoEffect(unsigned int in_filterEffectIndex) = 0;
    virtual bool GetEffectTimeRange(unsigned int in_filterEffectIndex, int64_t *out_startTime, int64_t *out_endTime) = 0;

    virtual void GetCustomGpuEffect(unsigned int in_filterEffectIndex, INvCustomGpuVideoEffect **out_ppICustomGpuVideoEffect) = 0;

    // Audio effect related methods
    virtual bool GetAudioEffectDesc(unsigned int in_filterEffectIndex, INvEffectDescriptor **out_ppIEffectDesc) = 0;
    virtual bool FillAudioEffectSettings(unsigned int in_filterEffectIndex, INvEffectSettings *io_pIEffectSettings) = 0;
};

///////////////////////////////////////////////////////////////////////////////////
// Streaming video track callback interface
///////////////////////////////////////////////////////////////////////////////////
struct SNvStreamingVideoTrackCallback
{
    // Video effect related methods
    virtual bool GetEffectDesc(unsigned int in_filterEffectIndex, INvEffectDescriptor **out_ppIEffectDesc) = 0;
    virtual bool FillEffectSettings(unsigned int in_filterEffectIndex, INvEffectSettings *io_pIEffectSettings) = 0;
    virtual bool IsCaptionVideoEffect(unsigned int in_filterEffectIndex) = 0;
    virtual bool IsAnimatedStickerVideoEffect(unsigned int in_filterEffectIndex) = 0;
    virtual bool GetEffectTimeRange(unsigned int in_filterEffectIndex, int64_t *out_startTime, int64_t *out_endTime) = 0;
};

///////////////////////////////////////////////////////////////////////////////////
// Clip callback interface
///////////////////////////////////////////////////////////////////////////////////
struct SNvClipCallback
{
    virtual bool GetAVFileInfo(SNvAVFileInfo *out_avFileInfo) = 0;

    virtual bool GetEffectDesc(unsigned int in_filterEffectIndex, INvEffectDescriptor **out_ppIEffectDesc) = 0;
    virtual bool IsRangedFilter(unsigned int in_filterEffectIndex) = 0;
    virtual bool GetFilterEffectTimeRange(unsigned int in_filterEffectIndex, int64_t *out_startTime, int64_t *out_endTime) = 0;
    virtual bool FillEffectSettings(unsigned int in_filterEffectIndex, INvEffectSettings *io_pIEffectSettings) = 0;

    virtual void GetCustomGpuEffect(unsigned int in_filterEffectIndex, INvCustomGpuVideoEffect **out_ppICustomGpuVideoEffect) = 0;

    virtual unsigned int ExternalSourceCount(unsigned int in_filterEffectIndex) = 0;
    virtual QString ExternalSourcePath(unsigned int in_filterEffectIndex, unsigned int in_externalSourceIndex) = 0;

    // Used only by audio clip
    virtual void GetVolumeGain(float *out_gainLeft, float *out_gainRight, int64_t in_streamTime) = 0;

    // Used only by image clip
    virtual ENvStreamingEngineImageClipMotionMode GetImageMotionMode() = 0;
    virtual bool IsImageMotionAnimationDisabled() = 0;
    virtual void GetImageMotionROIs(SNvRectF *in_startROI, SNvRectF *in_targetROI) = 0;

    // Used only by video and image clip
    virtual ENvVideoRotation GetExtraVideoRotation() = 0;

    // Used only by video and image clip, return false if we don't apply pan and scan to this video clip
    virtual bool GetPanAndScanParameters(float *out_pan, float *out_scan) = 0;
    // Used only by video and image clip, return false if we don't apply sourceBackgroundMode to this video clip
    virtual ENvSourceBackgroundMode GetSourceBackgroundModeParameter() = 0;
};


///////////////////////////////////////////////////////////////////////////////////
// Transition callback interface
///////////////////////////////////////////////////////////////////////////////////
struct SNvTransitionCallback
{
    virtual bool GetEffectDesc(INvEffectDescriptor **out_ppIEffectDesc) = 0;
    virtual bool FillEffectSettings(INvEffectSettings *io_pIEffectSettings) = 0;

    virtual bool GetTransitionFilterEffectDesc(unsigned int in_filterEffectIndex, INvEffectDescriptor **out_ppIEffectDesc) = 0;
    virtual bool FillTransitionFilterEffectSettings(unsigned int in_filterEffectIndex, INvEffectSettings *io_pIEffectSettings) = 0;
};


// NOTE: All the methods except VideoFrameArrived() in this callback interface
// will be called from the thread where you created streaming engine
struct INvStreamingEngineCallback
{
    // NOTE: This method will be called from internal thread of streaming engine!
    virtual void VideoFrameArrived(INvVideoFrame *in_pIVideoFrame,
                                   int64_t in_streamTime,
                                   int in_liveWindowId) = 0;

    virtual void NotifyPlaybackPreloadingCompletion(int in_timelineId) = 0;
    virtual void NotifyFirstVideoFramePresented(int in_timelineId) = 0;
    virtual void NotifyCaptureDeviceCaps(unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyCaptureDevicePreviewResolutionReady(unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyCaptureDevicePreviewStarted(unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyCaptureDeviceError(unsigned int in_captureDeviceIndex, HRESULT in_errorCode) = 0;
    virtual void NotifyCaptureDeviceStopped(unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyCaptureDeviceAutoFocusComplete(bool in_success, unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyCaptureRecordingFinished(unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyCaptureRecordingError(unsigned int in_captureDeviceIndex) = 0;
    virtual void NotifyPlaybackTimelinePosition(int in_timelineId, int64_t in_position) = 0;
    virtual void NotifyPlaybackStopped(int in_timelineId) = 0;
    virtual void NotifyPlaybackEOF(int in_timelineId) = 0;
    virtual void NotifyCompileProgress(int in_timelineId, int in_progress) = 0;
    virtual void NotifyCompileFinished(int in_timelineId) = 0;
    virtual void NotifyCompileCompleted(int in_timelineId, bool in_isCanceled) = 0;
    virtual void NotifyCompileFailed(int in_timelineId) = 0;
    virtual void NotifyStateChanged(ENvStreamingEngineState in_state) = 0;
};

// NOTE: this callback will be called from a thread depending on platform
struct INvStreamingCapturedVideoFrameGrabberCallback
{
    virtual void CapturedVideoFrameGrabbed(INvVideoFrame *in_pICapturedVideoFrame, int64_t in_streamTime) = 0;
};

// NOTE: this callback will be called from one of the thread of streaming engine
struct INvStreamingVideoFrameGrabberCallback
{
    virtual ENvPixelFormat QueryVideoFramePixelFormat() = 0;
    virtual void VideoFrameGrabbed(INvVideoFrame *in_pIGrabbedVideoFrame, int64_t in_streamTime) = 0;
};


//
// Capture device capability
//
struct SNvCaptureDeviceCaps
{
    bool supportAutoFocus;
    bool supportAutoExposure;

    // Zoom related
    bool supportZoom;
    bool supportSteplessZoom;
    int maxZoom;
    QList<float> zoomRatios;

    // Flash related
    bool supportFlash;

    // Exposure Compensation related
    bool supportExposureCompensation;
    bool supportSteplessExposureCompensation;
    int minExposureCompensation;
    int maxExposureCompensation;
    float exposureCompensationStep;

public:
    SNvCaptureDeviceCaps()
    {
        supportAutoFocus = false;
        supportAutoExposure = false;
        supportZoom = false;
        supportSteplessZoom = false;
        maxZoom = 0;
        supportFlash = false;
        supportExposureCompensation = false;
        supportSteplessExposureCompensation = false;
        minExposureCompensation = 0;
        maxExposureCompensation = 0;
        exposureCompensationStep = 0;
    }
};

