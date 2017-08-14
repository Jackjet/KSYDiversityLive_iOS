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
#include <NvJpegReader.h>
#include <INvVideoFrame.h>

#include <QFile>
#include <QImage>
#include <QVector>


class CNvCafFileReader
{
public:
    CNvCafFileReader(const QString &filePath);
    ~CNvCafFileReader();

public:
    bool IsValid() const;

    ENvCafImageFormat ImageFormat() const;
    unsigned int ImageWidth() const;
    unsigned int ImageHeight() const;
    SNvRational PixelAspectRatio() const;
    SNvRational FrameRate() const;
    unsigned int FrameCount() const;
    ENvCafLoopMode DefaultLoopMode() const;

    QImage ReadFrame(unsigned int frameIndex);
    void ReadFrameEx(unsigned int frameIndex, INvVideoFrame **ppIVideoFrame);

private:
    QString m_filePath;
    unsigned int m_version;
    ENvCafImageFormat m_imageFormat;
    unsigned int m_imageWidth;
    unsigned int m_imageHeight;
    SNvRational m_pixelAspectRatio;
    SNvRational m_frameRate;
    unsigned int m_frameCount;
    ENvCafLoopMode m_defaultLoopMode;
    uint64_t m_frameIndexTableOffset;

private:
    QFile m_file;

    struct __SNvFrameIndexTableEntry {
        uint64_t offset;
        unsigned int frameSize;
    };
    QVector<__SNvFrameIndexTableEntry> m_frames;

private:
    bool ReadHeader();
    bool PrepareFrameIndexTable();

private:
    CNvJpegReader m_jpegReader;

private:
    void ReadFrameByJpegReader(unsigned int frameIndex, INvVideoFrame **ppIVideoFrame);
};

