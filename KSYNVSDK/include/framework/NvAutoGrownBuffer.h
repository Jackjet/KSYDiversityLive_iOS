//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 12. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvFrameworkDef.h"


class CNvAutoGrownBuffer
{
public:
    CNvAutoGrownBuffer(unsigned int growStep = 4096);
    ~CNvAutoGrownBuffer();

public:
    void * BufferPtr() const { return m_ptr; }
    unsigned int ActualSizeInBytes() const { return m_actualSizeInBytes; }
    bool EnsureBufferSize(unsigned int sizeInBytes);
    void Clear();

private:
    void *m_ptr;
    unsigned int m_growStep;
    unsigned int m_actualSizeInBytes;
};

