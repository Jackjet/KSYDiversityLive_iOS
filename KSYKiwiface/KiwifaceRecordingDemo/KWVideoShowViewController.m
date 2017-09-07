//
//  KWVideoShowViewController.m
//  KiwifaceRecordingDemo
//
//  Created by zhaoyichao on 2017/2/4.
//  Copyright © 2017年 kiwiFaceSDK. All rights reserved.
//

#import "KWVideoShowViewController.h"
#import <AVFoundation/AVFoundation.h>
#import <AssetsLibrary/ALAssetsLibrary.h>
#import <Foundation/Foundation.h>
#import "FaceTracker.h"
#import "KWSDK.h"

#import "Global.h"
#import <GPUImage/GPUImage.h>
#import "KSYKiwiGPUStreamerKit.h"

#define weakObj(o) __weak typeof(o) o##Weak = o;

@interface KWVideoShowViewController ()<GPUImageVideoCameraDelegate,KWSDKUIDelegate>

@property (nonatomic, strong) GPUImageStillCamera *videoCamera;
@property (nonatomic, strong) GPUImageMovieWriter *movieWriter;
@property (nonatomic, strong) NSURL *movieURL;
@property (nonatomic, strong) NSURL *exportURL;
@property (nonatomic, strong) CALayer *watermarkLayer;
@property (nonatomic, strong) GPUImageFilter *emptyFilter;

@property (nonatomic, strong) CIImage *outputImage;
@property (nonatomic, assign) size_t outputWidth;
@property (nonatomic, assign) size_t outputheight;

@property (nonatomic, strong) UILabel *labRecordState;
@property (nonatomic, strong) dispatch_queue_t sessionQueue;

// 推流地址 完整的URL
@property (nonatomic, strong) UILabel *streamState;
@property (nonatomic, retain) KSYKiwiGPUStreamerKit *kit;
@property NSURL * hostURL;

@end

@implementation KWVideoShowViewController
{
    __block KWVideoShowViewController *__blockSelf;
    __weak KWVideoShowViewController *__weakSelf;
    dispatch_queue_t queue; //视频录制队列
    AVURLAsset *asset;
    
    UIButton *btnRecord;//临时申请的按钮控件指针 控制是否可用。
}

- (void)initKSYStreamerSettings
{
    //streamkit initialized
    _kit = [[KSYKiwiGPUStreamerKit alloc] initWithDefaultCfg];
    _kit.videoOrientation = [[UIApplication sharedApplication] statusBarOrientation];
    _kit.captureDimension = CGSizeMake(640, 480);
    _kit.previewDimension = [self.cfgview capResolutionSize];
    _kit.streamDimension  = [self.cfgview strResolutionSize];
    _kit.gpuOutputPixelFormat = kCVPixelFormatType_32BGRA;
    _kit.capturePixelFormat   = kCVPixelFormatType_32BGRA;
    
    [self setStreamCfg];
    NSString *version = [_kit.streamerBase getKSYVersion];
    NSLog(@"ksyun sdk version: %@", version);
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(streamStateChanged) name:KSYStreamStateDidChangeNotification object:nil];
}

- (void)setStreamCfg{
    if (_cfgview) {
        _kit.streamerBase.videoCodec       = [_cfgview videoCodec];
        _kit.streamerBase.videoInitBitrate = [_cfgview videoKbps]*6/10;//60%
        _kit.streamerBase.videoMaxBitrate  = [_cfgview videoKbps];
        _kit.streamerBase.videoMinBitrate  = 0; //
        _kit.streamerBase.audioCodec       = [_cfgview audioCodec];
        _kit.streamerBase.audiokBPS        = [_cfgview audioKbps];
        _kit.streamerBase.videoFPS         = [_cfgview frameRate];
        _kit.streamerBase.bwEstimateMode   = [_cfgview bwEstMode];
        _kit.streamerBase.logBlock = ^(NSString* str){
            //NSLog(@"%@", str);
        };
        _hostURL = [NSURL URLWithString:[_cfgview hostUrl]];
        NSLog(@"streamer url:%@", _hostURL);
    }
}

- (void)streamStateChanged{
    NSLog(@"stream State %@", [_kit.streamerBase getCurStreamStateName]);
    switch (_kit.streamerBase.streamState) {
        case KSYStreamStateIdle:
            _streamState.text = @"空闲状态";
            break;
        case KSYStreamStateConnecting:
            _streamState.text = @"连接中";
            break;
        case KSYStreamStateConnected:
            _streamState.text = @"已连接";
            break;
        case KSYStreamStateDisconnecting:
            _streamState.text = @"失去连接";
            break;
        case KSYStreamStateError:
            _streamState.text = @"连接错误";
            break;
        default:
            break;
    }
}


- (void)viewDidLoad {
    [super viewDidLoad];
    
    //设置视频录制文件导出路径
    [self commonInit];
    
    self.navigationController.navigationBarHidden = YES;
    
    [[UIApplication sharedApplication]setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];
    
    
    self.kwSdkUI = [KWSDK_UI shareManagerUI];
    self.kwSdkUI.delegate = self;
    self.kwSdkUI.kwSdk = [KWSDK sharedManager];
    
    self.kwSdkUI.kwSdk.renderer = [[KWRenderer alloc]initWithModelPath:self.modelPath];

    self.kwSdkUI.kwSdk.cameraPositionBack  = NO;
    if([KWRenderer isSdkInitFailed]){
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"错误提示" message:@"使用 license 文件生成激活码时失败，可能是授权文件过期。" delegate:nil cancelButtonTitle:@"好的" otherButtonTitles:nil, nil];
        
        [alert show];
        return;
    }
    
    [self.kwSdkUI setViewDelegate:self];
    [self.kwSdkUI.kwSdk initSdk];
    
    [self initKSYStreamerSettings];

    self.videoCamera.horizontallyMirrorFrontFacingCamera = YES;
    self.videoCamera.horizontallyMirrorRearFacingCamera = NO;
    self.kwSdkUI.isClearOldUI = YES;
    
    /******** 录制功能 *********************/
    self.kwSdkUI.kwSdk.videoCamera = self.videoCamera;
    self.kwSdkUI.previewView = _kit.preview;
    self.kwSdkUI.kwSdk.movieWriter = self.movieWriter;
    
    [self.kwSdkUI initSDKUI];
    
    [self.kwSdkUI setCloseVideoBtnHidden:NO];
    
    __blockSelf = self;
    __weakSelf = __blockSelf;
    
    __weak KWSDK_UI *__weakSdkUI = self.kwSdkUI;
    __weakSdkUI.toggleBtnBlock = ^()
    {
        /* 切换摄像头 */
        __weakSelf.kwSdkUI.kwSdk.cameraPositionBack  = !__weakSelf.kwSdkUI.kwSdk.cameraPositionBack;
        [__weakSelf.videoCamera rotateCamera];
    };
    
    self.kwSdkUI.closeVideoBtnBlock = ^(void)
    {
        [__weakSelf dismissViewControllerAnimated:YES completion:^{
            [[NSNotificationCenter defaultCenter] removeObserver:__weakSelf name:kReachabilityChangedNotification object:nil];
            if (__weakSelf.sessionQueue) {
                dispatch_sync(__weakSelf.sessionQueue, ^{
                });
            }
            
            __weakSelf.sessionQueue = nil;
            
            [__weakSelf.kwSdkUI popAllView];
            
            
            [__weakSdkUI.kwSdk.videoCamera stopCameraCapture];
            /* 内存释放 */
            [KWSDK_UI releaseManager];
            [KWSDK releaseManager];
            
            [_kit stopPreview];
            
        }];
    };
    
    __weakSdkUI.streamerBtnBlock = ^(void)
    {
        /* 推流 */
        [__weakSelf onStream];
    };
    
    //拍照
//    self.kwSdkUI.takePhotoBtnTapBlock = ^(UIButton *sender)
//    {
//        [__weakSelf takePhoto:sender];
//    };
    
//    __weakSdkUI.offPhoneBlock = ^(UIButton *sender)
//    {
//        //录制视频
//        [__weakSelf recordVideo:sender];
//        
//    };

    [self.view addSubview:self.streamState];

    [self.view addSubview:self.labRecordState];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillResignActive:)
                                                 name:UIApplicationWillResignActiveNotification object:nil]; //监听是否触发home键挂起程序.
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationDidBecomeActive:)
                                                 name:UIApplicationDidBecomeActiveNotification object:nil]; //监听是否重新进入程序程序.
    //启动金山音频采集
    [_kit startAudioCap];
}

//拍照
- (void)takePhoto
{
    if (self.outputImage) {
        
        /* 录制demo 前置摄像头修正图片朝向*/
        UIImage *processedImage = [self image:[self convertBufferToImage] rotation:UIImageOrientationRight];
        UIImageWriteToSavedPhotosAlbum(processedImage, self, @selector(image:finishedSavingWithError:contextInfo:), nil);
    }
}

- (void) onStream{
    if (_kit.streamerBase.streamState == KSYStreamStateIdle ||
        _kit.streamerBase.streamState == KSYStreamStateError) {
        [_kit.streamerBase startStream:self.hostURL];
    }
    else {
        [_kit.streamerBase stopStream];
    }
}

- (void)startRecording
{
    
    [self.labRecordState setText:@"正在录制"];
    [self.labRecordState setHidden:NO];
    if (self.kwSdkUI.kwSdk.currentStickerIndex >= 0) {
        [self.kwSdkUI.kwSdk.filters.lastObject addTarget:self.movieWriter];
    } else if (self.kwSdkUI.kwSdk.currentDistortionFilter) {
        [self.kwSdkUI.kwSdk.currentDistortionFilter addTarget:self.movieWriter];
    } else if (self.kwSdkUI.kwSdk.currentColorFilter) {
        [self.kwSdkUI.kwSdk.currentColorFilter addTarget:self.movieWriter];
        
    }
    
    self.videoCamera.audioEncodingTarget = self.movieWriter;
    
    [[NSFileManager defaultManager] removeItemAtURL:self.movieURL error:nil];
    
    [self.videoCamera startCameraCapture];
    
    [Global sharedManager].PIXCELBUFFER_ROTATE = KW_PIXELBUFFER_ROTATE_0;
    
    [self.kwSdkUI.kwSdk resetDistortionParams];
    
    [self.movieWriter startRecording];
}


- (void)endRecording
{
    [self.labRecordState setText:@"正在保存视频..."];
    [self.kwSdkUI setCloseBtnEnable:NO];
    if (self.kwSdkUI.kwSdk.currentStickerIndex >= 0) {
        [self.kwSdkUI.kwSdk.filters.lastObject removeTarget:self.movieWriter];
    }
    if (self.kwSdkUI.kwSdk.currentDistortionFilter) {
        [self.kwSdkUI.kwSdk.currentDistortionFilter removeTarget:self.movieWriter];
    }
    if (self.kwSdkUI.kwSdk.currentColorFilter) {
        [self.kwSdkUI.kwSdk.currentColorFilter removeTarget:self.movieWriter];
    }
    
    self.videoCamera.audioEncodingTarget = nil;
    
    [self.movieWriter finishRecordingWithCompletionHandler:^{
        [self addWatermarkToVideo];
    }];
}

-(void)didClickOffPhoneButton
{
    if(_kit.streamerBase.streamState == KSYStreamStateConnected){
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"错误提示" message:@"请断开直播再操作" delegate:nil cancelButtonTitle:@"好的" otherButtonTitles:nil, nil];
        
        [alert show];
        return;
    }
    [self takePhoto];
}

-(void)didBeginLongPressOffPhoneButton
{
    if(_kit.streamerBase.streamState == KSYStreamStateConnected){
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"错误提示" message:@"请断开直播再操作" delegate:nil cancelButtonTitle:@"好的" otherButtonTitles:nil, nil];
        
        [alert show];
        return;
    }
    [self startRecording];
}

-(void)didEndLongPressOffPhoneButton
{
    if(_kit.streamerBase.streamState == KSYStreamStateConnected){
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"错误提示" message:@"请断开直播再操作" delegate:nil cancelButtonTitle:@"好的" otherButtonTitles:nil, nil];
        
        [alert show];
        return;
    }
    [self endRecording];
}


- (void)applicationWillResignActive:(NSNotification *)noti
{
    if (btnRecord.isSelected) {
        [self.movieWriter setPaused:YES];
        [self.videoCamera stopCameraCapture];
    }
    
}

- (void)applicationDidBecomeActive:(NSNotification *)noti
{
    if (btnRecord.isSelected) {
        [self.movieWriter setPaused:NO];
        [self.videoCamera startCameraCapture];
    }
}

#pragma mark < 录制视频 code

//加载拍照,视频录制摄像头工具类
-(GPUImageStillCamera *)videoCamera
{
    if (!_videoCamera) {
        _videoCamera = [[GPUImageStillCamera alloc] initWithSessionPreset:AVCaptureSessionPreset640x480
                                                           cameraPosition:AVCaptureDevicePositionFront];
        _videoCamera.frameRate = 25;
        _videoCamera.outputImageOrientation = UIDeviceOrientationPortrait;
        _videoCamera.horizontallyMirrorFrontFacingCamera = YES;
        _videoCamera.horizontallyMirrorRearFacingCamera = NO;
 
        //[_videoCamera addAudioInputsAndOutputs];
        _videoCamera.delegate = self;
        

        [_videoCamera startCameraCapture];

        [_videoCamera addTarget:self.movieWriter];
    }
    return _videoCamera;
}

- (GPUImageMovieWriter *)movieWriter
{
    if (!_movieWriter) {
        NSDictionary * videoSettings = @{ AVVideoCodecKey : AVVideoCodecH264,
                                          AVVideoWidthKey : @(480),
                                          AVVideoHeightKey : @(640),
                                          AVVideoScalingModeKey : AVVideoScalingModeResizeAspectFill};
        _movieWriter = [[GPUImageMovieWriter alloc] initWithMovieURL:self.movieURL size:CGSizeMake(480.0, 640.0) fileType:AVFileTypeQuickTimeMovie outputSettings:videoSettings];
    }
    return _movieWriter;
}

- (GPUImageFilter *)emptyFilter
{
    if (!_emptyFilter) {
        _emptyFilter = [[GPUImageFilter alloc]init];
    }
    return _emptyFilter;
}

//设置视频录制文件导出路径
- (void)commonInit
{
    NSString *fileName = [[NSUUID UUID] UUIDString];
    NSString *pathToMovie = [NSTemporaryDirectory() stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.MOV", fileName]];
    self.movieURL = [NSURL fileURLWithPath:pathToMovie];
    
    NSString *pathToExport = [NSTemporaryDirectory() stringByAppendingPathComponent:[NSString stringWithFormat:@"%@_export.MOV", fileName]];
    self.exportURL = [NSURL fileURLWithPath:pathToExport];
}

- (void)addWatermarkToVideo
{
    asset = [AVURLAsset URLAssetWithURL:self.movieURL options:nil];
    
    /*****************    裁剪视频头0.1秒 解决第一帧偶现黑屏问题    ************************/
    // 创建可变的音视频组合
    AVMutableComposition *comosition = [AVMutableComposition composition];
    //分离视频的音轨和视频轨
    AVMutableCompositionTrack *videoTrack = [comosition addMutableTrackWithMediaType:AVMediaTypeVideo preferredTrackID:kCMPersistentTrackID_Invalid];
    AVMutableCompositionTrack *audioTrack = [comosition addMutableTrackWithMediaType:AVMediaTypeAudio preferredTrackID:kCMPersistentTrackID_Invalid];
    
    //视频原时长
    CMTime duration = asset.duration;
    //要截取的时间
    CMTime clipTime = CMTimeMakeWithSeconds(0.1, duration.timescale);
    //截取后的视频时长
    CMTime clipDurationTime = CMTimeSubtract(duration, clipTime);
    //截取后的视频时间范围
    CMTimeRange videoTimeRange = CMTimeRangeMake(clipTime, clipDurationTime);
    
    //视频采集通道
    AVAssetTrack *videoAssetTrack = [[asset tracksWithMediaType:AVMediaTypeVideo] firstObject];
    //把采集的轨道数据加入到视频轨道之中
    [videoTrack insertTimeRange:videoTimeRange ofTrack:videoAssetTrack atTime:kCMTimeZero error:nil];
    
    //音频采集通道
    AVAssetTrack *audioAssetTrack = [[asset tracksWithMediaType:AVMediaTypeAudio]firstObject];
    //把采集的轨道数据加入到音频轨道之中
    [audioTrack insertTimeRange:videoTimeRange ofTrack:audioAssetTrack atTime:kCMTimeZero error:nil];
    
    /************************************************************************************************/
    
    AVMutableVideoCompositionLayerInstruction *passThroughLayer = [AVMutableVideoCompositionLayerInstruction videoCompositionLayerInstructionWithAssetTrack:videoAssetTrack];
    
    AVMutableVideoCompositionInstruction *passThroughInstruction = [AVMutableVideoCompositionInstruction videoCompositionInstruction];
    
    
    passThroughInstruction.timeRange = CMTimeRangeMake(kCMTimeZero, comosition.duration);
    //     passThroughInstruction.timeRange = videoTimeRange;
    
    passThroughInstruction.layerInstructions = @[ passThroughLayer ];
    
    AVMutableVideoComposition *videoComposition = [AVMutableVideoComposition videoComposition];
    videoComposition.frameDuration = CMTimeMake(1, self.videoCamera.frameRate);
    videoComposition.renderSize = videoAssetTrack.naturalSize;
    videoComposition.instructions = @[ passThroughInstruction ];
    
    
    // watermark
    CGSize renderSize = videoComposition.renderSize;
    //CGFloat ratio = MIN(renderSize.width / CGRectGetWidth(self.previewView.frame), renderSize.height / CGRectGetHeight(self.previewView.frame));
    CGFloat ratio = MIN(renderSize.width / CGRectGetWidth(_kit.preview.frame), renderSize.height / CGRectGetHeight(_kit.preview.frame));
    CGFloat watermarkWidth = ceil(renderSize.width / 5.);
    CGFloat watermarkHeight = ceil(watermarkWidth * CGRectGetHeight(self.watermarkLayer.frame) / CGRectGetWidth(self.watermarkLayer.frame));
    //
    CALayer *exportWatermarkLayer = [CALayer layer];
    exportWatermarkLayer.contents = self.watermarkLayer.contents;
    exportWatermarkLayer.frame = CGRectMake(renderSize.width - watermarkWidth - ceil(ratio * 16), ceil(ratio * 16), watermarkWidth, watermarkHeight);
    
    CALayer *parentLayer = [CALayer layer];
    CALayer *videoLayer = [CALayer layer];
    parentLayer.frame = CGRectMake(0, 0, renderSize.width, renderSize.height);
    videoLayer.frame = CGRectMake(0, 0, renderSize.width, renderSize.height);
    [parentLayer addSublayer:videoLayer];
    [parentLayer addSublayer:exportWatermarkLayer];
    videoComposition.animationTool = [AVVideoCompositionCoreAnimationTool videoCompositionCoreAnimationToolWithPostProcessingAsVideoLayer:videoLayer inLayer:parentLayer];
    
    // export
    AVAssetExportSession *exportSession = [[AVAssetExportSession alloc] initWithAsset:[comosition copy] presetName:AVAssetExportPresetHighestQuality];
    exportSession.videoComposition = videoComposition;
    exportSession.shouldOptimizeForNetworkUse = NO;
    exportSession.outputURL = self.exportURL;
    exportSession.outputFileType = AVFileTypeQuickTimeMovie;
    
    [[NSFileManager defaultManager] removeItemAtURL:self.exportURL error:nil];
    
    
    __weak AVAssetExportSession *weakExportSession = exportSession;
    [exportSession exportAsynchronouslyWithCompletionHandler:^(void){
        if (weakExportSession.status == AVAssetExportSessionStatusCompleted) {
            [[NSFileManager defaultManager] removeItemAtURL:self.movieURL error:nil];
            [self saveVideoToAssetsLibrary];
        } else {
            
            
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error" message:@"Video Saving Failed"
                                                           delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
            [alert show];
            
            [self.kwSdkUI setCloseBtnEnable:YES];
            [btnRecord setEnabled:YES];
            [self.labRecordState setHidden:YES];
            
            [self.videoCamera removeTarget:_movieWriter];
            _movieWriter = nil;
            [self.videoCamera addTarget:self.movieWriter];
        }
    }];
}

- (CALayer *)watermarkLayer
{
    if (!_watermarkLayer) {
        UIImage *watermark = [UIImage imageNamed:@"watermark"];
        _watermarkLayer = [CALayer layer];
        _watermarkLayer.contents = (id)watermark.CGImage;
        _watermarkLayer.frame = CGRectMake(0, 0, watermark.size.width, watermark.size.height);
    }
    
    return _watermarkLayer;
}

- (void)saveVideoToAssetsLibrary
{
    ALAssetsLibrary *library = [[ALAssetsLibrary alloc] init];
    
    if ([library videoAtPathIsCompatibleWithSavedPhotosAlbum:self.exportURL])
    {
        [library writeVideoAtPathToSavedPhotosAlbum:self.exportURL completionBlock:^(NSURL *assetURL, NSError *error)
         {
             //             self.recordButton.enabled = YES;
             [[NSFileManager defaultManager] removeItemAtURL:self.exportURL error:nil];
             
             dispatch_sync(dispatch_get_main_queue(), ^{
                 if (error) {
                     UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error" message:@"Video Saving Failed"
                                                                    delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
                     [alert show];
                     
                     
                 } else {
                     UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Video Saved" message:@"Saved To Photo Album"
                                                                    delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
                     [alert show];
                 }
                 [self.kwSdkUI setCloseBtnEnable:YES];
                 [btnRecord setEnabled:YES];
                 [self.labRecordState setHidden:YES];
                 [self.videoCamera removeTarget:self.movieWriter];
                 _movieWriter = nil;
                 [self.videoCamera addTarget:self.movieWriter];
             });
         }];
    } else {
        //        self.recordButton.enabled = YES;
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error" message:@"Video Cannot Be Saved"
                                                       delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [alert show];
        
        [self.kwSdkUI setCloseBtnEnable:YES];
        [btnRecord setEnabled:YES];
        [self.labRecordState setHidden:YES];
        
        [self.videoCamera removeTarget:_movieWriter];
        _movieWriter = nil;
        [self.videoCamera addTarget:self.movieWriter];
    }
}

- (void)recordVideo:(UIButton *)sender
{
    btnRecord = sender;
    if (sender.selected) {
        // recording
        sender.selected = NO;
        sender.enabled = NO;
        [self.labRecordState setText:@"正在保存视频..."];
        [self.kwSdkUI setCloseBtnEnable:NO];
        if (self.kwSdkUI.kwSdk.currentStickerIndex >= 0) {
            [self.kwSdkUI.kwSdk.filters.lastObject removeTarget:self.movieWriter];
        }
        if (self.kwSdkUI.kwSdk.currentDistortionFilter) {
            [self.kwSdkUI.kwSdk.currentDistortionFilter removeTarget:self.movieWriter];
        }
        if (self.kwSdkUI.kwSdk.currentColorFilter) {
            [self.kwSdkUI.kwSdk.currentColorFilter removeTarget:self.movieWriter];
        }
        
        self.videoCamera.audioEncodingTarget = nil;
        
        [self.movieWriter finishRecordingWithCompletionHandler:^{
            [self addWatermarkToVideo];
        }];
        
        
    } else {
        [self.labRecordState setText:@"正在录制"];
        [self.labRecordState setHidden:NO];
        if (self.kwSdkUI.kwSdk.currentStickerIndex >= 0) {
            [self.kwSdkUI.kwSdk.filters.lastObject addTarget:self.movieWriter];
        } else if (self.kwSdkUI.kwSdk.currentDistortionFilter) {
            [self.kwSdkUI.kwSdk.currentDistortionFilter addTarget:self.movieWriter];
        } else if (self.kwSdkUI.kwSdk.currentColorFilter) {
            [self.kwSdkUI.kwSdk.currentColorFilter addTarget:self.movieWriter];
        }
        
        self.videoCamera.audioEncodingTarget = self.movieWriter;

        [[NSFileManager defaultManager] removeItemAtURL:self.movieURL error:nil];
        
        sender.selected = YES;
        
        [self.videoCamera startCameraCapture];
        
        [Global sharedManager].PIXCELBUFFER_ROTATE = KW_PIXELBUFFER_ROTATE_0;
        
        [self.kwSdkUI.kwSdk resetDistortionParams];
        
        [self.movieWriter startRecording];
        
    }
}
#pragma mark > 录制视频 End


- (UILabel *)labRecordState
{
    if (!_labRecordState) {
        _labRecordState = [[UILabel alloc]initWithFrame:CGRectMake(0, 400, ScreenWidth_KW, 50)];
        [_labRecordState setHidden:YES];
        [_labRecordState setText:@"正在录制"];
        [_labRecordState setFont:[UIFont systemFontOfSize:25.f]];
        [_labRecordState setTextAlignment:NSTextAlignmentCenter];
        [_labRecordState setTextColor:[UIColor whiteColor]];
    }
    return _labRecordState;
}

- (UILabel *)streamState
{
    if (!_streamState) {
        _streamState = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, ScreenWidth_KW, 50)];
        [_streamState setText:@""];
        [_streamState setFont:[UIFont systemFontOfSize:14.f]];
        [_streamState setTextAlignment:NSTextAlignmentCenter];
        [_streamState setTextColor:[UIColor redColor]];
    }
    return _streamState;
}

//拍照
- (void)takePhoto:(UIButton *)sender
{
    [sender setEnabled:NO];
    if (self.outputImage) {
        
        /* 录制demo修正图片朝向*/
        UIImage *processedImage = [self image:[self convertBufferToImage] rotation:UIImageOrientationRight];
        UIImageWriteToSavedPhotosAlbum(processedImage, self, @selector(image:finishedSavingWithError:contextInfo:), nil);
        [sender setEnabled:YES];
    }
}

- (UIImage *)convertBufferToImage
{
    CIContext *temporaryContext = [CIContext contextWithOptions:nil];
    CGImageRef videoImage = [temporaryContext
                             createCGImage:self.outputImage
                             fromRect:CGRectMake(0, 0,
                                                 self.outputWidth,
                                                 self.outputheight)];
    
    UIImage *uiImage = [UIImage imageWithCGImage:videoImage];
    CGImageRelease(videoImage);
    return uiImage;
}


- (void)image:(UIImage *)image finishedSavingWithError:(NSError *)error contextInfo: (void *) contextInfo
{
    UIAlertController *alertView = [[UIAlertController alloc]init];
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"好的" style:UIAlertActionStyleCancel handler:nil];
    [alertView addAction:cancelAction];
    [alertView setTitle:@"提示"];
    
    if (error) {
        [alertView setMessage:[NSString stringWithFormat:@"拍照失败，原因：%@",error]];
        
        NSLog(@"save failed.");
    } else {
        [alertView setMessage:[NSString stringWithFormat:@"拍照成功！相片已保存到相册！"]];
        NSLog(@"save success.");
    }
    [self presentViewController:alertView animated:NO completion:nil];
}

static void KSYARGBRotate(uint8_t* src_argb, uint8_t* dst_argb, int width, int height,
               cv_rotate_type mode, BOOL mirrored) {
    int i, j;
    if (mirrored){
        for (j = 0; j < height; j++){
            for (i = 0; i < width; i++){
                dst_argb[i*4*height+4*j] = src_argb[j*4*width+4*i];
                dst_argb[i*4*height+4*j+1] = src_argb[j*4*width+4*i+1];
                dst_argb[i*4*height+4*j+2] = src_argb[j*4*width+4*i+2];
                dst_argb[i*4*height+4*j+3] = src_argb[j*4*width+4*i+3];
            }
        }
    }else{
        for (j = 0; j < height; j++){
            for (i = 0; i < width; i++){
                dst_argb[i*4*height+4*(height-1-j)] = src_argb[j*4*width+4*i];
                dst_argb[i*4*height+4*(height-1-j)+1] = src_argb[j*4*width+4*i+1];
                dst_argb[i*4*height+4*(height-1-j)+2] = src_argb[j*4*width+4*i+2];
                dst_argb[i*4*height+4*(height-1-j)+3] = src_argb[j*4*width+4*i+3];
            }
        }
    }
}

- (void)willOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
{
    
    CVPixelBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);

    UIDeviceOrientation iDeviceOrientation = [[UIDevice currentDevice] orientation];
    BOOL mirrored;

    mirrored = !self.kwSdkUI.kwSdk.cameraPositionBack && self.videoCamera.horizontallyMirrorFrontFacingCamera;

    cv_rotate_type cvMobileRotate;
    UIInterfaceOrientation orie;
    
    switch (iDeviceOrientation) {
        case UIDeviceOrientationPortrait:
            cvMobileRotate = CV_CLOCKWISE_ROTATE_90;
            [Global sharedManager].PIXCELBUFFER_ROTATE = KW_PIXELBUFFER_ROTATE_0;
            orie = UIInterfaceOrientationPortrait;
            break;
            
        case UIDeviceOrientationLandscapeLeft:
            cvMobileRotate = mirrored ? CV_CLOCKWISE_ROTATE_180 : CV_CLOCKWISE_ROTATE_0;
            [Global sharedManager].PIXCELBUFFER_ROTATE = KW_PIXELBUFFER_ROTATE_270;
            orie = UIInterfaceOrientationLandscapeRight;
            break;
            
        case UIDeviceOrientationLandscapeRight:
            cvMobileRotate = mirrored ? CV_CLOCKWISE_ROTATE_0 : CV_CLOCKWISE_ROTATE_180;
            [Global sharedManager].PIXCELBUFFER_ROTATE = KW_PIXELBUFFER_ROTATE_90;
            orie = UIInterfaceOrientationLandscapeLeft;
            break;
            
        case UIDeviceOrientationPortraitUpsideDown:
            cvMobileRotate = CV_CLOCKWISE_ROTATE_270;
            [Global sharedManager].PIXCELBUFFER_ROTATE = KW_PIXELBUFFER_ROTATE_180;
            orie = UIInterfaceOrientationPortraitUpsideDown;
            break;
            
        default:
            cvMobileRotate = CV_CLOCKWISE_ROTATE_0;
            orie = UIInterfaceOrientationPortrait;
            break;
    }
    
    
    [self.kwSdkUI.kwSdk.renderer processPixelBuffer:pixelBuffer withRotation:cvMobileRotate mirrored:mirrored];
    
    /*********** 如果有拍照功能则必须加上 ***********/
    self.outputImage =  [CIImage imageWithCVPixelBuffer:pixelBuffer];
    self.outputWidth = CVPixelBufferGetWidth(pixelBuffer);
    self.outputheight = CVPixelBufferGetHeight(pixelBuffer);
    
    //是否需要旋转
    [_kit rotateStreamTo:orie];
    
    //推流
    NSDictionary* pixelBufferOptions = @{
                                         (NSString*) kCVPixelBufferWidthKey : @(self.outputheight),
                                         (NSString*) kCVPixelBufferHeightKey : @(self.outputWidth),
                                         (NSString*) kCVPixelBufferOpenGLESCompatibilityKey : @YES,
                                         (NSString*) kCVPixelBufferIOSurfacePropertiesKey : @{}};
    CVPixelBufferRef streamerBuffer = NULL;
    CVReturn ret = CVPixelBufferCreate(kCFAllocatorDefault, self.outputheight, self.outputWidth, kCVPixelFormatType_32BGRA, (__bridge CFDictionaryRef)pixelBufferOptions, &streamerBuffer);
    if (ret!= noErr) {
        NSLog(@"创建streamer buffer失败");
        streamerBuffer = NULL;
        return;
    }
    
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    CVPixelBufferLockBaseAddress(streamerBuffer, 0);
    uint8_t *src_frame      = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    uint8_t *dst_frame      = CVPixelBufferGetBaseAddressOfPlane(streamerBuffer, 0);
    int wdt = (int)CVPixelBufferGetWidth(pixelBuffer);
    int hgt = (int)CVPixelBufferGetHeight(pixelBuffer);
    KSYARGBRotate(src_frame, dst_frame, wdt, hgt, cvMobileRotate, mirrored);
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    CVPixelBufferUnlockBaseAddress(streamerBuffer, 0);
    CMTime pts = CMSampleBufferGetPresentationTimeStamp(sampleBuffer);
    [_kit.capToGpu processPixelBuffer:streamerBuffer time:pts];
    
    CFRelease(streamerBuffer);
    /*********** End ***********/
}

- (UIImage *)image:(UIImage *)image rotation:(UIImageOrientation)orientation
{
    long double rotate = 0.0;
    CGRect rect;
    float translateX = 0;
    float translateY = 0;
    float scaleX = 1.0;
    float scaleY = 1.0;
    
    switch (orientation) {
        case UIImageOrientationLeft:
            rotate = M_PI_2;
            rect = CGRectMake(0, 0, image.size.height, image.size.width);
            translateX = 0;
            translateY = -rect.size.width;
            scaleY = rect.size.width/rect.size.height;
            scaleX = rect.size.height/rect.size.width;
            break;
        case UIImageOrientationRight:
            rotate = 3 * M_PI_2;
            rect = CGRectMake(0, 0, image.size.height, image.size.width);
            translateX = -rect.size.height;
            translateY = 0;
            scaleY = rect.size.width/rect.size.height;
            scaleX = rect.size.height/rect.size.width;
            break;
        case UIImageOrientationDown:
            rotate = M_PI;
            rect = CGRectMake(0, 0, image.size.width, image.size.height);
            translateX = -rect.size.width;
            translateY = -rect.size.height;
            break;
        default:
            rotate = 0.0;
            rect = CGRectMake(0, 0, image.size.width, image.size.height);
            translateX = 0;
            translateY = 0;
            break;
    }
    
    UIGraphicsBeginImageContext(rect.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    //做CTM变换
    CGContextTranslateCTM(context, 0.0, rect.size.height);
    CGContextScaleCTM(context, 1.0, -1.0);
    CGContextRotateCTM(context, rotate);
    CGContextTranslateCTM(context, translateX, translateY);
    
    CGContextScaleCTM(context, scaleX, scaleY);
    //绘制图片
    CGContextDrawImage(context, CGRectMake(0, 0, rect.size.width, rect.size.height), image.CGImage);
    
    UIImage *newPic = UIGraphicsGetImageFromCurrentImageContext();
    
    if (!self.kwSdkUI.kwSdk.cameraPositionBack) {
        newPic = [self convertMirrorImage:newPic];
    }
    
    
    return newPic;
}

- (UIImage *)convertMirrorImage:(UIImage *)image
{
    
    //Quartz重绘图片
    CGRect rect =  CGRectMake(0, 0, image.size.width , image.size.height);//创建矩形框
    //根据size大小创建一个基于位图的图形上下文
    UIGraphicsBeginImageContextWithOptions(rect.size, false, 2);
    CGContextRef currentContext =  UIGraphicsGetCurrentContext();//获取当前quartz 2d绘图环境
    CGContextClipToRect(currentContext, rect);//设置当前绘图环境到矩形框
    CGContextRotateCTM(currentContext, (CGFloat)M_PI); //旋转180度
    //平移， 这里是平移坐标系，跟平移图形是一个道理
    CGContextTranslateCTM(currentContext, -rect.size.width, -rect.size.height);
    CGContextDrawImage(currentContext, rect, image.CGImage);//绘图
    
    //翻转图片
    UIImage *drawImage =  UIGraphicsGetImageFromCurrentImageContext();//获得图片
    UIImage *flipImage =  [[UIImage alloc]initWithCGImage:drawImage.CGImage];
    
    
    return flipImage;
}

- (void)dealloc
{
    btnRecord = nil;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

@end
