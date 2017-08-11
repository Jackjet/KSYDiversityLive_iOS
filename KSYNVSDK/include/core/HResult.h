//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 6. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <qglobal.h>
#include <stdint.h>


#ifdef Q_OS_WIN
#include <winerror.h>
#else

// Define HRESULT
typedef int32_t HRESULT;

#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)

#endif // Q_OS_WIN


#define NV_MAKE_HRESULT(sev, fac, code) \
    ((HRESULT)(((uint32_t)(sev) << 31) | ((uint32_t)(fac) << 16) | ((uint32_t)(code))))

#define NV_MAKE_HRESULT_ERROR(code) NV_MAKE_HRESULT(1, 0x666, code)


//
// Define Some General HRESULT codes
//
#define NV_NOERROR                          0
#define NV_S_OK                             0
#define NV_S_FALSE                          1

#define NV_E_FAIL                           NV_MAKE_HRESULT_ERROR(0x6FFF)  // Generic error.
#define NV_E_NO_INTERFACE                   NV_MAKE_HRESULT_ERROR(0x6FFE)  // Interface is not available.
#define NV_E_OUT_OF_MEMORY                  NV_MAKE_HRESULT_ERROR(0x6000)  // Out of memory!
#define NV_E_INVALID_PARAMETER              NV_MAKE_HRESULT_ERROR(0x6001)  // One of the method parameter is invalid.
#define NV_E_INVALID_POINTER                NV_MAKE_HRESULT_ERROR(0x6002)  // One of the method parameter is a uninitialized pointer.
#define NV_E_INVALID_STRUCTURE_SIZE         NV_MAKE_HRESULT_ERROR(0x6003)  // Structure size not set or not valid.
#define NV_E_INVALID_FILE_FORMAT            NV_MAKE_HRESULT_ERROR(0x6004)  // Unreadable file format.
#define NV_E_INVALID_HANDLE                 NV_MAKE_HRESULT_ERROR(0x6005)  // Handle is invalid.
#define NV_E_NOT_INITIALIZED                NV_MAKE_HRESULT_ERROR(0x6006)  // Failed because the module/object is not initialized.
#define NV_E_NOT_SUPPORTED                  NV_MAKE_HRESULT_ERROR(0x6008)  // The requested operation is not supported.
#define NV_E_NOT_IMPLEMENTED                NV_MAKE_HRESULT_ERROR(0x6009)  // The code is not implemented yet for the requested operation.
#define NV_E_NOT_FOUND                      NV_MAKE_HRESULT_ERROR(0x600A)  // No custom object of this GUID has been attached or requested item could not be found.
#define NV_E_FILE_NOT_FOUND                 NV_MAKE_HRESULT_ERROR(0x600B)  // Failed because a file was not found.
#define NV_E_TIME_OUT                       NV_MAKE_HRESULT_ERROR(0x600C)  // Failed because there was a time out.
#define NV_E_WRONG_THREAD                   NV_MAKE_HRESULT_ERROR(0x600D)  // Task is running in a wrong thread.
#define NV_E_NO_GL_CONTEXT                  NV_MAKE_HRESULT_ERROR(0x600E)  // No OpenGL context available.
#define NV_E_NO_GL_RESOURCE                 NV_MAKE_HRESULT_ERROR(0x600F)  // No OpenGL resource available.
#define NV_E_OUT_OF_GPU_MEMORY              NV_MAKE_HRESULT_ERROR(0x6010)  // Run out of GPU memory!
#define NV_E_GL_COMPILE_SHADER_FAILED       NV_MAKE_HRESULT_ERROR(0x6011)  // Failed to compile shader.
#define NV_E_GL_LINK_PROGRAM_FAILED         NV_MAKE_HRESULT_ERROR(0x6012)  // Failed to link shader program.
#define NV_E_FILE_EOF                       NV_MAKE_HRESULT_ERROR(0x6013)  // End of file.

#define NV_E_CAPTURE_DEVICE_ERROR_UNKNOWN           NV_MAKE_HRESULT_ERROR(0x7000)  // Unknown error from capture device
#define NV_E_CAPTURE_DEVICE_ERROR_SERVER_DIED       NV_MAKE_HRESULT_ERROR(0x7001)  // Capture device server died
#define NV_E_CAPTURE_DEVICE_FAIL_TO_START           NV_MAKE_HRESULT_ERROR(0x7002)  // Failed to start capture device
#define NV_E_CAPTURE_DEVICE_FAIL_TO_CONNECT         NV_MAKE_HRESULT_ERROR(0x7003)  // Failed to connect to capture device
#define NV_E_MEDIA_RECORDER_ERROR_UNKNOWN           NV_MAKE_HRESULT_ERROR(0x7004)  // Unknown error from media recorder
#define NV_E_MEDIA_ERROR_SERVER_DIED                NV_MAKE_HRESULT_ERROR(0x7005)  // Media server died

#define NV_E_VIDEO_EFFECT_POS_DESCRIPTION			NV_MAKE_HRESULT_ERROR (0x9301)  // can not get video effect position description
#define NV_E_VIDEO_EFFECT_SET_PARAM					NV_MAKE_HRESULT_ERROR (0x9302)  // can not set video effect param
#define NV_E_VIDEO_EFFECT_PROCESS					NV_MAKE_HRESULT_ERROR (0x9303)  // can not process video effect
#define NV_E_VIDEO_EFFECT_NULL_INPUT_IMAGE			NV_MAKE_HRESULT_ERROR (0x9304)  // the video effect has no input image, effect render should do nothing


