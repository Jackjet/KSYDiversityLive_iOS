//
//  LiveStreamingViewController.m
//  TuSDKLiveEngineDemo
//
//  Created by TuSDK on 2017/3/20.
//  Copyright © 2017年 TuSDK. All rights reserved.
//

#import "LiveStreamingViewController.h"
#import "FilterView.h"
#import "StickerScrollView.h"
#import "KSYTuGPUStreamerKit.h"

@interface LiveStreamingViewController ()<FilterViewEventDelegate,StickerViewClickDelegate,TuSDKLiveVideoCameraDelegate>
{
    //当前获取的滤镜对象；
    TuSDKFilterWrap *_currentFilter;
    // 当前闪光灯模式
    AVCaptureFlashMode _flashMode;
    //滤镜栏；
    FilterView *_filterView;
    //贴纸栏；
    StickerScrollView *_stickerView;
    // 直播滤镜列表
    NSArray *_videoFilters;
    // 当前的滤镜索引
    NSInteger _videoFilterIndex;
    BOOL uiInited;
    KSYTuGPUStreamerKit *_kit;//推流kit
}

/**
 *  推流状态
 */
@property (nonatomic, strong) UILabel *streamState;

/**
 时间戳
 */
@property (nonatomic, assign) CGFloat timeStamp;

/**
 推流地址
 */
@property NSURL * hostURL;

@end

@implementation LiveStreamingViewController

// 隐藏状态栏 for IOS7
- (BOOL)prefersStatusBarHidden;
{
    return YES;
}

// 是否允许旋转 IOS5
-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    return NO;
}

// 是否允许旋转 IOS6
-(BOOL)shouldAutorotate
{
    return NO;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskPortrait;
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    return UIInterfaceOrientationPortrait;
}


- (void)loadView;
{
    [super loadView];
    
    // 直播滤镜列表
    _videoFilters = @[@"Normal",@"Naturally",@"Fair", @"Pink", @"Brilliantly",@"Paul",@"Movie",@"Coral",@"Mint",@"Vivid",@"Honey"];
    _videoFilterIndex = 0;
    
    self.view.backgroundColor = lsqRGB(255, 255, 255);
    
    // 设置全屏 隐藏状态栏 for IOS6
    self.wantsFullScreenLayout = YES;
    [self setNavigationBarHidden:YES];
    [self setStatusBarHidden:YES];
}


- (void)initKSYStreamerSettings
{
    //streamkit initialized
    _kit = [[KSYTuGPUStreamerKit alloc] initWithDefaultCfg];
    _kit.videoOrientation = [[UIApplication sharedApplication] statusBarOrientation];
    _kit.captureDimension = [self.cfgview capResolutionSize];
    _kit.previewDimension = [self.cfgview capResolutionSize];
    _kit.streamDimension  = [self.cfgview strResolutionSize];
    _kit.cameraPosition = [self.cfgview cameraPos];
    _kit.gpuOutputPixelFormat = kCVPixelFormatType_32BGRA;
    _kit.capturePixelFormat   = kCVPixelFormatType_32BGRA;
    _kit.tuVCap.camera.videoDelegate = self;
    _kit.tuVCap.camera.outputSize = CGSizeMake(_kit.captureDimension.height, _kit.captureDimension.width);
    _kit.tuVCap.camera.frameRate = _kit.videoFPS;
    [_kit.tuVCap.camera switchFilterWithCode:_videoFilters[_videoFilterIndex]];

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


- (void)viewDidLoad
{
    [super viewDidLoad];
    // 添加禁止自动锁屏
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    
    self.view.backgroundColor = lsqRGB(255, 255, 255);
    _sessionQueue = dispatch_queue_create("org.lasque.tusdkvideo", DISPATCH_QUEUE_SERIAL);
    
    [self.view addSubview:self.streamState];
    
    //添加手势方法；
    UIView *tapView =  [[UIView alloc]initWithFrame:CGRectMake(0, 100, self.view.lsqGetSizeWidth, self.view.lsqGetSizeWidth)];
    UITapGestureRecognizer *cameraTap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(liveCameraTapEvent)];
    [tapView setBackgroundColor:[UIColor clearColor]];
    [self.view addSubview:tapView];
    [tapView addGestureRecognizer:cameraTap];
    
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [_kit.tuVCap stopCapture];
    [_kit stopPreview];
    self.sessionQueue = nil;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    if (!uiInited)
    {
        [self lsqInitView];
                       
        uiInited = YES;
    }
}

#pragma mark - init view

/**
 *  构建界面元素
 */
- (void)lsqInitView
{
    [self lsqInitViewElements];
    [self initSettingsAndStartPreview];
}

- (void)initSettingsAndStartPreview
{
    NSLog(@"TuSDK Version: %@", lsqSDKVersion);
    [self initKSYStreamerSettings];
    [self.view addSubview:_kit.preview];
    [self.view sendSubviewToBack:_kit.preview];
    _kit.preview.frame = self.view.bounds;
    
    // Off | On
    self.flashModeIndex = 0;
    [self updateFlashModeStatus];
    
    //启动涂图视频采集
    [_kit startVideoCap];
    //启动金山音频采集
    [_kit startAudioCap];
}

-(void)lsqInitViewElements
{
    // 顶部工具栏按钮
    CGRect rect = [[UIScreen mainScreen] applicationFrame];
    
    CGFloat topButtonWidth = 58;
    CGFloat topLeftMargin = 22;
    CGFloat topHeight = 15;
    
    // 关闭按钮
    _mCloseButton = [self buildConfigButton:@"video_style_default_btn_close" frame:CGRectMake(rect.size.width - topLeftMargin -topButtonWidth, topHeight, topButtonWidth, topButtonWidth)];
    // 转换摄像头
    _mToggleCameraButton = [self buildConfigButton:@"video_style_default_btn_switch" frame:CGRectMake(topLeftMargin, topHeight, topButtonWidth, topButtonWidth)];
    // 闪光灯
    _mFlashButton = [self buildConfigButton:@"video_style_default_btn_flash_off" frame:CGRectMake(topLeftMargin + topButtonWidth + 72, topHeight, topButtonWidth, topButtonWidth)];

    // 底部操作按钮
    CGFloat bottomButtonWidth = 120;
    CGFloat stickerButtonWidth = 70;
    CGFloat bottomLeftMargin = 37;
    
    // 播放键
    _mActionButton = [[UIButton alloc] initWithFrame:CGRectMake((rect.size.width - bottomButtonWidth)/2, rect.size.height - bottomButtonWidth - 16, bottomButtonWidth, bottomButtonWidth)];
    // 贴纸按钮
    _mStickerButton = [self buildConfigButton:@"video_style_default_btn_sticker" frame:CGRectMake(bottomLeftMargin, rect.size.height - bottomLeftMargin - stickerButtonWidth, stickerButtonWidth, stickerButtonWidth)];
    // 滤镜按钮
    _mFilterButton = [self buildConfigButton:@"video_style_default_btn_filter" frame:CGRectMake(rect.size.width - bottomLeftMargin - stickerButtonWidth, rect.size.height - bottomLeftMargin - stickerButtonWidth, stickerButtonWidth, stickerButtonWidth)];
    
    [_mActionButton setImage:[UIImage imageNamed:@"video_style_default_camera_btn_record"] forState:UIControlStateNormal];
    [_mActionButton setImage:[UIImage imageNamed:@"video_style_default_camera_btn_pause"] forState:UIControlStateSelected];
    
    [_mActionButton setAdjustsImageWhenHighlighted:NO];
    _mActionButton.layer.cornerRadius = bottomButtonWidth/2;
    [_mActionButton addTouchUpInsideTarget:self action:@selector(onActionHandle:)];
    [self.view addSubview:_mActionButton];
}

#pragma mark - 点击事件方法
- (void)liveCameraTapEvent
{
   
    if (_stickerView) {
        _stickerView.hidden = YES;
    }
    
    if (_filterView) {
        _filterView.hidden = YES;
    }
    [self updateBottomButtonStatus:NO];

}

- (UILabel*)streamState
{
    if(!_streamState)
    {
        _streamState = [[UILabel alloc] initWithFrame:CGRectMake(20, 150, 280, 140)];
        _streamState.text = @" ";
        _streamState.textColor = [UIColor redColor];
        _streamState.numberOfLines = -1;
        _streamState.font = [UIFont boldSystemFontOfSize:14.f];
    }
    return _streamState;
}

- (UIButton *)buildConfigButton:(NSString *)iconName frame:(CGRect)frame
{
    UIButton *btn = [[UIButton alloc] initWithFrame:frame];
    
    UIImage *image = [UIImage imageNamed:iconName];
    [btn setImage:image forState:UIControlStateNormal];
    [btn addTarget:self action:@selector(onConfigButtonClicked:) forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:btn];
    return btn;
}

- (void)switchFilter:(NSString *)code
{
    dispatch_async(self.sessionQueue, ^{
        [_kit.tuVCap.camera switchFilterWithCode:code];
    });
}

- (void)dealloc
{
    self.sessionQueue = nil;
}

#pragma mark - Action

- (void)onConfigButtonClicked:(id)sender
{
    if (sender == self.mCloseButton)
    {
        [self dismissModalViewControllerAnimated];
    }
    else if (sender == self.mFilterButton)
    {
        [self updateBottomButtonStatus:YES];
        
        if (_filterView) {
            _filterView.hidden = NO;
        }else{
            CGFloat filterViewHeight = 246;
            _filterView = [[FilterView alloc]initWithFrame:CGRectMake(0, self.view.lsqGetSizeHeight - filterViewHeight, self.view.lsqGetSizeWidth, filterViewHeight)];
            _filterView.canAdjustParameter = true;
            _filterView.filterEventDelegate = self;
            _filterView.currentFilterTag = _videoFilterIndex;
            [_filterView createFilterWith:_videoFilters];
            [_filterView refreshAdjustParameterViewWith:_currentFilter.code filterArgs:_currentFilter.filterParameter.args];
            
            [self.view addSubview:_filterView];
        }
    }
    else if (sender == self.mStickerButton)
    {
        [self updateBottomButtonStatus:YES];
        
        if (_stickerView) {
            _stickerView.hidden = NO;
        }else{
        CGFloat stickerViewHeight = 246;
        _stickerView = [[StickerScrollView alloc]initWithFrame:CGRectMake(0, self.view.lsqGetSizeHeight - stickerViewHeight, self.view.lsqGetSizeWidth, stickerViewHeight)];
        _stickerView.stickerDelegate = self;
        _stickerView.backgroundColor = [UIColor colorWithRed:0.22 green:0.22 blue:0.22 alpha:0.7];
        [self.view addSubview:_stickerView];
        }
    }
    else if (sender == self.mToggleCameraButton)
    {
        [_kit.tuVCap.camera rotateCamera];
        
        [self.mFlashButton setEnabled:_kit.tuVCap.camera.cameraPosition == AVCaptureDevicePositionBack];
    }
    else if (sender == self.mFlashButton)
    {
        self.flashModeIndex++;
        
        if (self.flashModeIndex >1)
        {
            self.flashModeIndex = 0;
        }
        
        [self updateFlashModeStatus];
        
        dispatch_async(self.sessionQueue, ^{
            [_kit.tuVCap.camera flashWithMode:[self getFlashModeByValue:self.flashModeIndex]];
        });
    }
}

/**
 *  更新操作按钮
 *
 *  @param isRunning 是否直播中
 */
- (void)updateShowStatus:(BOOL)isRunning
{
    NSString *imageName = isRunning ? @"video_style_default_camera_btn_pause" : @"video_style_default_camera_btn_record";
    UIImage *image = [UIImage imageNamed:imageName];
    [_mActionButton setImage:image forState:UIControlStateNormal];
}

- (void)updateBottomButtonStatus:(BOOL)isHidden
{
    self.mActionButton.hidden = isHidden;
    self.mFilterButton.hidden = isHidden;
    self.mStickerButton.hidden = isHidden;
}

- (AVCaptureFlashMode)getFlashModeByValue:(NSInteger)value
{
    
    if(value == 1)
    {
        return AVCaptureFlashModeOn;
    }
    
    return AVCaptureFlashModeOff;
}

// 更新闪光灯模式
- (void)updateFlashModeStatus
{
    NSString *imageName = @"";
    
    if(_flashModeIndex == 1)
    {
        imageName = @"video_style_default_btn_flash_on";
    }
    else
    {
        imageName = @"video_style_default_btn_flash_off";
    }
    
    UIImage *image = [UIImage imageNamed:imageName];
    [_mFlashButton setImage:image forState:UIControlStateNormal];
    
    [_mFlashButton setEnabled:_kit.tuVCap.camera.cameraPosition == AVCaptureDevicePositionBack];
}

#pragma mark - 开启直播的事件
/**
 是否开启直播
 
 @param sender
 */
- (void)onActionHandle:(id)sender
{
    if (_kit.streamerBase.streamState == KSYStreamStateIdle ||
        _kit.streamerBase.streamState == KSYStreamStateError) {
        [_kit.streamerBase startStream:_hostURL];
        [self updateShowStatus:YES];
    }
    else {
        [_kit.streamerBase stopStream];
        [self updateShowStatus:NO];
    }
}

#pragma mark -- 贴纸栏点击代理方法 StickerViewClickDelegate
- (void)clickStickerViewWith:(TuSDKPFStickerGroup *)stickGroup{
    if (!stickGroup) {
        //为nil时 移除已有贴纸组；
        [_kit.tuVCap.camera removeAllLiveSticker];
        return;
    }
    //展示对应贴纸组；
    [_kit.tuVCap.camera showGroupSticker:stickGroup];
}

#pragma mark -- 滤镜栏点击代理方法 FilterEventDelegate
- (void)filterViewParamChangedWith:(TuSDKICSeekBar *)seekbar changedProgress:(CGFloat)progress{
    //根据tag获得当前滤镜的对应参数，修改precent;
    NSInteger index = seekbar.tag;
    TuSDKFilterArg *arg = _currentFilter.filterParameter.args[index];
    arg.precent = progress;
    //设置滤镜参数；
    [_currentFilter submitParameter];
}

- (void)filterViewSwitchFilterWithCode:(NSString *)filterCode{
    //切换滤镜
    [_kit.tuVCap.camera switchFilterWithCode:filterCode];
}

#pragma mark - camera回调改变滤镜的方法
/**
 *  相机状态改变 (如需操作UI线程， 请检查当前线程是否为主线程)
 *
 *  @param camera 相机对象
 *  @param state  相机运行状态
 */
- (void)onVideoCamera:(id<TuSDKVideoCameraInterface>)camera stateChanged:(lsqCameraState)state;
{
    
}

/**
 *  相机滤镜改变 (如需操作UI线程， 请检查当前线程是否为主线程)
 *
 *  @param camera    相机对象
 *  @param newFilter 新的滤镜对象
 */
- (void)onVideoCamera:(id<TuSDKVideoCameraInterface>)camera filterChanged:(TuSDKFilterWrap *)newFilter;
{
    //赋值新滤镜 刷新新滤镜的参数配置；
    _currentFilter = newFilter;
    [_filterView refreshAdjustParameterViewWith:newFilter.code filterArgs:newFilter.filterParameter.args];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - TuSDKLiveVideoCameraDelegate 返回视频数据
/**
 获取滤镜处理后的帧数据, pixelFormatType 为 lsqFormatTypeBGRA 或 lsqFormatTypeYUV420F 时调用
 
 @param camera      相机
 @param pixelBuffer 帧数据, CVPixelBufferRef 类型, 默认为 kCVPixelFormatType_32BGRA 格式
 @param frameTime   帧时间戳
 */
- (void)onVideoCamera:(TuSDKLiveVideoCamera *)camera bufferData:(CVPixelBufferRef)pixelBuffer time:(CMTime)frameTime;
{
    // 将 pixelBuffer 用于编码和推流
    //NSLog(@"抛出来的视频数据 new buffer size: %zu - %zu", CVPixelBufferGetWidth(pixelBuffer), CVPixelBufferGetHeight(pixelBuffer));
    [_kit.capToGpu processPixelBuffer:pixelBuffer time:frameTime];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
