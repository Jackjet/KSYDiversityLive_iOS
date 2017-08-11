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
#include <stdint.h>
#include <string.h>


#ifdef Q_OS_WIN
#include <Guiddef.h>
#else

typedef struct _GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t Data4[8];
} GUID;

#define REFGUID const GUID &

typedef GUID IID;
#define REFIID const IID &
typedef GUID CLSID;
#define REFCLSID const CLSID &


#ifdef INITGUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    extern "C" const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#else
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    extern "C" const GUID name
#endif // INITGUID

__inline bool operator==(REFGUID guidOne, REFGUID guidOther)
{
    return !memcmp(&guidOne, &guidOther, sizeof(GUID));
}

__inline bool operator!=(REFGUID guidOne, REFGUID guidOther)
{
    return !(guidOne == guidOther);
}

// {00000000-0000-0000-0000-000000000000}
DEFINE_GUID(GUID_NULL,
            0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

#endif // Q_OS_WIN


__inline bool operator<(REFGUID guidOne, REFGUID guidOther)
{
    return memcmp(&guidOne, &guidOther, sizeof(GUID)) < 0;
}

#define NV_DEFINE_STATIC_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    static const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

