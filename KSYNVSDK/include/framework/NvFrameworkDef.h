//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 20. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <NvDef.h>

#include <qopengl.h>


struct SNvImageTextureInfo {
    unsigned int imageWidth;
    unsigned int imageHeight;
    GLuint texId;
    GLint internalFormat;
    bool isUpsideDown;
};

