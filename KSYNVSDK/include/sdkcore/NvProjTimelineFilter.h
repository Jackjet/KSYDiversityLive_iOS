//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2017. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 4. 2017
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvProjAppFx.h"


class CNvProjTimeline;
class CNvProjTimelineFilterGroup;

class CNvProjTimelineFilter : public CNvProjAppFx
{
    friend class CNvProjTimeline;
    friend class CNvProjTimelineFilterGroup;

protected:
    CNvProjTimelineFilter(CNvProjTimeline *ownerTimeline,
                          CNvAppFxInstance *fxInstance,
                          CNvProjContext *projContext,
                          ProjObjectType projObjType);
    ~CNvProjTimelineFilter();

public:
    CNvProjTimeline * OwnerTimeline() const { return m_ownerTimeline; }
    int64_t EffectIn() const { return m_effectIn; }
    int64_t EffectOut() const { return m_effectOut; }

    int64_t ChangeEffectIn(int64_t newEffectIn);
    int64_t ChangeEffectOut(int64_t newEffectOut);
    void MoveEffectPosition(int64_t offset);

    void SetClipAffinityEnabled(bool enabled);
    bool GetClipAffinityEnabled() const { return m_clipAffinityEnabled; }

protected:
    // NOTE: If a timeline filter is controlled by a CNvProjTimelineFilterGroup
    // You should NOT call Destroy() directly!,
    // call CNvProjTimelineFilterGroup::RemoveFilter() instead!
    virtual void Destroy();

protected:
    virtual int64_t MinDuration() const { return 0; }
    virtual int64_t MaxDuration() const { return 0; }

    virtual void OnChangeEffectIn(int64_t newEffectIn);
    virtual void OnChangeEffectOut(int64_t newEffectOut);
    virtual void OnMoveEffectPosition(int64_t offset);

protected:
    CNvProjTimeline *m_ownerTimeline;
    CNvProjTimelineFilterGroup *m_ownerGroup;
    CNvProjTimelineFilter *m_prevFilter;
    CNvProjTimelineFilter *m_nextFilter;

    int64_t m_effectIn, m_effectOut;

    bool m_clipAffinityEnabled;

private:
    void Unlink();
};

