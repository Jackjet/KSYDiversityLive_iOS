//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 13 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"

#include <QString>


// Default image pitch alignment
// Make sure it is equal to the maximum pack/unpack alignment of OpenGL ES 2
// i.e. default value of GL_PACK_ALIGNMENT and GL_UNPACK_ALIGNMENT
#define NV_DEFAULT_PITCH_ALIGNMENT      4


// Check if a pixel format is planar
bool NvIsPlanarPixFmt(ENvPixelFormat pixFmt);

// Check whether a pixel format has alpha channel
bool NvPixFmtHasAlphaChannel(ENvPixelFormat pixFmt);

// Check whether a pixel format is YUV format
bool NvPixFmtIsYUV(ENvPixelFormat pixFmt);

// Check whether a pixel format is GPU pixel format
bool NvPixFmtIsGPU(ENvPixelFormat pixFmt);

// Plane count for a given pixel format
// Return 1 when pixel format is packed
unsigned int NvPlaneCountOfPixFmt(ENvPixelFormat pixFmt);

// Get line size in bytes and optimized row pitch in bytes
// for a image plane with given pixel format and width
void NvGetLineInfoForImagePlane(ENvPixelFormat pixFmt,
                                unsigned int imageWidth,
                                unsigned int planeIndex,
                                int *lineSizeInBytes,
                                int *rowPitchInBytes,
                                unsigned int alignment = NV_DEFAULT_PITCH_ALIGNMENT);

// Get actual pixel count stored in buffer for a image plane with given pixel format and width
unsigned int NvGetActualWidthForImagePlane(ENvPixelFormat pixFmt,
                                           unsigned int imageWidth,
                                           unsigned int planeIndex);

// Get actual pixel line count stored in buffer for a image plane with given pixel format and height
unsigned int NvGetActualHeightForImagePlane(ENvPixelFormat pixFmt,
                                            unsigned int imageHeight,
                                            unsigned int planeIndex);

// Calculate size in bytes that a image buffer of the given width and height
// would occupy if stored in the given pixel format
unsigned int NvImageBufferGetSizeInBytes(ENvPixelFormat pixFmt,
                                         unsigned int imageWidth,
                                         unsigned int imageHeight,
                                         unsigned int alignment = NV_DEFAULT_PITCH_ALIGNMENT);

// Fill in the SNvImageBuffer fields
void NvImageBufferFillInfo(SNvImageBuffer *imgBuffer,
                           const void *imgBufferPtr,
                           ENvPixelFormat pixFmt,
                           unsigned int imageWidth,
                           unsigned int imageHeight,
                           unsigned int alignment = NV_DEFAULT_PITCH_ALIGNMENT);

void NvFillImageBufferWithOffset(const SNvImageBuffer *imgBuffer,
                                 ENvPixelFormat pixFmt,
                                 unsigned int lineOffset,
                                 SNvImageBuffer *offsetImgBuffer);

// Copy images with the same width, height and pixel format
void NvCopyImage(const SNvImageBuffer *srcImgBuffer,
                 const SNvImageBuffer *dstImgBuffer,
                 ENvPixelFormat pixFmt,
                 unsigned int imageWidth,
                 unsigned int imageHeight);

// Flip image vertically
void NvFlipImageVertically(const SNvImageBuffer *srcImgBuffer,
                           const SNvImageBuffer *dstImgBuffer,
                           ENvPixelFormat pixFmt,
                           unsigned int imageWidth,
                           unsigned int imageHeight);

// Flip image vertically inplace
void NvFlipImageVerticallyInplace(const SNvImageBuffer *imgBuffer,
                                  ENvPixelFormat pixFmt,
                                  unsigned int imageWidth,
                                  unsigned int imageHeight);

static inline bool NvIsValidPixelAspectRatio(const SNvRational &pixelAspectRatio)
{
    return pixelAspectRatio.num > 0 && pixelAspectRatio.den > 0;
}

static inline bool NvIsValidProxyScale(const SNvRational &proxyScale)
{
    return proxyScale.num > 0 && proxyScale.den > 0;
}

// Calculate the column and row(in buffer coordinates) of the top-left corner of center pixel of an image
static inline void NvCalcBufferCoordOfImageCenterPixel(unsigned int imageWidth,
                                                       unsigned int imageHeight,
                                                       int &centerPixCol,
                                                       int &centerPixRow)
{
    centerPixCol = imageWidth / 2;
    centerPixRow = imageHeight / 2;
}

void NvNomalizeBoundingRect(SNvRect *boundingRect);

bool NvIsEqualImagePos(const SNvImagePosition *imgPos1, const SNvImagePosition *imgPos2);

// Calculate image bounding rectangle(in pixel coordinates) from SNvImagePosition
void NvBoundingRectFromImagePos(const SNvImagePosition *imgPos,
                                SNvRect *boundingRect);

// Calculate image position from a bounding rectangle(in pixel coordinates)
void NvImagePosFromBoundingRect(const SNvRect *boundingRect,
                                SNvImagePosition *imgPos);

// Calculate bounding rectangle in canonical coordinates for a given SNvImagePostion
void NvCalcCanonicalBoundingRectFromImagePos(const SNvImagePosition *imgPos,
                                             const SNvRational *pixelAspectRatio,
                                             const SNvRational *proxyScale,
                                             SNvRectF *canonicalBounding);

// Calculate bounding rectangle in canonical coordinates from a given bounding rect in pixel coordinates
void NvCalcCanonicalBoundingRectFromPixelBoundingRect(const SNvRect *pixelBounding,
                                                      const SNvRational *pixelAspectRatio,
                                                      const SNvRational *proxyScale,
                                                      SNvRectF *canonicalBounding);

// Calculate bounding rectangle in image buffer coordinates from a given bounding rect in pixel coordinates
void NvCalcBufferBoundingRectFromPixelBoundingRect(const SNvRect *pixelBounding,
                                                   const SNvImagePosition *imgPos,
                                                   SNvRect *bufferBounding);

float NvCalcPixelCoordFromCanonicalCoordX(float xCanonical, const SNvRational *pixelAspectRatio, const SNvRational *proxyScale);
float NvCalcPixelCoordFromCanonicalCoordY(float yCanonical, const SNvRational *proxyScale);

float NvCalcCanonicalCoordFromPixelCoordX(float xPix, const SNvRational *pixelAspectRatio, const SNvRational *proxyScale);
float NvCalcCanonicalCoordFromPixelCoordY(float yPix, const SNvRational *proxyScale);

float NvCalcPixelCoordFromBufferCoordX(float x, const SNvImagePosition *imgPos);
float NvCalcPixelCoordFromBufferCoordY(float y, const SNvImagePosition *imgPos);

float NvCalcCanonicalCoordFromBufferCoordX(float xBuf, const SNvImagePosition *imgPos,
                                           const SNvRational *pixelAspectRatio, const SNvRational *proxyScale);
float NvCalcCanonicalCoordFromBufferCoordY(float yBuf, const SNvImagePosition *imgPos, const SNvRational *proxyScale);

float NvCalcCanonicalCoordFromNormalizedCoordX(float x, const SNvRectF *refBounding);
float NvCalcCanonicalCoordFromNormalizedCoordY(float y, const SNvRectF *refBounding);

float NvCalcNormalizedCoordFromCanonicalCoordX(float x, const SNvRectF *refBounding);
float NvCalcNormalizedCoordFromCanonicalCoordY(float y, const SNvRectF *refBounding);

// Calculate image position in a new pixel aspect ratio and proxy scale
void NvCalcImgPosInNewPixelAspectRatioAndProxyScale(const SNvImagePosition *orgImgPos,
                                                    const SNvRational *orgPAR,
                                                    const SNvRational *orgProxyScale,
                                                    const SNvRational *newPAR,
                                                    const SNvRational *newProxyScale,
                                                    SNvImagePosition *newImgPos);

// Calculate the angle needed to rotate angle1 to angle2 clock-wise
int NvCalcRotationAngleBetween(int angle1, int angle2, bool invert);

// NOTE: To display the jpeg image in correct orientation,
// you should first mirror horizontally according to the returned value in horizontalMirrored
// and then rotate clock-wise according to the returned value in videoRotation
bool NvDetectJpegImageOrientation(const QString &filePath,
                                  bool *horizontalMirrored,
                                  ENvVideoRotation *videoRotation);

