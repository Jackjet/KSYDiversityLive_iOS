//
//  KSYTuGPUStreamerKit.m
//  KSYStreamer
//
//  Created by ksyun
//  Copyright © 2017 ksyun. All rights reserved.
//
#import "KSYTuGPUStreamerKit.h"

@interface KSYTuGPUStreamerKit(){
}
@end

@implementation KSYTuGPUStreamerKit

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
    _videoOrientation = UIInterfaceOrientationPortrait;
    //视频采集模块
    _tuVCap = [[KSYTuVCapture alloc]init];
    
    return self;
}

/**
 启动摄像头
 */
- (void)startVideoCap
{
    AVAuthorizationStatus status_video = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    if ( status_video == AVAuthorizationStatusDenied) {
        NSLog(@"请开启相机采集权限");
        return;
    }
    [_tuVCap startCapture];
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
