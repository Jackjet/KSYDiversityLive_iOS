//
//  KSYNVCaptureScenceKit.m
//
//  Created by ksyun.
//  Copyright © 2017 ksyun. All rights reserved.
//


#import "KSYNVCapture.h"

#define SCENE_COUNT     2

@interface KSYNVCapture()<NvsAssetPackageManagerDelegate>{
    NSString * m_captureSceneIdArray[SCENE_COUNT];
}
@end

@implementation KSYNVCapture

-(id)init{
    self = [super init];
    m_captureSceneIdArray[0] = @"897F4258-74C0-4F89-884E-3E6C07E3EE0E";
    m_captureSceneIdArray[1] = @"8FB5A4C7-BAFC-4FCD-9994-F496A78F47C3";
    
    // 初始化NvsStreamingContext
    _context = [NvsStreamingContext sharedInstance];
    if (!_context)
        return NULL;
    
    // 可用采集设备的数量
    if ([_context captureDeviceCount] == 0) {
        NSLog(@"没有可用于采集的设备");
        return NULL;
    }
    _context.assetPackageManager.delegate = self;
    
    // 由于本示例程序需要演示虚拟场景，所以需要给拍摄添加一个抠像特技
    NvsCaptureVideoFx *keyerFx = [_context appendBuiltinCaptureVideoFx:@"Master Keyer"];
    if (keyerFx) {
        // 开启溢色去除
        [keyerFx setBooleanVal:@"Spill Removal" val:YES];
        // 将溢色去除强度设置为最低
        [keyerFx setFloatVal:@"Spill Removal Intensity" val:0];
        // 设置收缩边界强度
        [keyerFx setFloatVal:@"Shrink Intensity" val:0.4];
    }
    
    _videoFrameReceiver = [[NvsVideoFrameReceiver alloc]initWithMode:NvsVideoFrameReceiverMode_CVPixelBuffer];
    // 将采集预览输出连接到NvsLiveWindow控件
    if (![_context connectCapturePreviewWithVideoFrameReceiver:_videoFrameReceiver]) {
        NSLog(@"连接预览窗口失败");
        return NULL;
    }
    
    return self;
}

- (NvsColor) pickColor:(CVPixelBufferRef) pixelBuffer selRect:(CGRect)selRect {
    size_t bufWidth = CVPixelBufferGetWidth(pixelBuffer);
    size_t bufHeight = CVPixelBufferGetHeight(pixelBuffer);
    
    int selX = selRect.origin.x;
    if (selX < 0)
        selX = 0;
    else if (selX >= bufWidth)
        selX = (int)bufWidth-1;
    int selY = selRect.origin.y;
    if (selY < 0)
        selY = 0;
    else if (selY >= bufHeight)
        selY = (int)bufHeight-1;
    int selWidth = selRect.size.width;
    if (selX + selWidth > bufWidth)
        selWidth = (int)bufWidth - selX;
    int selHeight = selRect.size.height;
    if (selY + selHeight > bufHeight)
        selHeight = (int)bufHeight - selY;
    
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    void *pxdata = CVPixelBufferGetBaseAddress(pixelBuffer);
    
    int b=0; int g=0; int r=0;
    for (int i=0; i<selHeight; i++) {
        for (int j=0; j<selWidth; j++) {
            b += ((uint8_t*)pxdata)[(selY+i)*bufWidth + (selX+j) * 4];
            g += ((uint8_t*)pxdata)[(selY+i)*bufWidth + (selX+j) * 4 + 1];
            r += ((uint8_t*)pxdata)[(selY+i)*bufWidth + (selX+j) * 4 + 2];
        }
    }
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    int pxCount = selWidth * selHeight;
    b /= pxCount;
    g /= pxCount;
    r /= pxCount;
    
    NvsColor keyColor;
    keyColor.r = (float)r/255.f;
    keyColor.g = (float)g/255.f;
    keyColor.b = (float)b/255.f;
    keyColor.a = 1;
    
    return keyColor;
}

- (void)setCaptureSceneColor:(NvsColor) color {
    // 将吸取下来的背景画面颜色值设置给抠像特技
    NvsCaptureVideoFx *keyerFx = [_context getCaptureVideoFxByIndex:0];
    if (keyerFx)
        [keyerFx setColorVal:@"Key Color" val:&color];
}

- (void)startNVCap:(unsigned int)cameraPosition{
    if (![_context startCapturePreview:cameraPosition videoResGrade:NvsVideoCaptureResolutionGradeHigh flags:NvsStreamingEngineCaptureFlag_GrabCapturedVideoFrame aspectRatio:nil]) {
        NSLog(@"启动视频采集失败");
    }
}

- (void)selectScene:(unsigned int)sceneIndex
{
    NSString *sceneId = m_captureSceneIdArray[sceneIndex];
    NSString *scenePackageFilePath = [self getScenePackageFilePath:sceneId];
    
    // 首先检查该拍摄场景的资源包是否已经安装
    NvsAssetPackageManager *assetPackageManager = _context.assetPackageManager;
    const NvsAssetPackageStatus packageStatus = [assetPackageManager getAssetPackageStatus:sceneId
                                                                                      type:NvsAssetPackageType_CaptureScene];
    if (packageStatus == NvsAssetPackageStatus_NotInstalled) {
        // 该拍摄场景的资源包尚未安装，现在安装该资源包。由于拍摄场景的资源包尺寸较大
        // 为了不freeze UI，我们采用异步安装模式
        [assetPackageManager installAssetPackage:scenePackageFilePath
                                         license:nil
                                            type:NvsAssetPackageType_CaptureScene
                                            sync:NO
                                  assetPackageId:nil];
        return;
    }
    
    // 该拍摄场景的资源包已经安装，应用其效果
    [_context applyCaptureScene:sceneId];
}

- (NSString *)getScenePackageFilePath:(NSString *)sceneId
{
    NSString *packageFileName = [sceneId stringByAppendingString:@".capturescene"];
    NSString *appPath =[[NSBundle mainBundle] bundlePath];
    return [appPath stringByAppendingPathComponent:packageFileName];
}

- (void)didFinishAssetPackageInstallation:(NSString *)assetPackageId
                                 filePath:(NSString *)assetPackageFilePath
                                     type:(NvsAssetPackageType)assetPackageType
                                    error:(NvsAssetPackageManagerError)error
{
    if (error == NvsAssetPackageManagerError_NoError) {
        for (int i = 0; i < SCENE_COUNT; ++i) {
            if ([m_captureSceneIdArray[i] isEqualToString:assetPackageId]) {
                [self selectScene:i];
                break;
            }
        }
    }
}

- (void) closeNvCap{
    _context.assetPackageManager.delegate = nil;
    [_context  removeCurrentCaptureScene];
    NvsColor color = {0, 0, 0 ,0};
    [self setCaptureSceneColor:color];
    [_context stop];
    _context = nil;
}

@end
