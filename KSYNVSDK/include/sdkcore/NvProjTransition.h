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

#include "NvProjAppFx.h"
#include <NvStreamingEngineDef.h>


class CNvProjTrack;
class CNvProjVideoTrack;
class CNvProjClip;

class CNvProjTransition : public CNvProjAppFx,
                          public SNvTransitionCallback
{
    friend class CNvProjTrack;
    friend class CNvProjVideoTrack;

public:
    enum TransitionType
    {
        VideoTransition,
        AudioTransition
    };

protected:
    CNvProjTransition(CNvProjTrack *ownerTrack,
                      unsigned int srcClipIndex,
                      TransitionType type,
                      CNvAppFxInstance *fxInstance,
                      CNvProjContext *projContext,
                      ProjObjectType projObjType);
    ~CNvProjTransition();

public:
    CNvProjTrack * OwnerTrack() const { return m_ownerTrack; }
    CNvProjClip * SrcClip() const;
    CNvProjClip * DstClip() const;
    TransitionType GetTransitionType() const { return m_type; }

private:
    //
    // SNvTransitionCallback callback interface
    //
    virtual bool GetEffectDesc(INvEffectDescriptor **out_ppIEffectDesc);
    virtual bool FillEffectSettings(INvEffectSettings *io_pIEffectSettings);

    virtual bool GetTransitionFilterEffectDesc(unsigned int in_filterEffectIndex, INvEffectDescriptor **out_ppIEffectDesc);
    virtual bool FillTransitionFilterEffectSettings(unsigned int in_filterEffectIndex, INvEffectSettings *io_pIEffectSettings);

protected:
    virtual void Destroy();

protected:
    CNvProjTrack *m_ownerTrack;
    unsigned int m_srcClipIndex;
    TransitionType m_type;
};

