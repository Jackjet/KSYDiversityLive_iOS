//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Aug 8. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvCafFileDef.h"
#include <QImage>
#include <QFile>
#include <QVector>


class CNvCafFileWriter
{
protected:
    CNvCafFileWriter(const QString &filePath,
                     ENvCafImageFormat imageFormat,
                     unsigned int imageWidth,
                     unsigned int imageHeight,
                     const SNvRational &pixelAspectRatio,
                     const SNvRational &frameRate,
                     ENvCafLoopMode defaultLoopMode);

    ~CNvCafFileWriter();

public:
    static CNvCafFileWriter * CreateWriter(const QString &filePath,
                                           ENvCafImageFormat imageFormat,
                                           unsigned int imageWidth,
                                           unsigned int imageHeight,
                                           const SNvRational &pixelAsprectRatio,
                                           const SNvRational &frameRate,
                                           ENvCafLoopMode defaultLoopMode);

    static void DestroyWriter(CNvCafFileWriter *writer);

public:
    bool WriteFrame(const QImage &frameImage, int quality = -1);
    bool Close();

private:
    QString m_filePath;
    ENvCafImageFormat m_imageFormat;
    unsigned int m_imageWidth;
    unsigned int m_imageHeight;
    SNvRational m_pixelAspectRatio;
    SNvRational m_frameRate;
    ENvCafLoopMode m_defaultLoopMode;

private:
    QFile m_file;
    unsigned int m_headerAlignedSize;
    unsigned int m_reservedFrameIndexTableSize;
    bool m_ioErrOcurred;

private:
    struct __SNvFrameIndexTableEntry {
        uint64_t offset;
        unsigned int frameSize;
    };

    QVector<__SNvFrameIndexTableEntry> m_frames;

private:
    bool Init();
    bool WriteHeader(unsigned int frameCount, uint64_t frameIndexTableOffset);
    bool WriteFrameIndexTable(uint64_t frameIndexTableOffset);
};

