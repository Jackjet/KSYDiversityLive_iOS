//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 22. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvFrameworkDef.h"
#include <NvBaseVideoFrame.h>
#include <INvVideoFrameAllocator.h>
#include <NvVideoEffectDef.h>
#include <NvCafFileDef.h>

#include <qopengl.h>
#include <QImageReader>
#include <QImage>


// Convert OpenGL internal format to pixel format
ENvPixelFormat NvOpenGLInternalFormatToPixelFormat(GLint glInternalFormat);

// Convert pixel format to OpenGL internal format
GLint NvPixelFormatToOpenGLInternalFormat(ENvPixelFormat pixFmt);

GLenum NvGetMatchedForamtFromOpenGLInternalFormat(GLint glInternalFormat);

// Calculate max level of detail (inclusive) according to width and height of an 2D texture
unsigned int NvCalcMaxMipmapLOD(unsigned int baseWidth, unsigned int baseHeight);

// Get a path the current appication can write files to
QString NvGetAppWritablePath();

// NOTE: You can't compare arbitrary parameter type and array parameter type with this function!
bool NvIsEqualFxParamVal(const SNvFxParamVal *paramVal1, const SNvFxParamVal *paramVal2);

// Read scaled image with correct orientation
QImage NvReadScaledImage(const QString &imgFilePath, const QSize &expectedSize);

void NvDetermineCaptureVideoOutputResolution(const QSize &previewSize,
                                             ENvVideoRotation displayRotation,
                                             const SNvRational *fixedPreviewAspectRatio,
                                             SNvVideoResolution *videoOutputRes);

void NvTransformNormalizedRectOfCapturePreview(const SNvRectF *srcRect,
                                               const QSize &actualPreviewSize,
                                               const QSize &previewSize,
                                               ENvVideoRotation videoPreviewRotation,
                                               SNvRectF *transformedRect);

void NvGenerateGrabbedCapturedVideoFrame(const SNvImageBuffer &capturedImageBuffer,
                                         ENvPixelFormat capturedImagePixFmt,
                                         unsigned int capturedImageWidth,
                                         unsigned int capturedImageHeight,
                                         ENvVideoRotation capturedImageRoation,
                                         bool flipHorizontally,
                                         const SNvRational *fixedOutputImageAspectRatio, // May be null
                                         INvVideoFrameAllocator *pIHostVideoFrameAllocator,
                                         INvVideoFrame **ppIOutputVideoFrame);

// Perform preprocessing for video source frame: orientation handling, aspect ratio handling...
void NvPreprocessVideoSourceFrame(INvVideoFrame *pISrcVideoFrame,
                                  const SNvVideoResolution &videoOutputRes,
                                  INvVideoEffectResourceManager *pIVideoEffectResourceManager,
                                  INvVideoFrameAllocator *pIGPUVideoFrameAllocator,
                                  bool forceCopy,
                                  bool usePanAndScanParameter,
                                  float pan,    // Used only when usePanAndScanParameter is true
                                  float scan,   // Used only when usePanAndScanParameter is true
                                  ENvSourceBackgroundMode sourceBackgroundMode,
                                  INvVideoFrame **ppIDstVideoFrame);

bool NvCanPanAndScanVideoSourceFrame(const SNvAVFileInfo &sourceVideoFileInfo,
                                     ENvVideoRotation extraVideoRotation,
                                     const SNvVideoResolution &videoEditRes);

// Return false indicate that we can't find a proper pan and scan parameter
// to make source video looks like a square under the given video edit resolution
bool NvCalcPanAndScanParamForSquareVideo(const SNvAVFileInfo &squareVideoFileInfo,
                                         const SNvVideoResolution &videoEditRes,
                                         float &pan,
                                         float &scan);

bool NvConvertGifToCaf(const QString &gifFilePath,
                       const QString &cafFilePath,
                       ENvCafImageFormat cafImageFormat,
                       const SNvRational &cafFrameRate,
                       ENvCafLoopMode cafDefaultLoopMode,
                       int *durationInMs = nullptr);

// Return -1 indicate error, otherwise return file duration in 1/NV_TIME_BASE second unit
int64_t NvGetCafFileDuration(const QString &cafFilePath);


///////////////////////////////////////////////////////////////////////////////////
// Simple video frame wrapper for texture
///////////////////////////////////////////////////////////////////////////////////
class CNvTextureVideoFrameWrapper : public CNvBaseVideoFrame
{
public:
    CNvTextureVideoFrameWrapper(GLuint texId,
                                ENvPixelFormat pixFmt,
                                unsigned int texWidth,
                                unsigned int texHeight,
                                const SNvRational &in_pixelAspectRatio);

    ~CNvTextureVideoFrameWrapper() {}

public:
    virtual HRESULT __stdcall GetImageBuffer(SNvImageBuffer *out_imgBuffer);
    virtual uintptr_t __stdcall GetHandle();

protected:
    GLuint m_texId;
};


///////////////////////////////////////////////////////////////////////////////////
// Simple host buffer video frame wrapper
// NOTE: host buffer must be allocated by NvAlignedMalloc()
///////////////////////////////////////////////////////////////////////////////////
class CNvHostVideoFrameWrapper : public CNvBaseVideoFrame
{
public:
    CNvHostVideoFrameWrapper(void *imgBufferPtr, // Set it to nullptr if you don't want wrapper take ownership of the buffer
                             const SNvImageBuffer &imgBuffer,
                             ENvPixelFormat pixFmt,
                             unsigned int imageWidth,
                             unsigned int imageHeight,
                             const SNvRational &pixelAspectRatio,
                             ENvVideoRotation displayRotation);
    ~CNvHostVideoFrameWrapper();

public:
    virtual HRESULT __stdcall GetImageBuffer(SNvImageBuffer *out_imgBuffer);
    virtual uintptr_t __stdcall GetHandle() { return 0; }

protected:
    void *m_imgBufferPtr;
    SNvImageBuffer m_imgBuffer;
};


bool NvCreateWrapperVideoFrameFromTexInfo(const SNvImageTextureInfo *texInfo, INvVideoFrame **ppIVideoFrame);

