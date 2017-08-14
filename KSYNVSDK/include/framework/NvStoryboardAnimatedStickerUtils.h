//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2016. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 11. 2016
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <NvDef.h>
#include <NvVideoEffectParams.h>

#include <QString>


//
// Storyboard animated sticker information
//
struct SNvStoryboardAnimatedStickerInfo {
    float sceneWidth;
    float sceneHeight;
    float stickerCenterX;
    float stickerCenterY;
    float stickerWidth;
    float stickerHeight;

    int defaultDuration; // In millisecond

    QString audioSource; // Empy string if there is no audio track
    bool repeatAudioSource;

    SNvStoryboardAnimatedStickerInfo()
    {
        sceneWidth = 1280;
        sceneHeight = 720;
        stickerCenterX = 0;
        stickerCenterY = 0;
        defaultDuration = 4000;
        stickerWidth = 100;
        stickerHeight = 100;
        repeatAudioSource = false;
    }
};

bool NvStoryboardAnmiatedStickerReadInfoFromDescFile(const QString &animatedStickerDescFilePath,
                                                     SNvStoryboardAnimatedStickerInfo &info);

bool NvStoryboardAnmiatedStickerReadInfoFromDescString(const QString &animatedStickerDescString,
                                                       const QString &resourceDirPath,
                                                       SNvStoryboardAnimatedStickerInfo &info);

