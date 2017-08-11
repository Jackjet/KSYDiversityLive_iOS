//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 6 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <qglobal.h>

#ifdef Q_CC_GNU
#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS
    #ifdef Q_OS_ANDROID
    // stdint.h shipped in android NDK is stupid, we must include stdint-gcc.h
    #include <stdint-gcc.h>
    #else
    #include <stdint.h>
    #endif
#else
#include <stdint.h>
#endif


//
// Memory alignment
//
#ifdef _MSC_VER
#define NVC_MEMORY_ALIGN(A)         __declspec(align(A))
#else
#define NVC_MEMORY_ALIGN(A)         __attribute__((aligned(A)))
#endif // _MSC_VER

#ifndef _MSC_VER
#define interface struct
#define __stdcall
#endif

#define NV_UP_ALIGN(N, STEP)        (((N) + (STEP) - 1) / (STEP) * (STEP))

// Warning: you must ensure that STEP is power of 2
#define NV_UP_ALIGN_PO2(N, STEP)    (((N) + (STEP) - 1) & (~((STEP) - 1)))


#define	NV_GET_MEMBER_CONTAINER(PTR, TYPE, MEMBER)  ((TYPE *)((char *)(PTR) - (size_t)(&((TYPE *)0)->MEMBER)))
#define	NV_GET_MEMBER_OFFSET(TYPE, MEMBER)          (uintptr_t)(&((TYPE *)0)->MEMBER)

//
// Some constant values
//
#define NV_PI                       3.14159265358979323846
#define NV_E                        2.71828182845904523536

#define NV_TIME_BASE                1000000
#define NV_NOPTS_VALUE              ((int64_t)0x8000000000000000)


// Structure to represent a rational num/den
struct SNvRational {
    int num;    // Numerator
    int den;    // Denominator
};

static inline bool NvIsValidRational(const SNvRational &rational)
{
    return rational.den != 0;
}

struct SNvRect {
    int left;
    int top;
    int right;
    int bottom;
};

struct SNvRectF {
    float left;
    float top;
    float right;
    float bottom;
};

struct SNvPointF {
    float fX;
    float fY;
};

struct SNvPointD {
    double x;
    double y;
};

struct SNvColor {
    float r, g, b, a;
};

struct SNvPosition2D {
    float x, y;
};

struct SNvPosition3D {
    float x, y, z;
};

//
// Audio/Video file type
//
enum ENvAVFileType
{
    keNvAVFileType_Unknown = -1,
    keNvAVFileType_AudioVideo = 0,
    keNvAVFileType_Audio,
    keNvAVFileType_Image
};

//
// Pixel format
//
enum ENvPixelFormat
{
    keNvPixFmt_None = -1,
    keNvPixFmt_YUV420P,     // planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
    keNvPixFmt_YUYV422,     // packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
    keNvPixFmt_YUV422P,     // planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
    keNvPixFmt_UYVY422,     // packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1
    keNvPixFmt_NV12,        // planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
    keNvPixFmt_NV21,        // as above, but U and V bytes are swapped

    keNvPixFmt_GRAY8,       // Y 8bpp

    keNvPixFmt_RGB8,        // packed RGB 8:8:8, 24bpp, RGBRGB...
    keNvPixFmt_BGR8,        // packed RGB 8:8:8, 24bpp, BGRBGR...
    keNvPixFmt_ARGB8,       // packed ARGB 8:8:8:8, 32bpp, ARGBARGB...
    keNvPixFmt_RGBA8,       // packed RGBA 8:8:8:8, 32bpp, RGBARGBA...
    keNvPixFmt_BGRA8,       // packed BGRA 8:8:8:8, 32bpp, BGRABGRA...

    keNvPixFmt_OpenGL_RGB8,     // Corresponding to opengl internal format GL_RGB8 or GL_RGB
    keNvPixFmt_OpenGL_RGBA8,    // Corresponding to opengl internal format GL_RGBA8 or GL_RGBA

    keNvPixFmt_Count
};

//
// Alignment
//
enum ENvAlignment
{
    keNvAlignment_Left = 0,
    keNvAlignment_Center,
    keNvAlignment_Right
};

//
// Video aspect ratio
//
enum ENvVideoAspectRatio
{
    keNvVideoAspectRatio_16v9 = 0,
    keNvVideoAspectRatio_1v1
};

//
// Structure to describe image buffer
//
#define NV_IMAGE_BUFFER_MAX_POINTER     4

struct SNvImageBuffer {
    void *data[NV_IMAGE_BUFFER_MAX_POINTER];
    int pitch[NV_IMAGE_BUFFER_MAX_POINTER];
};

//
// Structure to describe image position
//
struct SNvImagePosition {
    unsigned int imageWidth;
    unsigned int imageHeight;
    // Pixel coordinates of center pixel's top-left corner of this image
    int centerPixelCoordX;
    int centerPixelCoordY;
};

enum ENvVideoRotation
{
    keNvVideoRotation_0 = 0,
    keNvVideoRotation_90,
    keNvVideoRotation_180,
    keNvVideoRotation_270
};

static inline int NvVideoRotationAngle(ENvVideoRotation videoRotation)
{
    return videoRotation * 90;
}

static inline ENvVideoRotation NvVideoRotationFromAngle(int angle)
{
    switch (angle) {
    default:
    case 0:
        return keNvVideoRotation_0;
    case 90:
        return keNvVideoRotation_90;
    case 180:
        return keNvVideoRotation_180;
    case 270:
        return keNvVideoRotation_270;
    }
}

static inline ENvVideoRotation NvCombineVideoRotation(ENvVideoRotation videoRotation,
                                                      ENvVideoRotation videoRotationToCombine)
{
    return (ENvVideoRotation)((videoRotation + videoRotationToCombine) % 4);
}

//
// Memory location
//
enum ENvMemoryLocation
{
    keNvMemoryLocation_None = -1,
    keNvMemoryLocation_Host,
    keNvMemoryLocation_OpenGL_Texture,  // OpenGL 2D texture
    keNvMemoryLocation_Count,
};

static inline bool NvIsGpuMemoryLocation(ENvMemoryLocation memLoc)
{
    return memLoc == keNvMemoryLocation_OpenGL_Texture;
}

//
// Audio sample format
// NOTE: the order of the enumeration can't be changed!
//
enum ENvAudioSampleFormat
{
    keNvAudSmpFmt_None = -1,
    keNvAudSmpFmt_U8,       // unsigned 8 bit
    keNvAudSmpFmt_S16,      // signed 16 bit
    keNvAudSmpFmt_S32,      // signed 32 bit
    keNvAudSmpFmt_FLT,      // single precision float
    keNvAudSmpFmt_DBL,      // double precision float

    keNvAudSmpFmt_U8P,      // unsigned 8 bit, planar
    keNvAudSmpFmt_S16P,     // signed 16 bit, planar
    keNvAudSmpFmt_S32P,     // signed 32 bit, planar
    keNvAudSmpFmt_FLTP,     // single precision float, planar
    keNvAudSmpFmt_DBLP,     // double precision float, planar

    keNvAudSmpFmt_Count
};

//
// Structure to describe audio samples buffer
//
#define NV_AUDIO_MAX_CHANNEL_COUNT      6

struct SNvAudioSamplesBuffer {
    void *data[NV_AUDIO_MAX_CHANNEL_COUNT];
    int planeSizeInBytes;   // For planar audio: size in bytes of each plane(channel)
                            // For packed audio: size in bytes of total samples
};

//
// Audio resolution
//
struct SNvAudioResolution {
    unsigned int sampleRate;
    ENvAudioSampleFormat sampleFormat;
    unsigned int channelCount;
};

//
// Video resolution
//
struct SNvVideoResolution {
    unsigned int imageWidth;
    unsigned int imageHeight;
    SNvRational imagePAR;
    SNvRational proxyScale;
};

//
// Audio/Video resolution
//
struct SNvAVResolution {
    SNvAudioResolution audioRes;
    SNvVideoResolution videoRes;
};

//
// Video stream information
//
struct SNvVideoStreamInfo {
    int64_t duration; // In 1/NV_TIME_BASE seconds unit
    unsigned int imageWidth;
    unsigned int imageHeight;
    SNvRational pixelAspectRatio;
    SNvRational frameRate;
    ENvVideoRotation displayRotation;
};

//
// Audio stream information
//
#define NV_AUDIO_MAX_STREAM_COUNT   4

struct SNvAudioStreamInfo {
    int64_t duration; // In 1/NV_TIME_BASE seconds unit
    unsigned int sampleRate;
    unsigned int channelCount; // Up to NV_AUDIO_MAX_CHANNEL_COUNT
};

//
// AV file information
//
struct SNvAVFileInfo {
    ENvAVFileType type;

    unsigned int numVideoStreams; // Up to 1 stream
    SNvVideoStreamInfo videoStreamInfo;

    unsigned int numAudioStreams; // Up to NV_AUDIO_MAX_STREAM_COUNT streams
    SNvAudioStreamInfo audioStreamInfo[NV_AUDIO_MAX_STREAM_COUNT];

    int64_t duration; // In 1/NV_TIME_BASE seconds unit
    uint64_t dataRate; // Bits per second
};

