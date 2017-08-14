//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jun 23. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include "NvFrameworkDef.h"


///////////////////////////////////////////////////////////////////////////////////
// Video effect render flag for GPU resizer
///////////////////////////////////////////////////////////////////////////////////
#define NV_VIDEOEFFECT_GPU_RESIZER_RENDER_FLAG_FIT_OUTPUT               1
#define NV_VIDEOEFFECT_GPU_RESIZER_RENDER_FLAG_USE_MIPMAP               2

#define NV_VIDEOEFFECT_GPU_RESIZER_RENDER_FLAG_FIT_MODE_MASK_SHIFT      8
#define NV_VIDEOEFFECT_GPU_RESIZER_RENDER_FLAG_FIT_MODE_MASK            (3 << NV_VIDEOEFFECT_GPU_RESIZER_RENDER_FLAG_FIT_MODE_MASK_SHIFT)
#define NV_VIDEOEFFECT_GPU_RESIZER_RENDER_FLAG_FIT_MODE_LETTER_BOX      (0 << NV_VIDEOEFFECT_GPU_RESIZER_RENDER_FLAG_FIT_MODE_MASK_SHIFT)
#define NV_VIDEOEFFECT_GPU_RESIZER_RENDER_FLAG_FIT_MODE_PAN_AND_SCAN    (1 << NV_VIDEOEFFECT_GPU_RESIZER_RENDER_FLAG_FIT_MODE_MASK_SHIFT)
#define NV_VIDEOEFFECT_GPU_RESIZER_RENDER_FLAG_FIT_MODE_STRETCH         (2 << NV_VIDEOEFFECT_GPU_RESIZER_RENDER_FLAG_FIT_MODE_MASK_SHIFT)


///////////////////////////////////////////////////////////////////////////////////
// Video effect parameter id
///////////////////////////////////////////////////////////////////////////////////

//
// GPU Resizer
//
#define NV_GPU_RESIZER_PARAM_ID_SCALE_FACTOR                100
#define NV_GPU_RESIZER_PARAM_ID_PAN                         200
#define NV_GPU_RESIZER_PARAM_ID_SCAN                        300

#define NV_GPU_RESIZER_PARAM_ID_BACKGROUND_COLOR            400

//
// GPU transform 2D
//
#define NV_GPU_TRANSFORM2D_PARAM_ID_SCALE_X                 100
#define NV_GPU_TRANSFORM2D_PARAM_ID_SCALE_Y                 200
#define NV_GPU_TRANSFORM2D_PARAM_ID_ROTATION_ANGLE          300
#define NV_GPU_TRANSFORM2D_PARAM_ID_ANCHOR_X                400
#define NV_GPU_TRANSFORM2D_PARAM_ID_ANCHOR_Y                500
#define NV_GPU_TRANSFORM2D_PARAM_ID_TRANSLATION_X           600
#define NV_GPU_TRANSFORM2D_PARAM_ID_TRANSLATION_Y           700
#define NV_GPU_TRANSFORM2D_PARAM_ID_OPACITY                 800
#define NV_GPU_TRANSFORM2D_PARAM_ID_TEX_WRAP_MODE           900
#define NV_GPU_TRANSFORM2D_PARAM_ID_TEX_ANCHOR_X            1000
#define NV_GPU_TRANSFORM2D_PARAM_ID_TEX_ANCHOR_Y            1100
#define NV_GPU_TRANSFORM2D_PARAM_ID_TEX_SCALE_X             1200
#define NV_GPU_TRANSFORM2D_PARAM_ID_TEX_SCALE_Y             1300
#define NV_GPU_TRANSFORM2D_PARAM_ID_TEX_ROTATION_Z          1400
#define NV_GPU_TRANSFORM2D_PARAM_ID_TEX_TRANSLATION_X       1500
#define NV_GPU_TRANSFORM2D_PARAM_ID_TEX_TRANSLATION_Y       1600

enum ENvTransform2DWrapMode
{
    keNvTransform2DWrapMode_ClampToEdge = 0,
    keNvTransform2DWrapMode_MirroredRepeat,
    keNvTransform2DWrapMode_Repeat,
    keNvTransform2DWrapMode_Count
};


//
// Storyboard effect
//
#define NV_STORYBOARD_PARAM_ID_DESCRIPTION_FILE_PATH            100
#define NV_STORYBOARD_PARAM_ID_DESCRIPTION_STRING               110
#define NV_STORYBOARD_PARAM_ID_RESOURCE_DIR_PATH                120
#define NV_STORYBOARD_PARAM_ID_EFFECTIVE_SCENE_WIDTH            130
#define NV_STORYBOARD_PARAM_ID_EFFECTIVE_SCENE_HEIGHT           140
#define NV_STORYBOARD_PARAM_ID_FILL_MODE                        200
#define NV_STORYBOARD_PARAM_ID_NO_BACKGROUND                    300
#define NV_STORYBOARD_PARAM_ID_STRETCH_TIME                     400
#define NV_STORYBOARD_PARAM_ID_IS_CAPTION                       500
#define NV_STORYBOARD_PARAM_ID_SHOW_CAPTION_POSTER              600
#define NV_STORYBOARD_PARAM_ID_CAPTION_FONT_SCALE_FACTOR        700
#define NV_STORYBOARD_PARAM_ID_CAPTION_ANCHOR_X                 710
#define NV_STORYBOARD_PARAM_ID_CAPTION_ANCHOR_Y                 720
#define NV_STORYBOARD_PARAM_ID_CAPTION_SCALE_X                  730
#define NV_STORYBOARD_PARAM_ID_CAPTION_SCALE_Y                  740
#define NV_STORYBOARD_PARAM_ID_CAPTION_TRANSLATION_X            800
#define NV_STORYBOARD_PARAM_ID_CAPTION_TRANSLATION_Y            900
#define NV_STORYBOARD_PARAM_ID_IS_PANORAMIC_CAPTION                         910
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_CAPTION_CENTER_POLAR_ANGLE         920
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_CAPTION_CENTER_AZIMUTH_ANGLE       930
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_CAPTION_POLAR_ANGLE_RANGE          940
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_CAPTION_ORIGINAL_CONTENT_WIDTH     950
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_CAPTION_ORIGINAL_CONTENT_HEIGHT    960
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_CAPTION_SCALE_X                    970
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_CAPTION_SCALE_Y                    980
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_CAPTION_ROTATION                   990
#define NV_STORYBOARD_PARAM_ID_IS_ANIMATED_STICKER              1000
#define NV_STORYBOARD_PARAM_ID_SHOW_ANIMATED_STICKER_POSTER     1100
#define NV_STORYBOARD_PARAM_ID_ANIMATED_STICKER_SCALE           1200
#define NV_STORYBOARD_PARAM_ID_ANIMATED_STICKER_FLIP_HORIZONTAL 1300
#define NV_STORYBOARD_PARAM_ID_ANIMATED_STICKER_FLIP_VERTICAL   1400
#define NV_STORYBOARD_PARAM_ID_ANIMATED_STICKER_ROTATION_Z      1500
#define NV_STORYBOARD_PARAM_ID_ANIMATED_STICKER_TRANSLATION_X   1600
#define NV_STORYBOARD_PARAM_ID_ANIMATED_STICKER_TRANSLATION_Y   1700
#define NV_STORYBOARD_PARAM_ID_ANIMATED_STICKER_OFFSET_X        1800
#define NV_STORYBOARD_PARAM_ID_ANIMATED_STICKER_OFFSET_Y        1900
#define NV_STORYBOARD_PARAM_ID_IS_PANORAMIC_ANIMATED_STICKER                        1910
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_ANIMATED_STICKER_CENTER_POLAR_ANGLE        1920
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_ANIMATED_STICKER_CENTER_AZIMUTH_ANGLE      1930
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_ANIMATED_STICKER_POLAR_ANGLE_RANGE         1940
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_ANIMATED_STICKER_ORIGINAL_CONTENT_WIDTH    1950
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_ANIMATED_STICKER_ORIGINAL_CONTENT_HEIGHT   1960
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_ANIMATED_STICKER_SCALE                     1970
#define NV_STORYBOARD_PARAM_ID_PANORAMIC_ANIMATED_STICKER_ROTATION                  1980
#define NV_STORYBOARD_PARAM_ID_EXTERNAL_SOURCE_IMAGE1_PATH      2000
#define NV_STORYBOARD_PARAM_ID_EXTERNAL_SOURCE_IMAGE2_PATH      2100
#define NV_STORYBOARD_PARAM_ID_EXTERNAL_SOURCE_IMAGE3_PATH      2200


enum ENvStoryboardFillMode
{
    keNvStoryboardFillMode_PanAndScan = 0,
    keNvStoryboardFillMode_LetterBox,
    keNvStoryboardFillMode_Stretch,
    keNvStoryboardFillMode_Count
};


//
// Storyboard wipe effect
//
#define NV_STORYBOARD_WIPE_PARAM_ID_MASK_DESCRIPTION_FILE_PATH      100
#define NV_STORYBOARD_WIPE_PARAM_ID_OVERLAY_DESCRIPTION_FILE_PATH   200
#define NV_STORYBOARD_WIPE_PARAM_ID_FILL_MODE                       300
#define NV_STORYBOARD_WIPE_PARAM_ID_PROGRESS                        400
#define NV_STORYBOARD_WIPE_PARAM_ID_STRETCH_TIME                    500

enum ENvStoryboardWipeFillMode
{
    keNvStoryboardWipeFillMode_PanAndScan = 0,
    keNvStoryboardWipeFillMode_LetterBox,
    keNvStoryboardWipeFillMode_Stretch,
    keNvStoryboardWipeFillMode_Count
};


//
// Storyboard 3D effect
//
#define NV_STORYBOARD3D_PARAM_ID_DESCRIPTION_FILE_PATH  100
#define NV_STORYBOARD3D_PARAM_ID_BACKGROUND_COLOR       200
#define NV_STORYBOARD3D_PARAM_ID_PROGRESS               300
#define NV_STORYBOARD3D_PARAM_ID_STRETCH_TIME           400


//
// Face Unity Effect
//
#define NV_FACE_UNITY_EFFECT_PARAM_ID_FACE_TYPE         100
#define NV_FACE_UNITY_EFFECT_PARAM_ID_FACE_SHAPE_LEVEL  200
#define NV_FACE_UNITY_EFFECT_PARAM_ID_CHEEK_THINNING    300
#define NV_FACE_UNITY_EFFECT_PARAM_ID_EYE_ENLARGE       400
#define NV_FACE_UNITY_EFFECT_PARAM_ID_FACE_ORNAMENT     500

enum ENvFaceUnityEffectFaceType
{
    keNvFaceUnityEffectFaceType_Default = 0,
    keNvFaceUnityEffectFaceType_Goddess,
    keNvFaceUnityEffectFaceType_InternetCelebrity,
    keNvFaceUnityEffectFaceType_Natural,
    keNvFaceUnityEffectFaceType_Count
};


// Video Fade effect
#define NV_VIDEO_FADE_PARAM_ID_PROGRESS             100

// Beauty effect
#define NV_BEAUTY_PARAM_ID_AMPLITUDE                100
#define NV_BEAUTY_PARAM_ID_WHITENING                200

// Fast Blur effect
#define NV_FAST_BLUR_PARAM_ID_BLUR_SIZE             100

// Border effect
#define NV_BORDER_PARAM_ID_OPACITY                  100
#define NV_BORDER_PARAM_ID_PATTERNIMAGE_PATH        200
#define NV_BORDER_PARAM_ID_MOVE_LENGTH              300
#define NV_BORDER_PARAM_ID_MOVE_ORENTATION          400

enum ENvBorderMovingMode
{
    keNvBorderMovingMode_Horizontal,
    keNvBorderMovingMode_Vertical,
    keNvBorderMovingMode_Count
};

//
// Video Echo
//
#define NV_VIDEO_ECHO_PARAM_ID_ECHO_FRAME_COUNT     100
#define NV_VIDEO_ECHO_PARAM_ID_ECHO_FRAME_INTERVAL  200
#define NV_VIDEO_ECHO_PARAM_ID_ENABLE_CHANNEL_GAIN  300

//
// Panoramic mapping
//
#define NV_PANORAMIC_MAPPING_PARAM_ID_MIN_PANORAMIC_COORD_S     100
#define NV_PANORAMIC_MAPPING_PARAM_ID_MIN_PANORAMIC_COORD_T     200
#define NV_PANORAMIC_MAPPING_PARAM_ID_MAX_PANORAMIC_COORD_S     300
#define NV_PANORAMIC_MAPPING_PARAM_ID_MAX_PANORAMIC_COORD_T     400
#define NV_PANORAMIC_MAPPING_PARAM_ID_CENTER_POLAR_ANGLE        500
#define NV_PANORAMIC_MAPPING_PARAM_ID_CENTER_AZIMUTH_ANGLE      600
#define NV_PANORAMIC_MAPPING_PARAM_ID_POLAR_ANGLE_RANGE         700
#define NV_PANORAMIC_MAPPING_PARAM_ID_ORTHO_ANGLE_RANGE         800
#define NV_PANORAMIC_MAPPING_PARAM_ID_SCALE_X                   900
#define NV_PANORAMIC_MAPPING_PARAM_ID_SCALE_Y                   1000
#define NV_PANORAMIC_MAPPING_PARAM_ID_ROTATION                  1100

// Push
#define NV_PUSH_PARAM_ID_PROGRESS                   100
#define NV_PUSH_PARAM_ID_DIRECTION                  200

enum ENvPushOrientation
{
    keNvPushOrientation_LeftToRight = 0,
    keNvPushOrientation_RightToLeft,
    keNvPushOrientation_TopToBottom,
    keNvPushOrientation_BottomToTop,
    keNvPushOrientation_Count
};

// Page curl effect
#define NV_PAGECURL_PARAM_ID_CURLMODE               100
#define NV_PAGECURL_PARAM_ID_ANGLE                  300
#define NV_PAGECURL_PARAM_ID_RADUIS                 400
#define NV_PAGECURL_PARAM_ID_HIGHLIGHT_RED          500
#define NV_PAGECURL_PARAM_ID_HIGHLIGHT_GREEN        600
#define NV_PAGECURL_PARAM_ID_HIGHLIGHT_BLUE         700
#define NV_PAGECURL_PARAM_ID_HIGHLIGHT_STRENTH      800
#define NV_PAGECURL_PARAM_ID_HIGHLIGHT_ROTATION_X   900
#define NV_PAGECURL_PARAM_ID_HIGHLIGHT_ROTATION_Y   1000
#define NV_PAGECURL_PARAM_ID_HIGHLIGHT_ROTATION_Z   1100
#define NV_PAGECURL_PARAM_ID_HIGHLIGHT_VISIBLE      1200
#define NV_PAGECURL_PARAM_ID_PROGRESS               1300

enum ENvPageCurlMode
{
    keNvPageCurlMode_Turning = 0,
    keNvPageCurlMode_Curling,
    keNvPageCurlMode_Count
};

// Wipe effect
#define NV_WIPE_PARAM_ID_PATTERNIMAGE_PATH          100
#define NV_WIPE_PARAM_ID_BORDER_WIDTH               200
#define NV_WIPE_PARAM_ID_PROGRESS                   300
#define NV_WIPE_PARAM_ID_MODE                       400

enum ENvWipeMode
{
    keNvWipeMode_Stretch = 0,
    keNvWipeMode_LitterBox,
    keNvWipeMode_PanAndScan,
    keNvWipeMode_Count
};

// Montion wipe effect
#define NV_MONTION_WIPE_PARAM_ID_PROGRESS           100
#define NV_MONTION_WIPE_PARAM_ID_PATTERNIMAGE_PATH  200
#define NV_MONTION_WIPE_PARAM_ID_FRONTIMAGE_PATH    300
#define NV_MONTION_WIPE_PARAM_ID_ANGLE              400
#define NV_MONTION_WIPE_PARAM_ID_MONTION_MODE       500
#define NV_MONTION_WIPE_PARAM_ID_FRONTIMAGE_SCALE   600

enum ENvMontionWipeMontionMode
{
    keNvMontionWipeMode_BottomLeftToTopRight = 0,
    keNvMontionWipeMode_BottomRightToTopLeft,
    keNvMontionWipeMode_LeftToRight,
    keNvMontionWipeMode_TopRightToBottomLeft,
    keNvMontionWipeMode_BottomToTop,
    keNvMontionWipeMode_RightToLeft,
    keNvMontionWipeMode_Count
};

// Tilt shift lens
#define NV_TILTSHIFT_LENS_PARAM_ID_CLEAR_AREA_TOP     100
#define NV_TILTSHIFT_LENS_PARAM_ID_CLEAR_AREA_BOTTOM  200

// Fish eye effect
#define NV_FISHEYE_PARAM_ID_SCALE                   100

// Lomo effect
#define NV_LOMO_PARAM_ID_STRENTH                    100
#define NV_LOMO_PARAM_ID_CENTER_RANGE               200

// Color correction
#define NV_COLORCORRECTION_PARAM_ID_STRENTH         100

// Warm color effect
#define NV_WARMCOLOR_PARAM_ID_STRENTH               100

// Reminiscent color effect
#define NV_REMINISCENTCOLOR_PARAM_ID_STRENTH        100

// Illusion color effect
#define NV_ILLUSIONCOLOR_PARAM_ID_STRENTH           100

// Memory color effect
#define NV_MEMORYCOLOR_PARAM_ID_STRENTH             100

// Yellowing color effect
#define NV_YELLOWINGCOLOR_PARAM_ID_STRENTH          100

// Color adjustment no mask effect
#define NV_COLORADJUSTMENT_NO_MASK_VIVID_GAIN           100

// Basic cc effect
#define NV_COLORADJUSTMENT_MASTER_PARAM_ID_SATURATION			100
#define NV_COLORADJUSTMENT_MASTER_PARAM_ID_CONTRAST				200
#define NV_COLORADJUSTMENT_MASTER_PARAM_ID_BALANCE_HUE			300
#define NV_COLORADJUSTMENT_MASTER_PARAM_ID_BALANCE_STRENGTH		400
#define NV_COLORADJUSTMENT_MASTER_PARAM_ID_GAIN_MASTER			500
#define NV_COLORADJUSTMENT_MASTER_PARAM_ID_GAIN_RED				600
#define NV_COLORADJUSTMENT_MASTER_PARAM_ID_GAIN_GREEN			700
#define NV_COLORADJUSTMENT_MASTER_PARAM_ID_GAIN_BLUE			800
#define NV_COLORADJUSTMENT_MASTER_PARAM_ID_OFFSET_MASTER		900
#define NV_COLORADJUSTMENT_MASTER_PARAM_ID_OFFSET_RED			1000
#define NV_COLORADJUSTMENT_MASTER_PARAM_ID_OFFSET_GREEN			1100
#define NV_COLORADJUSTMENT_MASTER_PARAM_ID_OFFSET_BLUE			1200
#define NV_COLORADJUSTMENT_SHADOWS_PARAM_ID_SATURATION			1300
#define NV_COLORADJUSTMENT_SHADOWS_PARAM_ID_CONTRAST			1400
#define NV_COLORADJUSTMENT_SHADOWS_PARAM_ID_BALANCE_HUE			1500
#define NV_COLORADJUSTMENT_SHADOWS_PARAM_ID_BALANCE_STRENGTH	1600
#define NV_COLORADJUSTMENT_SHADOWS_PARAM_ID_GAIN_MASTER			1700
#define NV_COLORADJUSTMENT_SHADOWS_PARAM_ID_GAIN_RED			1800
#define NV_COLORADJUSTMENT_SHADOWS_PARAM_ID_GAIN_GREEN			1900
#define NV_COLORADJUSTMENT_SHADOWS_PARAM_ID_GAIN_BLUE			2200
#define NV_COLORADJUSTMENT_SHADOWS_PARAM_ID_OFFSET_MASTER		2300
#define NV_COLORADJUSTMENT_SHADOWS_PARAM_ID_OFFSET_RED			2400
#define NV_COLORADJUSTMENT_SHADOWS_PARAM_ID_OFFSET_GREEN		2500
#define NV_COLORADJUSTMENT_SHADOWS_PARAM_ID_OFFSET_BLUE			2600
#define NV_COLORADJUSTMENT_MIDTONES_PARAM_ID_SATURATION			2700
#define NV_COLORADJUSTMENT_MIDTONES_PARAM_ID_CONTRAST			2800
#define NV_COLORADJUSTMENT_MIDTONES_PARAM_ID_BALANCE_HUE		2900
#define NV_COLORADJUSTMENT_MIDTONES_PARAM_ID_BALANCE_STRENGTH	3000
#define NV_COLORADJUSTMENT_MIDTONES_PARAM_ID_GAIN_MASTER		3100
#define NV_COLORADJUSTMENT_MIDTONES_PARAM_ID_GAIN_RED			3200
#define NV_COLORADJUSTMENT_MIDTONES_PARAM_ID_GAIN_GREEN			3300
#define NV_COLORADJUSTMENT_MIDTONES_PARAM_ID_GAIN_BLUE			3400
#define NV_COLORADJUSTMENT_MIDTONES_PARAM_ID_OFFSET_MASTER		3500
#define NV_COLORADJUSTMENT_MIDTONES_PARAM_ID_OFFSET_RED			3600
#define NV_COLORADJUSTMENT_MIDTONES_PARAM_ID_OFFSET_GREEN		3700
#define NV_COLORADJUSTMENT_MIDTONES_PARAM_ID_OFFSET_BLUE		3800
#define NV_COLORADJUSTMENT_HIGHLIGHTS_PARAM_ID_SATURATION		3900
#define NV_COLORADJUSTMENT_HIGHLIGHTS_PARAM_ID_CONTRAST			4000
#define NV_COLORADJUSTMENT_HIGHLIGHTS_PARAM_ID_BALANCE_HUE		4100
#define NV_COLORADJUSTMENT_HIGHLIGHTS_PARAM_ID_BALANCE_STRENGTH	4200
#define NV_COLORADJUSTMENT_HIGHLIGHTS_PARAM_ID_GAIN_MASTER		4300
#define NV_COLORADJUSTMENT_HIGHLIGHTS_PARAM_ID_GAIN_RED			4400
#define NV_COLORADJUSTMENT_HIGHLIGHTS_PARAM_ID_GAIN_GREEN		4500
#define NV_COLORADJUSTMENT_HIGHLIGHTS_PARAM_ID_GAIN_BLUE		4600
#define NV_COLORADJUSTMENT_HIGHLIGHTS_PARAM_ID_OFFSET_MASTER	4700
#define NV_COLORADJUSTMENT_HIGHLIGHTS_PARAM_ID_OFFSET_RED		4800
#define NV_COLORADJUSTMENT_HIGHLIGHTS_PARAM_ID_OFFSET_GREEN		4900
#define NV_COLORADJUSTMENT_HIGHLIGHTS_PARAM_ID_OFFSET_BLUE		5000

#define NV_COLORADJUSTMENT_PARAM_ID_IS_PROCESS_OTHER_TONES      6000


//
// Foreground animation effect
//
#define NV_FOREGROUNDANIMATION_PARAM_ID_FILE_PATH               100
#define NV_FOREGROUNDANIMATION_PARAM_ID_LOOP_MODE               200
// In milliseconds
#define NV_FOREGROUNDANIMATION_PARAM_ID_LOOP_PERIOD             250
#define NV_FOREGROUNDANIMATION_PARAM_ID_HORIZTONAL_ALIGNMENT    300
#define NV_FOREGROUNDANIMATION_PARAM_ID_VERTICAL_ALIGNMENT      400
#define NV_FOREGROUNDANIMATION_PARAM_ID_FILL_MODE               500
#define NV_FOREGROUNDANIMATION_PARAM_ID_FILE_START_POS          600
#define NV_FOREGROUNDANIMATION_PARAM_ID_BLENDING_MODE           700

enum ENvForegroundAnimationAlignment
{
    keNvForegroundAnimationAlignment_Center = 0,
    keNvForegroundAnimationAlignment_Top,
    keNvForegroundAnimationAlignment_Bottom,
    keNvForegroundAnimationAlignment_Left = keNvForegroundAnimationAlignment_Top,
    keNvForegroundAnimationAlignment_Right = keNvForegroundAnimationAlignment_Bottom,
    keNvForegroundAnimationAlignment_Count
};

enum ENvForegroundAnimationFillMode
{
    keNvForegroundAnimationFillMode_Normal = 0,
    keNvForegroundAnimationFillMode_LitterBox,
    keNvForegroundAnimationFillMode_PanAndScan,
    keNvForegroundAnimationFillMode_Stretch,
    keNvForegroundAnimationFillMode_Repeat,
    keNvForegroundAnimationFillMode_MirroredRepeat,
    keNvForegroundAnimationFillMode_Count
};

enum ENvForegroundAnimationLoopMode
{
    keNvForegroundAnimationLoopMode_Normal = 0,
    keNvForegroundAnimationLoopMode_Mirror,
    keNvForegroundAnimationLoopMode_Count
};

enum ENvForegroundAnimationBlendingMode
{
    keNvForegroundAnimationBlendingMode_Normal = 0,
    keNvForegroundAnimationBlendingMode_Multiply,
    keNvForegroundAnimationBlendingMode_Subtract,
    keNvForegroundAnimationBlendingMode_Screen,
    keNvForegroundAnimationBlendingMode_Add,
    keNvForegroundAnimationBlendingMode_Exclusion,
    keNvForegroundAnimationBlendingMode_Count
};


// Master keyer effect
#define NV_MASTER_KEYER_PARAM_ID_KEY_COLOR                  100
#define NV_MASTER_KEYER_PARAM_ID_SOFTNESS_AMENDMENT         200
#define NV_MASTER_KEYER_PARAM_ID_SPILL_REMOVAL              300
#define NV_MASTER_KEYER_PARAM_ID_SPILL_REMOVAL_INTENSITY    400
#define NV_MASTER_KEYER_PARAM_ID_SHRINK_INTENSITY           500

