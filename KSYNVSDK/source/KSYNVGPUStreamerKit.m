//
//  KSYNVGPUStreamerKit.m
//  KSYStreamer
//
//  Created by ksyun
//  Copyright © 2017 ksyun. All rights reserved.
//
#import "KSYNVGPUStreamerKit.h"
#import <mach/mach_time.h>

// 得到系统时间 单位为ns
inline static int64_t getABSTime() {
    static double timeBase = -1;
    if (timeBase < 0) {
        mach_timebase_info_data_t s_timebase_info;
        (void) mach_timebase_info(&s_timebase_info);
        timeBase = s_timebase_info.numer;
        timeBase /= s_timebase_info.denom;
    }
    return (int64_t)( mach_absolute_time() * timeBase);
}

@interface KSYNVGPUStreamerKit ()<NvsVideoFrameReceiverDelegate>{
    NSLock   *_quitLock;  // ensure capDev closed before dealloc
    bool _isTapped;
    CGRect _tapRect;
}
@end

@implementation KSYNVGPUStreamerKit

- (instancetype)init {
    return [self initWithDefaultCfg];
}

/**
 @abstract 初始化方法
 @discussion 创建带有默认参数的 kit，不会打断其他后台的音乐播放
 
 @warning kit只支持单实例推流，构造多个实例会出现异常
 */
- (instancetype) initWithDefaultCfg{
    self = [super initWithDefaultCfg];
    _quitLock = [[NSLock alloc] init];
    _kitReady = false;
    _isTapped = false;
    _videoOrientation = UIInterfaceOrientationPortrait;
    //采集模块
    _nvCap = [[KSYNVCapture alloc] init];
    _nvCap.videoFrameReceiver.delegate = self;
    
    return self;
}

//数据回调
- (void)didVideoFrameReceived:(NvsVideoFrameReceiver *)receiver
                  pixelBuffer:(CVPixelBufferRef)pixelBuffer
                    videoSize:(CGSize)videoSize
                  timelinePos:(int64_t)timelinePos
{
    if(_kitReady){
        if(_isTapped) {
            NvsColor color = [_nvCap pickColor:pixelBuffer selRect:_tapRect];
            [_nvCap setCaptureSceneColor:color];
            _isTapped = false;
        }
        [self.capToGpu processPixelBuffer:pixelBuffer time:CMTimeMake(getABSTime(),NSEC_PER_SEC)];
    }
}

- (void) getTapRect:(CGPoint)pos{
    static const int sampleWidth = 20, sampleHeight = 20;
    
    _tapRect.origin.x = pos.x - sampleWidth / 2;
    if (_tapRect.origin.x < 0)
        _tapRect.origin.x = 0;
    else if (_tapRect.origin.x + sampleWidth > self.preview.bounds.size.width)
        _tapRect.origin.x = self.preview.bounds.size.width - sampleWidth;
    
    _tapRect.origin.y = pos.y - sampleHeight / 2;
    if (_tapRect.origin.y < 0)
        _tapRect.origin.y = 0;
    else if (_tapRect.origin.y + sampleHeight > self.preview.bounds.size.height)
        _tapRect.origin.y = self.preview.bounds.size.height - sampleHeight;
    
    _tapRect.size.width = sampleWidth;
    _tapRect.size.height = sampleHeight;
    _isTapped = true;
}

- (void)dealloc {
    [_quitLock lock];
    [self closeNvKit];
    _nvCap = nil;
    [_quitLock unlock];
    _quitLock = nil;
}

/* reset all submodules */
- (void) closeNvKit{
    _nvCap.videoFrameReceiver.delegate = nil;
    [_nvCap closeNvCap];
}

#pragma mark - Dimension
// 根据朝向, 判断是否需要交换宽和高
-(CGSize) getDimension: (CGSize) sz
           byOriention: (UIInterfaceOrientation) ori {
    CGSize outSz = sz;
    if ( ( ori == UIInterfaceOrientationPortraitUpsideDown ||
          ori == UIInterfaceOrientationPortrait )) {
        outSz.height = MAX(sz.width, sz.height);
        outSz.width  = MIN(sz.width, sz.height);
    }
    else  {
        outSz.height = MIN(sz.width, sz.height);
        outSz.width  = MAX(sz.width, sz.height);
    }
    return outSz;
}
// 居中裁剪
-(CGRect) calcCropRect: (CGSize) camSz to: (CGSize) outSz {
    double x = (camSz.width  -outSz.width )/2/camSz.width;
    double y = (camSz.height -outSz.height)/2/camSz.height;
    double wdt = outSz.width/camSz.width;
    double hgt = outSz.height/camSz.height;
    return CGRectMake(x, y, wdt, hgt);
}

// 对 inSize 按照 targetSz的宽高比 进行裁剪, 得到最大的输出size
-(CGSize) calcCropSize: (CGSize) inSz to: (CGSize) targetSz {
    CGFloat preRatio = targetSz.width / targetSz.height;
    CGSize cropSz = inSz; // set width
    cropSz.height = cropSz.width / preRatio;
    if (cropSz.height > inSz.height){
        cropSz.height = inSz.height; // set height
        cropSz.width  = cropSz.height * preRatio;
    }
    return cropSz;
}

// 分辨率有效范围检查
@synthesize previewDimension = _previewDimension;
- (void) setPreviewDimension:(CGSize) sz{
    _previewDimension.width  = MAX(sz.width, sz.height);
    _previewDimension.height = MIN(sz.width, sz.height);
    _previewDimension.width  = MAX(160, MIN(_previewDimension.width, 1920));
    _previewDimension.height = MAX( 90, MIN(_previewDimension.height,1080));
}

// 根据宽高比计算需要裁剪掉的区域
- (void) updatePreDimension {
    _previewDimension = [self getDimension:_previewDimension
                               byOriention:_videoOrientation];
    CGSize  inSz     =  _captureDimension;
    inSz = [self getDimension:inSz byOriention:_videoOrientation];
    CGSize cropSz = [self calcCropSize:inSz to:_previewDimension];
    self.capToGpu.cropRegion = [self calcCropRect:inSz to:cropSz];
    [self.capToGpu forceProcessingAtSize:_previewDimension];
}

// 分辨率有效范围检查
@synthesize captureDimension = _captureDimension;
- (void) setCaptureDimension:(CGSize) sz{
    _captureDimension.width  = MAX(sz.width, sz.height);
    _captureDimension.height = MIN(sz.width, sz.height);
    _captureDimension.width  = MAX(160, MIN(_captureDimension.width, 1920));
    _captureDimension.height = MAX( 90, MIN(_captureDimension.height,1080));
}

@synthesize videoOrientation = _videoOrientation;
- (void) setVideoOrientation: (UIInterfaceOrientation) orie {
    _videoOrientation   = orie;
}

@end
