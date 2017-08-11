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

#include "NvProjObject.h"
#include "NvAppFx.h"
#include <NvSmartPtr.h>


class CNvProjAppFx : public CNvProjObject
{
protected:
    CNvProjAppFx(CNvAppFxInstance *fxInstance,
                 CNvProjContext *projContext,
                 ProjObjectType projObjType);
    ~CNvProjAppFx();

public:
    CNvAppFxInstance * FxInstance() const { return m_fxInstance; }
    const QByteArray AppFxName() const;
    const QByteArray EffectName() const;

protected:
    virtual void Destroy();

protected:
    TNvSmartPtr<CNvAppFxInstance> m_fxInstance;
};

