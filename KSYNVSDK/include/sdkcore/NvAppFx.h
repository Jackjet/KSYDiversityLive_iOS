//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jun 27. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <NvEffectDef.h>
#include <NvVideoEffect.h>
#include <NvAudioEffect.h>
#include <NvIntpCurves.h>
#include <NvSmartPtr.h>

#include <QObject>
#include <QVariant>
#include <QHash>
#include <QMap>
#include <QVector>
#include <QPair>
#include <QSet>
#include <QReadWriteLock>


#define NV_APPFX_FLAG_FILTER                1
#define NV_APPFX_FLAG_TRANSITION            2
#define NV_APPFX_FLAG_CAPTURE_MODE          4
#define NV_APPFX_FLAG_CAPTURE_FILTER        8
#define NV_APPFX_FLAG_INVISIBLE             16


struct SNvAppFxGeneralParamInfo {
	const char *appFxParamId;
	QString paramName; // Human readable name
	SNvFxParamDef paramDef;
};

struct SNvAppFxMenuParamInfo {
	const char *menuId;
	QString menuName; // Human readable name
	QVector<QPair<QString, QString> > menuItemArray;    // First value in pair is menu item id,
	// second value in pair is menu item human readable name
	QString defMenuId; // Default menu item id
};


class CNvAppFxDesc;

// Function prototype to create application Fx descriptor
typedef CNvAppFxDesc * (*pfnCreateAppFxDesc)();

#define NV_DECLARE_VIDEO_APPFX() \
public: \
	static CNvAppFxDesc * CreateAppFxDescriptor(); \
	virtual bool IsVideoAppFx() const { return true; } \

#define NV_IMPLEMENT_VIDEO_APPFX(APPFX_CLASS_NAME, EFFECT_NAME) \
	CNvAppFxDesc * APPFX_CLASS_NAME::CreateAppFxDescriptor() \
{ \
	TNvSmartPtr<INvEffectDescriptor> pJEffectDesc; \
	NvCreateVideoEffectDescriptor(EFFECT_NAME, pJEffectDesc.GetPtrAddr()); \
	if (!pJEffectDesc) \
	return 0; \
	APPFX_CLASS_NAME *appFxDesc = new APPFX_CLASS_NAME; \
	if (!appFxDesc->Init(pJEffectDesc)) \
	return 0; \
	return static_cast<CNvAppFxDesc *>(appFxDesc); \
}

#define NV_DECLARE_AUDIO_APPFX() \
public: \
	static CNvAppFxDesc * CreateAppFxDescriptor(); \
	virtual bool IsVideoAppFx() const { return false; } \

#define NV_IMPLEMENT_AUDIO_APPFX(APPFX_CLASS_NAME, EFFECT_NAME) \
	CNvAppFxDesc * APPFX_CLASS_NAME::CreateAppFxDescriptor() \
{ \
	TNvSmartPtr<INvEffectDescriptor> pJEffectDesc; \
	NvCreateAudioEffectDescriptor(EFFECT_NAME, pJEffectDesc.GetPtrAddr()); \
	if (!pJEffectDesc) \
	return 0; \
	APPFX_CLASS_NAME *appFxDesc = new APPFX_CLASS_NAME; \
	if (!appFxDesc->Init(pJEffectDesc)) \
	return 0; \
	return static_cast<CNvAppFxDesc *>(appFxDesc); \
}


class CNvAppFxParamId
{
public:
	CNvAppFxParamId(const char *id) : m_id(id) {}

public:
	inline bool operator<(const CNvAppFxParamId &id) const
	{
		if (m_id && id.m_id)
			return strcmp(m_id, id.m_id) < 0;
		else if (!m_id && !id.m_id)
			return true;
		else if (m_id)
			return true;
		else
			return false;
	}

	inline bool operator==(const CNvAppFxParamId &id) const
	{
		if (m_id && id.m_id)
			return strcmp(m_id, id.m_id) == 0;
		else if (!m_id && !id.m_id)
			return true;
		else
			return false;
	}

	const char *Id() const { return m_id; }

private:
	const char *m_id;
};


//
// Application Fx descriptor
//
class CNvAppFxInstance;

class CNvAppFxDesc
{
    friend class CNvAppFxInstance;

protected:
	CNvAppFxDesc();

public:
    virtual ~CNvAppFxDesc();

public:
	void GetEffectDesc(INvEffectDescriptor **ppIEffectDesc) const;

	CNvAppFxInstance * CreateAppFxInstance() const ;

	QVector<SNvAppFxGeneralParamInfo> GetAllGeneralParamsInfo() const;
	bool GetGeneralParamInfo(const char *appFxParamId, SNvAppFxGeneralParamInfo *paramInfo) const;

	QVector<SNvAppFxMenuParamInfo> GetAllMenusInfo() const;
	bool GetMenuInfo(const char *menuId, SNvAppFxMenuParamInfo *menuInfo) const;

	QByteArray EffectName() const;

public:
	virtual QByteArray AppFxName() const = 0;
	virtual QString AppFxDisplayName() const = 0;
	virtual int AppFxFlags() const = 0;

	// Never override the following method explicitly
	virtual bool IsVideoAppFx() const = 0;

private:
	virtual void DescribeAppFx() = 0;

protected:
	bool Init(INvEffectDescriptor *pIEffectDesc);

protected:
	TNvSmartPtr<INvEffectDescriptor> m_pJEffectDesc;

	// Array of parameter value to override the default effect parameter value
	// First value of the pair is effect parameter id
    struct __SNvOverridenDefParamVal {
        int paramId;
        SNvFxParamVal paramVal;
        bool isAspectRatioSuffixedDescFilePath;
    };
    QVector<__SNvOverridenDefParamVal> m_overriddenDefParamValArray;

	struct SNvAppFxGeneralParamDef {
		QString paramName;
		SNvFxParamDef paramDef;
	};

	// App Fx parameter which simply expose the parameter of the internal effect
	// Key is App Fx parameter id(a latin string to uniquely identify the parameter in this App Fx effect),
	// Value contains the corresponding parameter definition of the internal effect
	QMap<CNvAppFxParamId, SNvAppFxGeneralParamDef> m_mapGeneralParam;

#ifndef QT_NO_DEBUG
	mutable QSet<CNvAppFxInstance *> m_instances;
#endif

	struct SNvAppFxMenuItemEntry {
		const char *menuItemId;
		const char *menuItemName;
	};

	// Menu App Fx parameter definition
	struct SNvAppFxMenuParamDef {
		const char *menuId;
		QString menuName;
		int effectMenuParamId;
		QVector<QPair<QString, QString> > menuItemDefArray; // First value in pair is menu item id,
                                                            // second value in pair is menu item human readable name
		QString defMenuId;
        unsigned int defMenuIndex;
	};

	// Menu App Fx parameter map, key is menu id
	QMap<CNvAppFxParamId, SNvAppFxMenuParamDef> m_mapMenu;

protected:
    bool OverrideDefaultEffectParamValue(int effectParamId,
                                         const SNvFxParamVal &paramVal,
                                         bool isAspectRatioSuffixedDescFilePath = false);

	bool DefineAppFxGeneralParam(const char *appFxParamId,
		const QString &paramName,
		int effectParamId,
		const SNvFxParamVal *defVal = 0);

	bool DefineAppFxMenuParam(const char *menuId,
		const QString menuName,
		int effectMenuParamId,
		const SNvAppFxMenuItemEntry *menuItemEntries,
		int defMenuItemIndex = -1);

private:
	const SNvAppFxGeneralParamDef * GetGeneralParamDef(const CNvAppFxParamId &paramId) const;
    const SNvAppFxMenuParamDef * GetMenuDef(const CNvAppFxParamId &menuId) const;
};


//
// Application Fx instance
//
class CNvAppFxInstance
{
    friend class CNvAppFxDesc;

protected:
	CNvAppFxInstance(const CNvAppFxDesc *appFxDesc);
	virtual ~CNvAppFxInstance();

public:
	//
	// NOTE: The followling methods are thread-safe!
	//
	const CNvAppFxDesc * AppFxDesc() const { return m_appFxDesc; }

    // NOTE: if the parameter type is keNvFxParamTypeArbitrary,
    // this method will take the ownership of the arbitrary data
    void SetGeneralParamVal(const char *appFxParamId, const SNvFxParamVal *paramVal);
	// NOTE: if the parameter type is keNvFxParamTypeArbitrary,
	// the getter should destroy the returned arbitrary data using DestroyArbData();
	bool GetGeneralParamVal(const char *appFxParamId, SNvFxParamVal *paramVal);

    void SetIntGeneralParamVal(const char *appFxParamId, int val);
    int GetIntGeneralParamVal(const char *appFxParamId);

    void SetFloatGeneralParamVal(const char *appFxParamId, double val);
    double GetFloatGeneralParamVal(const char *appFxParamId);

    void SetBooleanGeneralParamVal(const char *appFxParamId, bool val);
    bool GetBooleanGeneralParamVal(const char *appFxParamId);

    void SetMenuGeneralParamVal(const char *appFxParamId, int val);
    int GetMenuGeneralParamVal(const char *appFxParamId);

    void SetStringGeneralParamVal(const char *appFxParamId, const QString &val);
    QString GetStringGeneralParamVal(const char *appFxParamId);

    // NOTE: This method will take the ownership of the arbitrary data
    void SetArbitraryGeneralParamVal(const char *appFxParamId, void *val);
    // NOTE: It is up to the caller to destroy the returned arbitrary data
    void * GetArbitraryGeneralParamVal(const char *appFxParamId);

    void SetColorGeneralParamVal(const char *appFxParamId, const SNvColor &val);
    SNvColor GetColorGeneralParamVal(const char *appFxParamId);

    void SetPosition2DGeneralParamVal(const char *appFxParamId, const SNvPosition2D &val);
    SNvPosition2D GetSNvPosition2DGeneralParamVal(const char *appFxParamId);

    void SetPosition3DGeneralParamVal(const char *appFxParamId, const SNvPosition3D &val);
    SNvPosition3D GetSNvPosition3DGeneralParamVal(const char *appFxParamId);

    // NOTE: You can't use the following methods to set/get parameter value if its type is keNvFxParamTypeArbitrary
    void SetGeneralParamValWithQVariant(const char *appFxParamId, const QVariant &paramVal);

    double GetGeneralParamIntpCurvesVal(const char *appFxParamId, int64_t effectTime);
	void DestroyArbData(const char *appFxParamId, void *arbData);

    // NOTE: AppFx instance will take the ownership of the intepolation curves handle on success,
    // The old intepolation curves handle(if there is one) will be destroyed
    bool SetIntpCurvesToGeneralParamVal(const char *appFxParamId, NV_HINTPCURVES hIntpCurves);
    bool AppendPointToIntpCurvesOfGeneralParamVal(const char *appFxParamId, double x, double y);
    void DestroyIntpCurvesOfGeneralParamVal(const char *appFxParamId);

	void SetMenuVal(const char *menuId, const QString &menuItemId);
	QString GetMenuVal(const char *menuId);
    // Return -1 indicate error
    int GetMenuIndexVal(const char *menuId);

	void FillEffectSettings(INvEffectSettings *pIEffectSettings);

public:
    void AddRef();
    void Release();

private:
    void DoSetGeneralParamVal(const char *appFxParamId, const SNvFxParamVal *paramVal);
    bool DoGetGeneralParamVal(const char *appFxParamId, SNvFxParamVal *paramVal);

    const CNvAppFxDesc::SNvAppFxGeneralParamDef * ValidateParamType(const char *appFxParamId,
                                                                    ENvFxParamType expectedType) const;

private:
    QAtomicInt m_refCount;

protected:
	const CNvAppFxDesc *m_appFxDesc;

    QHash<QByteArray, QPair<int, SNvFxParamVal> > m_mapGeneralParamVal;
    // NOTE: x coordinates of the interpolation curves is millisecond
    QHash<QByteArray, QPair<int, NV_HINTPCURVES> > m_generalParamCurveTable;
    QHash<QByteArray, QPair<QString, unsigned int> > m_mapMenuVal;
	QReadWriteLock m_rwLock;
};

