//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Mar 4. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"
#include "IUnknown.h"
#include "INvVideoFrame.h"
#include "NvVideoEffectDef.h"
#include "INvAudioSamples.h"
#include "NvAudioEffectDef.h"

#include <QVariantMap>
#include <QList>


//
// Video reader factory flag
//
#define NV_VIDEO_FILE_READER_FACTORY_FLAG_ICON_GETTER       1


//
// Video reader capabilities
//
#define NV_VIDEO_FILE_READER_CAP_PROCESS_PAN_AND_SCAN       1


//
// Video file reader factory
//
interface INvVideoFileReader;
struct INvVideoFileReaderCallback;

interface INvVideoFileReaderFactory : public IUnknown
{
    virtual HRESULT __stdcall CreateReader(const QString &in_videoFileName,
                                           const QVariantMap &in_options,
                                           INvVideoFileReaderCallback *in_pICallback, // Optional
                                           const SNvAVFileInfo *in_avFileInfo, // Optional
                                           INvVideoFileReader **out_ppIReader) = 0;
};


struct SNvVideoReaderExtraParams {
    bool usePanAndScan;
    float pan;
    float scan;
    ENvSourceBackgroundMode sourceBackgroundMode;

    SNvVideoReaderExtraParams()
    {
        usePanAndScan = false;
        pan = 0;
        scan = 0;
        sourceBackgroundMode = keNvSourceBackgroundModeColorSolid;
    }

    SNvVideoReaderExtraParams(bool usePanAndScanParam, float panParam, float scanParam)
    {
        usePanAndScan = usePanAndScanParam;
        pan = panParam;
        scan = scanParam;
        sourceBackgroundMode = keNvSourceBackgroundModeColorSolid;
    }

    SNvVideoReaderExtraParams(bool usePanAndScanParam, float panParam, float scanParam, ENvSourceBackgroundMode sourceBackgroundModeParam)
    {
        usePanAndScan = usePanAndScanParam;
        pan = panParam;
        scan = scanParam;
        sourceBackgroundMode = sourceBackgroundModeParam;
    }
};

// Video file reader callback interface
// NOTE: The methods in this interface will be called in the thread you called CreateReader()
struct INvVideoFileReaderCallback
{
    virtual bool IsStoppingEngine() = 0;
};

//
// Video file reader interface
//
interface INvVideoFileReader : public IUnknown
{
    // Return 0 if this is not a valid video file
    virtual unsigned int __stdcall GetStreamCount() = 0;
    virtual HRESULT __stdcall GetStreamInfo(unsigned int in_streamIndex,
                                            SNvVideoStreamInfo *out_streamInfo) = 0;

    virtual HRESULT __stdcall SeekVideoFrame(int64_t in_timestamp,      // in 1/NV_TIME_BASE second unit
                                             int64_t in_timeTolerance,  // in 1/NV_TIME_BASE second unit
                                             const SNvVideoResolution *in_expectedResolution,
                                             ENvVideoRotation in_extraVideoRotation,
                                             const SNvVideoReaderExtraParams *in_extraParams, // Optional
                                             INvVideoFrame **out_ppIVideoFrame) = 0;

    virtual HRESULT __stdcall StartPlayback(int64_t in_startTimestamp,
                                            int64_t in_timeTolerance,
                                            const SNvVideoResolution *in_expectedResolution) = 0;

    virtual HRESULT __stdcall GetNextVideoFrameForPlayback(const SNvVideoResolution *in_expectedResolution,
                                                           ENvVideoRotation in_extraVideoRotation,
                                                           const SNvVideoReaderExtraParams *in_extraParams, // Optional
                                                           INvVideoFrame **out_ppIVideoFrame) = 0;

    virtual int __stdcall GetReaderCapabilities() = 0;
};


//
// Audio file reader factory
//
interface INvAudioFileReader;

interface INvAudioFileReaderFactory : public IUnknown
{
    virtual HRESULT __stdcall CreateReader(const QString &in_audioFileName,
                                           const SNvAVFileInfo *in_avFileInfo, // Optional
                                           INvAudioFileReader **out_ppIReader) = 0;
};


//
// Audio file reader interface
//
interface INvAudioFileReader : public IUnknown
{
    // Return 0 if this is not a valid audio file
    virtual unsigned int __stdcall GetStreamCount() = 0;
    virtual HRESULT __stdcall GetStreamInfo(unsigned int in_streamIndex,
                                            SNvAudioStreamInfo *out_streamInfo) = 0;

    virtual HRESULT __stdcall StartPlayback(int64_t in_startTimestamp) = 0;

    virtual HRESULT __stdcall GetNextAudioSamplesForPlayback(INvAudioSamples **out_ppIAudioSamples) = 0;

    virtual void __stdcall SetUserData(uintptr_t in_userData) = 0;
    virtual uintptr_t __stdcall GetUserData() = 0;
};


//
// AV file writer factory interface
//
interface INvAVFileWriter;

struct INvAVFileWriterCallback
{
    virtual void ReleaseVideoPipelineResource() = 0;
    virtual void ReleaseAudioPipelineResource() = 0;

    virtual void NotifyLiveStreamingError() = 0;
};

#define NV_AV_FILE_WRITER_FLAG_LIVE_MODE                    1
#define NV_AV_FILE_WRITER_FLAG_LIVE_STREAMING               2
#define NV_AV_FILE_WRITER_FLAG_DISABLE_HARDWARE_ENCODER     4
#define NV_AV_FILE_WRITER_FLAG_MAC_CM_SAMPLE_BUFFER         8 // This imply NV_AV_FILE_WRITER_FLAG_LIVE_MODE

//
// AV file writer option keys
//

enum ENvAVFileWriterVideoBitrateGrade
{
    keNvAVFileWriterVideoBitrateGradeLow,
    keNvAVFileWriterVideoBitrateGradeMedium,
    keNvAVFileWriterVideoBitrateGradeHigh
};

// value type: int (keNvVideoRotation_XXX)
#define NV_AV_FILE_WRITER_OPTION_KEY_VIDEO_STREAM_ROTATION      "video-stream-rotation"

// value type: int (keNvAVFileWriterVideoBitrateGradeXXX)
#define NV_AV_FILE_WRITER_OPTION_KEY_VIDEO_BITRATE_GRADE        "video-bitrate-grade"

// value type: int
#define NV_AV_FILE_WRITER_OPTION_KEY_CAPTURE_DEVICE_INDEX       "capture-device-index"

// value type: float
#define NV_AV_FILE_WRITER_OPTION_KEY_VIDEO_BITRATE_MULTIPLIER   "video-bitrate-multiplier"

// value type: bool
#define NV_AV_FILE_WRITER_OPTION_KEY_VIDEO_INTRA_FRAME_ONLY     "video-intra-frame-only"


interface INvAVFileWriterFactory : public IUnknown
{
    virtual HRESULT __stdcall CreateWriter(const QString &in_avFilePath,
                                           const QVariantMap &in_options,
                                           int in_flag, // NV_AV_FILE_WRITER_FLAG_XXX
                                           INvAVFileWriter **out_ppIWriter) = 0;

    virtual void __stdcall SetFileWriterCallback(INvAVFileWriterCallback *in_pIFileWriterCallback) = 0;
};


//
// AV file writer interface
//
interface INvAVFileWriter : public IUnknown
{
    virtual bool __stdcall IsWorkingInPullMode() = 0;
    virtual bool __stdcall SupportGPUInputVideo() = 0;
    virtual QList<ENvPixelFormat> __stdcall QuerySupportedHostInputVideoPixelFormat() = 0;
    virtual QList<ENvAudioSampleFormat> __stdcall QuerySupportedInputAudioSampleFormat() = 0;
    virtual QList<unsigned int> __stdcall QuerySupportedInputAudioChannelCount() = 0;

    virtual HRESULT __stdcall AddVideoStream(const SNvVideoResolution &in_videoRes,
                                             const SNvRational &in_avgFps,
                                             ENvPixelFormat in_inputImagePixelFormat,
                                             ENvVideoRotation in_videoRotation) = 0;
    virtual unsigned int __stdcall GetVideoStreamCount() = 0;

    virtual HRESULT __stdcall AddAudioStream(const SNvAudioResolution &in_audioRes) = 0;
    virtual unsigned int __stdcall GetAudioStreamCount() = 0;
    virtual unsigned int __stdcall QueryAudioEncodingFrameSize(unsigned int in_audioStreamIndex) = 0;

    virtual HRESULT __stdcall Start() = 0;

    virtual HRESULT __stdcall WriteVideoFrame(INvVideoFrame *in_pIVideoFrame,
                                              int64_t in_streamTime) = 0;

    virtual HRESULT __stdcall WriteAudioSamples(INvAudioSamples *in_pIAudioSamplesArray[],
                                                int64_t in_streamTimeArray[]) = 0;

    virtual void __stdcall NotifyVideoEOS() = 0;
    virtual void __stdcall NotifyAudioEOS() = 0;

    virtual HRESULT __stdcall FlushWriter() = 0;
};


//
// Image file reader factory
//
#define NV_IMAGE_FILE_READER_CREATION_FLAG_DISABLE_CACHE    1

interface INvImageFileReader;

interface INvImageFileReaderFactory : public IUnknown
{
    virtual HRESULT __stdcall CreateReader(const QString &in_imageFilePath,
                                           int in_creationFlags,
                                           INvImageFileReader **out_ppIReader) = 0;
};


//
// Video file reader interface
//
interface INvImageFileReader : public IUnknown
{
    virtual HRESULT __stdcall ReadImage(ENvVideoRotation in_extraImageRotation,
                                        INvVideoFrame **out_ppIImageFrame) = 0;

    virtual QString __stdcall GetFilePath() = 0;
};

