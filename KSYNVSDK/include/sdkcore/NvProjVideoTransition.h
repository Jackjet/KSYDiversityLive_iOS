//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2016. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Dec 16. 2016
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvProjTransition.h"


class CNvProjTrack;
class CNvProjVideoTrack;

class CNvProjVideoTransition : public CNvProjTransition
{
    friend class CNvProjTrack;
    friend class CNvProjVideoTrack;

public:
    enum VideoTransitionType
    {
        BuiltinVideoTransition = 0,
        PackagedVideoTransition,
    };

    enum Category
    {
        DefaultCategory,
        UserCategory,
        ThemeCategory
    };

protected:
    CNvProjVideoTransition(CNvProjTrack *ownerTrack,
                           VideoTransitionType videoTransitionType,
                           Category VideoTransitionCategory,
                           unsigned int srcClipIndex,
                           CNvAppFxInstance *fxInstance,
                           CNvProjContext *projContext);
    ~CNvProjVideoTransition();

public:
    VideoTransitionType GetVideoTransitionType() const { return m_videoTransitionType; }
    Category GetCategory() const { return m_catetory; }
    QString TransitionPackageId() const { return m_transitionPackageId; }

protected:
    virtual void Destroy();

private:
    VideoTransitionType m_videoTransitionType;
    Category m_catetory;

    QString m_transitionPackageId;
};

