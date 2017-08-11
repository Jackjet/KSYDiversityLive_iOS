//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 13. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"


///////////////////////////////////////////////////////////////////////////////////
// The following coefficients is for 8 Bit RGB <--> YUV conversion
///////////////////////////////////////////////////////////////////////////////////
//
// RGB --> YUV conversion fomula (Rec.601)
//
// Y =  (0.25679 * R) + (0.50413 * G) + (0.09791 * B) + 16
// U = -(0.14822 * R) - (0.29099 * G) + (0.43922 * B) + 128
// V =  (0.43922 * R) - (0.36779 * G) - (0.07143 * B) + 128
//
//
// YUV --> RGB conversion fomula (Rec.601)
//
// R = 1.16438(Y - 16)                    + 1.59603(V - 128)
// G = 1.16438(Y - 16) - 0.39176(U - 128) - 0.81297(V - 128)
// B = 1.16438(Y - 16) + 2.01723(U - 128)
//
//
// RGB --> YUV conversion fomula (Rec.709)
//
// Y =  (0.18259 * R) + (0.61423 * G) + (0.06201 * B) + 16
// U = -(0.10064 * R) - (0.33857 * G) + (0.43922 * B) + 128
// V =  (0.43922 * R) - (0.39894 * G) - (0.04027 * B) + 128
//
//
// YUV --> RGB conversion fomula (Rec.709)
//
// R = 1.16438(Y - 16)                    + 1.79274(V - 128)
// G = 1.16438(Y - 16) - 0.21325(U - 128) - 0.53291(V - 128)
// B = 1.16438(Y - 16) + 2.11240(U - 128)
//
///////////////////////////////////////////////////////////////////////////////////

// RGB-->YUV
#define NV_YUY2_R_Y_601     0.25679
#define	NV_YUY2_G_Y_601     0.50413
#define	NV_YUY2_B_Y_601     0.09791

#define NV_YUY2_R_U_601     0.14822
#define	NV_YUY2_G_U_601     0.29099
#define	NV_YUY2_B_U_601     0.43922

#define NV_YUY2_R_V_601     0.43922
#define	NV_YUY2_G_V_601     0.36779
#define	NV_YUY2_B_V_601     0.07143

#define NV_YUY2_R_Y_709     0.18259
#define	NV_YUY2_G_Y_709     0.61423
#define	NV_YUY2_B_Y_709     0.06201

#define NV_YUY2_R_U_709     0.10064
#define	NV_YUY2_G_U_709     0.33857
#define	NV_YUY2_B_U_709     0.43922

#define NV_YUY2_R_V_709     0.43922
#define	NV_YUY2_G_V_709     0.39894
#define	NV_YUY2_B_V_709     0.04027

// YUV-->RGB
#define NV_YUY2_Y           1.16438 // Both the coefficients of Y to R,G,B in 601 and 709 standards are the same

#define NV_YUY2_V_R_601     1.59603
#define NV_YUY2_U_G_601     0.39176
#define NV_YUY2_V_G_601     0.81297
#define NV_YUY2_U_B_601     2.01723

#define NV_YUY2_V_R_709     1.79274
#define NV_YUY2_U_G_709     0.21325
#define NV_YUY2_V_G_709     0.53291
#define NV_YUY2_U_B_709     2.11240

//
// Full range YUV goes below
//

// RGB-->YUV
#define NV_YUY2_R_Y_601_FULL    0.299
#define	NV_YUY2_G_Y_601_FULL    0.587
#define	NV_YUY2_B_Y_601_FULL    0.114

#define NV_YUY2_R_U_601_FULL    0.16807
#define	NV_YUY2_G_U_601_FULL    0.32997
#define	NV_YUY2_B_U_601_FULL    0.49804

#define NV_YUY2_R_V_601_FULL    0.49804
#define	NV_YUY2_G_V_601_FULL    0.41705
#define	NV_YUY2_B_V_601_FULL    0.08099

#define NV_YUY2_R_Y_709_FULL    0.2126
#define	NV_YUY2_G_Y_709_FULL    0.7152
#define	NV_YUY2_B_Y_709_FULL    0.0722

#define NV_YUY2_R_U_709_FULL    0.11412
#define	NV_YUY2_G_U_709_FULL    0.38392
#define	NV_YUY2_B_U_709_FULL    0.49804

#define NV_YUY2_R_V_709_FULL    0.49804
#define	NV_YUY2_G_V_709_FULL    0.45237
#define	NV_YUY2_B_V_709_FULL    0.04567

// YUV-->RGB
#define NV_YUY2_Y_FULL          1

#define NV_YUY2_V_R_601_FULL    1.40752
#define NV_YUY2_U_G_601_FULL    0.34549
#define NV_YUY2_V_G_601_FULL    0.71695
#define NV_YUY2_U_B_601_FULL    1.77898

#define NV_YUY2_V_R_709_FULL    1.581
#define NV_YUY2_U_G_709_FULL    0.18806
#define NV_YUY2_V_G_709_FULL    0.46997
#define NV_YUY2_U_B_709_FULL    1.86291


void NvYUV8ToRGB8(uint8_t y, uint8_t u, uint8_t v,
                  bool isBt709,
                  bool isFullRangeYUV,
                  uint8_t &r, uint8_t &g, uint8_t &b);


///////////////////////////////////////////////////////////////////////////////////
// some constants used in rgb<-->yuv color-space conversion
///////////////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* RGB-->YUV                                                            */
/*                                                                      */
/* Y = Wr * R + Wg * G + Wb * B                                         */
/* U = 0.436 * (B - Y) / (1 - Wb)                                       */
/* V = 0.615 * (R - Y) / (1 - Wr)                                       */
/*                                                                      */
/* YUV-->RGB                                                            */
/*                                                                      */
/* R = Y + (1 - Wr) / 0.615 * V                                         */
/* G = Y - Wb * (1 - Wb) / (Wg * 0.436) * U - Wr * (1 - Wr) / (Wg * 0.615) * V    */
/* B = Y + (1 - Wb) / 0.436 * U                                         */
/*                                                                      */
/* For rec 601: Wr = 0.299, Wg = 0.587, Wb = 0.114                      */
/* For rec 709: Wr = 0.2126, Wg = 0.7152, Wb = 0.0722                   */
/*                                                                      */
/************************************************************************/
#define	__NV_U_MAX          0.436
#define	__NV_V_MAX          0.615

#define __NV_BMY_U_601      0.49210
#define __NV_RMY_V_601      0.87732

#define	__NV_R_Y_601        0.299
#define	__NV_G_Y_601        0.587
#define	__NV_B_Y_601        0.114
#define	__NV_R_U_601        (-0.14714)
#define	__NV_G_U_601        (-0.28886)
#define	__NV_B_U_601        0.436
#define	__NV_R_V_601        0.615
#define	__NV_G_V_601        (-0.51499)
#define	__NV_B_V_601        (-0.10001)

#define	__NV_V_R_601        1.13984
#define	__NV_U_G_601        (-0.39465)
#define	__NV_V_G_601        (-0.58060)
#define	__NV_U_B_601        2.03211

#define	__NV_BMY_U_709      0.46993
#define	__NV_RMY_V_709      0.78105

#define	__NV_R_Y_709        0.2126
#define	__NV_G_Y_709        0.7152
#define	__NV_B_Y_709        0.0722
#define	__NV_R_U_709        (-0.09991)
#define	__NV_G_U_709        (-0.33609)
#define	__NV_B_U_709        0.436
#define	__NV_R_V_709        0.615
#define	__NV_G_V_709        (-0.55861)
#define	__NV_B_V_709        (-0.05639)

#define	__NV_V_R_709        1.28033
#define	__NV_U_G_709        (-0.21482)
#define	__NV_V_G_709        (-0.38059)
#define	__NV_U_B_709        2.12798


// convert RGB to YUV (rec 601)
static inline void NvcRGBToYUV601(float r, float g, float b,
                                  float &y, float &u, float &v)
{
    y = __NV_R_Y_601 * r + __NV_G_Y_601 * g + __NV_B_Y_601 * b;
    u = __NV_BMY_U_601 * (b - y);
    v = __NV_RMY_V_601 * (r - y);
}

// convert RGB to YUV (rec 709)
static inline void NvcRGBToYUV709(float r, float g, float b,
                                  float &y, float &u, float&v)
{
    y = __NV_R_Y_709 * r + __NV_G_Y_709 * g + __NV_B_Y_709 * b;
    u = __NV_BMY_U_709 * (b - y);
    v = __NV_RMY_V_709 * (r - y);
}

// Convert HSV to RGB
void NvHSVToRGB(float hue, float sat, float value,
                float& r, float& g, float& b);


void Nv_YUY2Image_To_YUV420PImage(const SNvImageBuffer *yuy2ImgBuffer,
                                  SNvImageBuffer *yuv420pImgBuffer,
                                  unsigned int imageWidth,
                                  unsigned int imageHeight);

void Nv_YUY2Image_To_NV12Image(const SNvImageBuffer *yuy2ImgBuffer,
                               SNvImageBuffer *nv12ImgBuffer,
                               unsigned int imageWidth,
                               unsigned int imageHeight);

void Nv_YUY2Image_To_NV21Image(const SNvImageBuffer *yuy2ImgBuffer,
                               SNvImageBuffer *nv21ImgBuffer,
                               unsigned int imageWidth,
                               unsigned int imageHeight);

void Nv_YUV420PImage_To_NV12Image(const SNvImageBuffer *yuv420pImgBuffer,
                                  SNvImageBuffer *nv12ImgBuffer,
                                  unsigned int imageWidth,
                                  unsigned int imageHeight);



