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
#include <qopengl.h>

#include <QVector>


class CNvAutoGrownTexture
{
public:
    CNvAutoGrownTexture(GLint internalFormat, unsigned int growStep = 128);
    ~CNvAutoGrownTexture();

public:
    GLuint Texture() const { return m_tex; }
    unsigned int TexActualWidth() const { return m_texActualWidth; }
    unsigned int TexActualHeight() const { return m_texActualHeight; }
    bool EnsureTextureStorage(unsigned int texWidth,
                              unsigned int texHeight,
                              bool exactMatch);
    void Clear();

private:
    GLuint m_tex;
    GLint m_internalFormat;
    unsigned int m_growStep;
    unsigned int m_texActualWidth, m_texActualHeight;
};


class CNvRecycledAutoGrownTexturePool
{
public:
    CNvRecycledAutoGrownTexturePool(GLint internalFormat,
                                    unsigned int poolSize,
                                    unsigned int growStep = 128);

    ~CNvRecycledAutoGrownTexturePool();

public:
    bool EnsureTextureStorage(unsigned int texWidth,
                              unsigned int texHeight,
                              bool exactMatch);

    CNvAutoGrownTexture * GetRecycledTexture();

    void Clear();

private:
    QVector<CNvAutoGrownTexture *> m_texPool;
    unsigned int m_currentIndex;
};

