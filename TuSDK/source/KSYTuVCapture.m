//
//  KSYTuVCapture.m
//
//  Created by ksyun.
//  Copyright © 2017 ksyun. All rights reserved.
//


#import "KSYTuVCapture.h"

@interface KSYTuVCapture(){
}
@end

@implementation KSYTuVCapture

-(id)init{
    self = [super init];
    _camera = [TuSDKLiveVideoCamera initWithSessionPreset:AVCaptureSessionPresetHigh cameraPosition:AVCaptureDevicePositionFront cameraView:nil];
    // 配置相机参数
    /**
     输出画面分辨率，默认原始采样尺寸输出。
     如果设置了输出尺寸，则对画面进行等比例缩放，必要时进行裁剪，保证输出尺寸和预设尺寸一致。
     */
    _camera.outputSize = CGSizeMake(360, 640);
    // 禁用持续自动对焦
    _camera.disableContinueFoucs = NO;
    // 视频视图显示比例 (默认：0， 0 <= mRegionRatio, 当设置为0时全屏显示)
    _camera.cameraViewRatio = 0;
    // 视频覆盖区域颜色 (默认：[UIColor blackColor])
    _camera.regionViewColor = [UIColor blackColor];
    // 禁用前置摄像头自动水平镜像 (默认: NO，前置摄像头拍摄结果自动进行水平镜像)
    _camera.disableMirrorFrontFacing = NO;
    // 默认闪光灯模式
    [_camera flashWithMode:AVCaptureFlashModeOff];
    /**
     相机采集帧率，默认 20 帧
     */
    _camera.frameRate = 20;
    // 启用智能贴纸
    _camera.enableLiveSticker = YES;
    // 设置滤镜
    [_camera switchFilterWithCode:nil];
    // 设置静音
    [_camera setMuted:NO];
    //关闭自带音频采集
    _camera.enableAudioCapture = false;
    
    return self;
}

- (void) startCapture{
    if (_camera){
        [_camera tryStartCameraCapture];
        [_camera startRecording];
    }
}

- (void) stopCapture{
    if (_camera) {
        [_camera cancelRecording];
        [_camera destory];
        _camera = nil;
    }
}

- (void)dealloc{
    if (_camera){
        [_camera destory];
        _camera = nil;
    }
}

@end
