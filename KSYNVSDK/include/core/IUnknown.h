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

#include "NvDef.h"
#include "HResult.h"
#include "NvGuidDef.h"


#ifdef Q_OS_WIN
#include <unknwn.h>
#else

interface IUnknown
{
public:
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObj) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};

// {00000000-0000-0000-C000-000000000046}
DEFINE_GUID(IID_IUnknown,
            0x00000000, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);

#endif // Q_OS_WIN

