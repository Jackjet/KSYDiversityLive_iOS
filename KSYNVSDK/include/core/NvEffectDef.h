//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Mar 6. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvDef.h"
#include "IUnknown.h"
#include <QByteArray>
#include <QString>
#include <QVector>


//
// Effect parameter type
//
enum ENvFxParamType
{
    keNvFxParamTypeInvalid = -1,
    keNvFxParamTypeFirst = 0,
    keNvFxParamTypeArbitrary = 0,
    keNvFxParamTypeInt,
    keNvFxParamTypeFloat,
    keNvFxParamTypeBoolean,
    keNvFxParamTypeMenu,
    keNvFxParamTypeString,
    keNvFxParamTypeColor,
    keNvFxParamTypePosition2D,
    keNvFxParamTypePosition3D,
    keNvFxParamTypeCount,

    // NOTE: the following type can only be used internally
    // NOTE: internal parameter id must be negative!
    keNvFxParamTypeIntArray,
    keNvFxParamTypeFloatArray
};

//
// Effect parameter value
//
struct SNvFxParamVal {
    union {
        void *arbData;          // Used when parameter type is arbitrary
        int intVal;             // Used when parameter type is integer
        double fltVal;          // Used when parameter type is float
        bool boolVal;           // Used when parameter type is boolean
        int menuItemIdx;        // Used when parameter type is menu
        SNvColor colorVal;      // Used when parameter type is color
        SNvPosition2D pos2dVal; // Used when parameter type is position 2D
        SNvPosition3D pos3dVal; // Used when parameter type is position 3D
    } u;

    QString strVal;             // used when parameter type is string

    ENvFxParamType type;
};

struct SNvFxParamIntDef {
    int defVal;
    int minVal, maxVal;
};

struct SNvFxParamFltDef {
    double defVal;
    double minVal, maxVal;
};

struct SNvFxParamBoolDef {
    bool defVal;
};

struct SNvFxParamMenuDef {
    unsigned int defMenuItem;
    unsigned int menuItemCount;
};

struct SNvFxParamColorDef {
    SNvColor defColor;
};

struct SNvFxParamPosition2DDef {
    SNvPosition2D defPos;
};

struct SNvFxParamPosition3DDef {
    SNvPosition3D defPos;
};

enum ENvFxStringParamType
{
    keNvFxStringParamTypeInvalid = -1,
    keNvFxStringParamTypeSingleLine = 0,
    keNvFxStringParamTypeMultiLine,
    keNvFxStringParamTypeFilePath,
    keNvFxStringParamTypeDirectoryPath,
    keNvFxStringParamTypeLabel
};

static inline bool NvIsFxStringParamTypeAPath(ENvFxStringParamType type)
{
    return type == keNvFxStringParamTypeFilePath || type == keNvFxStringParamTypeDirectoryPath;
}

struct SNvFxParamStringDef {
    ENvFxStringParamType type;
};

struct SNvFxParamDef {
    int paramId;			// Parameter unique identifier
    ENvFxParamType type;	// Parameter type

    union {
        SNvFxParamIntDef        id;
        SNvFxParamFltDef        fd;
        SNvFxParamBoolDef       bd;
        SNvFxParamMenuDef       md;
        SNvFxParamColorDef      cd;
        SNvFxParamPosition2DDef p2d;
        SNvFxParamPosition3DDef p3d;
        SNvFxParamStringDef     sd;
    } u;

    QString defaultString;      // Used when parameter type is string
};


//
// Effect description
//
struct SNvEffectDescription
{
    QByteArray name;
    int flags;  // NV_VIDEOEFFECT_FLAG_XXX for video effect, NV_AUDIOEFFECT_FLAG_XXX for audio effect
    unsigned int minInput;
    unsigned int maxInput;
    int progressParamId; // The parameter identifier used as progress of transition effect
};

enum ENvSourceBackgroundMode {
    keNvSourceBackgroundModeColorSolid = 0,
    keNvSourceBackgroundModeBlur = 1
};

interface INvEffectSettings;
interface INvEffectContext;

interface INvEffectDescriptor : public IUnknown
{
    ///////////////////////////////////////////////////////////////////////////////////
    // All the methods in this interface should be thread-safe
    ///////////////////////////////////////////////////////////////////////////////////
    virtual HRESULT __stdcall GetEffectDescription(SNvEffectDescription *out_effectDesc) = 0;

    virtual HRESULT __stdcall GetEffectSettings(INvEffectSettings **out_ppIEffectSettings) = 0;

    // NOTE: becuase INvEffectSettings in itself is NOT thread-safe, you must make sure no one
    // can modify the source effect settings object when calling this methods!!!
    virtual HRESULT __stdcall CloneEffectSettings(INvEffectSettings *pISrcEffectSettings,
                                                  INvEffectSettings **out_ppIClonedEffectSettings) = 0;

    virtual HRESULT __stdcall GetEffectContext(INvEffectContext **out_ppIEffectContext) = 0;

    // Get parameter definition acoording to parameter id, return 0 if paramId is invalid
    virtual const SNvFxParamDef * __stdcall GetParamDef(int in_paramId) = 0;

    virtual void __stdcall GetAllParamDefs(QVector<SNvFxParamDef *> &io_paramDefs) = 0;

    virtual ENvFxParamType __stdcall GetInternalParamType(int in_paramId) = 0;

    //
    // Arbitrary data handling
    //
    virtual void * __stdcall CreateArbData(int in_arbDataId) = 0;
    virtual void * __stdcall CreateArbDataFromFlattenedData(int in_arbDataId, void *in_flattenedData, unsigned int in_dataSize) = 0;
    virtual void __stdcall DestroyArbData(int in_arbDataId, void *in_arbData) = 0;
    virtual void * __stdcall FlattenArbData(int in_arbDataId, void *in_arbData, unsigned int *out_dataSize) = 0;
    virtual void __stdcall ReleaseFlattenedArbData(int in_arbDataId, void *in_flattenedData) = 0;
    virtual void * __stdcall CopyArbData(int in_arbDataId, void *in_arbData) = 0;
};

// NOTE: this interface is NOT thread-safe!!!
interface INvEffectSettings : public IUnknown
{
    virtual HRESULT __stdcall GetEffectDescriptor(INvEffectDescriptor **out_ppIEffectDescriptor) = 0;

    virtual HRESULT __stdcall SetEffectTime(int64_t in_currentTime,
                                            int64_t in_startTime,
                                            int64_t in_endTime) = 0;

    virtual HRESULT __stdcall GetEffectTime(int64_t *out_currentTime,
                                            int64_t *out_startTime,
                                            int64_t *out_endTime) = 0;

    virtual void __stdcall SetTargetVideoAspectRatio(ENvVideoAspectRatio in_targetVideoAspectRatio) = 0;
    virtual ENvVideoAspectRatio __stdcall GetTargetVideoAspectRatio() = 0;

    virtual void __stdcall SetParamVal(int in_paramId, const SNvFxParamVal &in_val) = 0;
    // The returned parameter type will be keNvFxParamTypeInvalid if parameter id is invalid!
    virtual void __stdcall GetParamVal(int in_paramId, SNvFxParamVal &in_val) = 0;

    virtual void __stdcall SetArbParamVal(int in_paramId, void *in_arbData) = 0;
    virtual void * __stdcall GetArbParamVal(int in_paramId) = 0;
    virtual void __stdcall SetIntParamVal(int in_paramId, int in_val) = 0;
    virtual int __stdcall GetIntParamVal(int in_paramId) = 0;
    virtual void __stdcall SetFloatParamVal(int in_paramId, double in_val) = 0;
    virtual double __stdcall GetFloatParamVal(int in_paramId) = 0;
    virtual void __stdcall SetBooleanParamVal(int in_paramId, bool in_val) = 0;
    virtual bool __stdcall GetBooleanParamVal(int in_paramId) = 0;
    virtual void __stdcall SetMenuParamVal(int in_paramId, unsigned int in_val) = 0;
    virtual unsigned int __stdcall GetMenuParamVal(int in_paramId) = 0;
    virtual void __stdcall SetStringParamVal(int in_paramId, const QString &in_val) = 0;
    virtual QString __stdcall GetStringParamVal(int in_paramId) = 0;
    virtual void __stdcall SetColorParamVal(int in_paramId, const SNvColor &in_val) = 0;
    virtual void __stdcall GetColorParamVal(int in_paramId, SNvColor &out_val) = 0;
    virtual void __stdcall SetPosition2DParamVal(int in_paramId, const SNvPosition2D &in_val) = 0;
    virtual void __stdcall GetPosition2DParamVal(int in_paramId, SNvPosition2D &out_val) = 0;
    virtual void __stdcall SetPosition3DParamVal(int in_paramId, const SNvPosition3D &in_val) = 0;
    virtual void __stdcall GetPosition3DParamVal(int in_paramId, SNvPosition3D &out_val) = 0;

    virtual void __stdcall SetIntArrayParamVal(int in_paramId, const int *in_intArray, unsigned int in_count) = 0;
    virtual unsigned int __stdcall GetIntArrayParamDimension(int in_paramId) = 0;
    virtual int __stdcall GetIntArrayParamVal(int in_paramId, unsigned int in_index) = 0;
    virtual void __stdcall SetFloatArrayParamVal(int in_paramId, const float *in_floatArray, unsigned int in_count) = 0;
    virtual unsigned int __stdcall GetFloatArrayParamDimension(int in_paramId) = 0;
    virtual float __stdcall GetFloatArrayParamVal(int in_paramId, unsigned int in_index) = 0;
};

// NOTE: this interface is NOT thread-safe!!!
interface INvEffectContext : public IUnknown
{
    virtual HRESULT __stdcall GetEffectDescriptor(INvEffectDescriptor **out_ppIEffectDescriptor) = 0;

    virtual void __stdcall Reset() = 0;

    virtual void __stdcall ReleaseResources() = 0;
};

