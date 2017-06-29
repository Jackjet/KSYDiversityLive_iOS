#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>

@interface KSYFaceUnityWrapper:NSObject
/**
 @abstract   构造函数
 @param      items
 
 **/

-(id) initWithArray:(NSArray *) items;

-(void)renderFaceUnity:(CMSampleBufferRef)inSampleBuffer;

@property(nonatomic, copy) void(^videoProcessingCallback)(CMSampleBufferRef  sampleBuffer);

/**
 @abstract      瘦脸参数，大于等于0的浮点数，0为关闭效果，1为默认效果，大于1为进一步增强效果
 **/
@property(nonatomic, assign) float cheekThinning;

/**
 @abstract      大眼参数，大于等于0的浮点数，0为关闭效果，1为默认效果，大于1为进一步增强效果
 **/
@property(nonatomic, assign) float eyeEnlarging;

/**
 @abstract      美白参数，大于等于0的浮点数，0为关闭效果，1为默认效果，大于1为进一步增强效果
 **/
@property(nonatomic, assign) float colorLevel;

/**
 @abstract      磨皮参数，取值范围为0-6
 **/
@property(nonatomic, assign) float blurLevel;

/**
 @abstract      滤镜索引
 **/
@property(nonatomic, assign) int filterIndex;

/**
 @abstract      贴纸索引
 **/
@property (nonatomic,assign) int stickerIndex;

@end
