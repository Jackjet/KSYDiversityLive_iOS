//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Mar 18. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"
#include "IUnknown.h"
#include "INvVideoFrame.h"
#include "INvAudioSamples.h"
#include <QList>
#include <QPair>
#include <QSize>


enum ENvCameraFacing
{
    keNvCameraFacing_Front,
    keNvCameraFacing_Back
};

struct SNvVideoCaptureDeviceInfo {
    ENvCameraFacing facing;
    ENvVideoRotation orientation;
};

// Recording quality
enum ENvRecordingQuality
{
    keNvRecordingQuality_High,
    keNvRecordingQuality_Medium,
    keNvRecordingQuality_Low,
    keNvRecordingQuality_SuperHigh
};

// Capture flag
enum ENvCaptureFlag
{
    keNvCaptureFlag_None = 0,
    keNvCaptureFlag_GrabCapturedVideoFrame = 1,
    keNvCaptureFlag_LiveStreaming = 2,
    keNvCaptureFlag_DontUseSystemRecorder = 4,
    keNvCaptureFlag_StrictPreviewVideoSize = 8,
    keNvCaptureFlag_DontCaptureAudio = 16,
};

// Recording flag
enum ENvRecordingFlag
{
    keNvRecordingFlag_None = 0,
    keNvRecordingFlag_RecordWithFx = 1,
    keNvRecordingFlag_VideoIntraFrameOnly = 2,
};


interface INvVideoCaptureDevice;

// Video capture device callback interface
// NOTE: The methods(except CapturedVideoFrameGrabbed) in this interface will be called in the thread you called OpenDevice()
struct INvVideoCaptureDeviceCallback
{
    virtual void VideoFrameCaptured(INvVideoFrame *in_pIVideoFrame, unsigned int in_deviceIndex) = 0;
    virtual void AudioFrameCaptured(INvAudioSamples *in_pIAudioSamples, unsigned int in_deviceIndex) = 0;

    virtual void CaptureErrorNotification(HRESULT in_errCode, unsigned int in_deviceIndex) = 0;

    virtual void RecordingErrorNotification(HRESULT in_errCode, unsigned int in_deviceIndex) = 0;

    virtual void AutoFocusComplete(bool success, unsigned int in_deviceIndex) = 0;

    virtual bool TryAcquireVideoPipelineResource() = 0;
    virtual void ReleaseVideoPipelineResource() = 0;

    virtual bool TryAcquireAudioPipelineResource() = 0;
    virtual void ReleaseAudioPipelineResource() = 0;

    // Called from a thread depending on platform
    virtual void CapturedVideoFrameGrabbed(INvVideoFrame *in_pICapturedVideoFrame, int64_t in_streamTime) = 0;
};

// Video capture device enumerator interface
interface INvVideoCaptureDeviceEnumerator : public IUnknown
{
    virtual unsigned int __stdcall GetDeviceCount() = 0;

    virtual HRESULT __stdcall GetDeviceInfo(unsigned int in_deviceIndex,
                                            SNvVideoCaptureDeviceInfo *out_deviceInfo) = 0;
};

// Video capture device factory interface
interface INvVideoCaptureDeviceFactory : public IUnknown
{
    virtual unsigned int __stdcall GetDeviceCount() = 0;

    virtual HRESULT __stdcall GetDeviceInfo(unsigned int in_deviceIndex,
                                            SNvVideoCaptureDeviceInfo *out_deviceInfo) = 0;

    virtual HRESULT __stdcall OpenDevice(unsigned int in_deviceIndex,
                                         INvVideoCaptureDeviceCallback *pICallback,
                                         INvVideoCaptureDevice **out_ppIDevice) = 0;
};

// Video capture device interface
interface INvVideoCaptureDevice : public IUnknown
{
    virtual HRESULT __stdcall StartCapture(ENvRecordingQuality in_quality,
                                           ENvCaptureFlag in_flags,
                                           const SNvRational *in_fixedPreviewAspectRatio) = 0;
    virtual void __stdcall StopCapture() = 0;

    // You will only get valid video preview resolution after capture has been started
    virtual void __stdcall GetVideoPreviewResolution(SNvVideoResolution *out_videoPreivewResolution) = 0;
    // You will only get valid video preview FPS after capture has been started
    virtual void __stdcall GetVideoPreviewFps(SNvRational *out_videoFps) = 0;
    // You will only get valid audio resolution after capture has been started
    virtual void __stdcall GetAudioResolution(SNvAudioResolution *out_audioResolution) = 0;

    virtual QList<QSize> __stdcall GetSupportedVideSizes() = 0;
    virtual ENvVideoRotation __stdcall GetVideoRecordingRotation() = 0;
    virtual HRESULT __stdcall StartRecording(const QString &in_outputFilePath,
                                             float in_videoBitrateMultiplier,
                                             ENvRecordingFlag in_flags) = 0;
    virtual HRESULT __stdcall StopRecording() = 0;

    virtual bool __stdcall SupportAutoFocus() = 0;
    // NOTE: the focus area is in normalized coordinates
    virtual void __stdcall StartAutoFocus(const SNvRectF *in_focusArea) = 0;
    virtual void __stdcall CancelAutoFocus() = 0;

    virtual bool __stdcall SupportAutoExposure() = 0;
    // NOTE: the auto exposure area is in normalized coordinates
    virtual void __stdcall StartAutoExposure(const SNvRectF *in_exposureArea) = 0;

    virtual bool __stdcall IsZoomSupported() = 0;
    virtual bool __stdcall IsSteplessZoomSupported() = 0;
    virtual int __stdcall MaxZoom() = 0;
    virtual QList<float> __stdcall GetZoomRatios() = 0;
    virtual bool __stdcall SetZoom(int in_zoom) = 0;
    virtual bool __stdcall SetZoomRatio(float in_zoomRatio) = 0;

    virtual bool __stdcall IsFlashSupported() = 0;
    virtual void __stdcall ToggleFlash(bool in_on) = 0;

    virtual bool __stdcall IsExposureCompensationSupported() = 0;
    virtual bool __stdcall IsSteplessExposureCompensationSupported() = 0;
    virtual void __stdcall GetExposureCompensationRange(int *out_minExposureCompensation,
                                                        int *out_maxExposureCompensation) = 0;
    virtual float __stdcall GetExposureCompensationStep() = 0;
    virtual bool __stdcall SetExposureCompensation(int in_exposureCompensation) = 0;
    virtual bool __stdcall SetExposureCompensationValue(float in_exposureCompensationValue) = 0;

    virtual void __stdcall ReleaseDevice() = 0;
};

