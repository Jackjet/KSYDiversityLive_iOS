//
//  KSYPlayerVC.m
//
//  Created by zengfanping on 11/3/15.
//  Copyright (c) 2015 zengfanping. All rights reserved.
//

#import "KSYUIView.h"
#import "KSYPlayerVC.h"
#import "KSYProgressView.h"
#import <KSYMediaPlayer/KSYMediaPlayer.h>
#import "KSYGPUView.h"
#import "KSYGPUPicInput.h"
#import "KSYFaceUnityWrapper.h"

#define ELEMENT_GAP  6

@interface KSYPlayerVC()
@property (strong, nonatomic) NSURL *url;
@property (strong, nonatomic) NSURL *reloadUrl;
@property(strong, nonatomic) NSArray *fileList;
@property (nonatomic, readwrite)  BOOL bRecording;
@property (strong, nonatomic) KSYMoviePlayerController *player;
@property (strong, nonatomic) KSYGPUView     *gpuimageView;
@property (strong, nonatomic) KSYGPUPicInput  *rawdataInput;
@end

@implementation KSYPlayerVC{
    KSYUIView *ctrlView;
    
    UIView *videoView;
    
    UIButton *btnVideo;
    UILabel *labelContenMode;
    UISegmentedControl *segContentMode;             //画面填充模式
    UILabel *labelRotate;
    UISegmentedControl *segRotate;                         //旋转
    UILabel *labelMirror;
    UISegmentedControl *segMirror;                         //镜像
    UIButton *btnShotScreen;
    NSMutableArray *arrayBtnVideo;
    
    UIButton *btnSticker;
    
    UILabel *labelStat;
    UIButton *btnPlay;
    UIButton *btnPause;
    UIButton *btnResume;
    UIButton *btnStop;
    UIButton *btnQuit;

    KSYProgressView *progressView;
    
    double lastSize;
    NSTimeInterval lastCheckTime;
    NSString* serverIp;
    
    BOOL reloading;
    
    BOOL bStopped;
    
    NSMutableArray *registeredNotifications;
}

- (instancetype)initWithURL:(NSURL *)url fileList:(NSArray *)fileList{
    if((self = [super init])) {
        self.url = url;
        self.reloadUrl = url;
        self.fileList = fileList;
    }

    return self;
}

- (void)dealloc {
    [self removeObserver:self forKeyPath:@"player"];
}

#pragma mark layout
- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self setupUI];
    [self addSubViews];
    
    //添加手势操作
    [self registerHandGesture];
    
    [self addObserver:self forKeyPath:@"player" options:NSKeyValueObservingOptionNew context:nil];
    
    [self initPlayerWithURL:_url fileList:_fileList];
    _rawdataInput = [[KSYGPUPicInput alloc] initWithFmt:kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange];
    _gpuimageView = [[KSYGPUView alloc] init];
    [_gpuimageView setFrame:videoView.bounds];
    [videoView addSubview:_gpuimageView];
    [_rawdataInput addTarget:_gpuimageView];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

- (UILabel *)addLabelWithText:(NSString *)text textColor:(UIColor*)textColor {
    UILabel *label = [[UILabel alloc] init];
    label.backgroundColor = [UIColor clearColor];
    label.textColor = textColor;
    label.numberOfLines = -1;
    label.text = text;
    label .textAlignment = NSTextAlignmentLeft;
    [self.view addSubview:label];
    return label;
}

- (void) setupUI {
    ctrlView = [[KSYUIView alloc] initWithFrame:self.view.bounds];
    ctrlView.backgroundColor = [UIColor blackColor];
    ctrlView.gap = ELEMENT_GAP;
    
    @WeakObj(self);
    ctrlView.onBtnBlock = ^(id sender){
        [selfWeak  onBtn:sender];
    };
    ctrlView.onSegCtrlBlock = ^(id sender){
        [selfWeak onSeg:sender];
    };
    
    //add UIView for player
    videoView = [[UIView alloc] init];
    videoView.backgroundColor = [UIColor whiteColor];
    [ctrlView addSubview:videoView];
    
    btnVideo = [ctrlView addButton:@"图像"];
    labelContenMode = [ctrlView addLable:@"填充模式"];
    segContentMode = [ctrlView addSegCtrlWithItems:@[@"无", @"同比", @"裁剪", @"满屏"]];
    segContentMode.selectedSegmentIndex = 1;
    labelRotate = [ctrlView addLable:@"旋转"];
    segRotate = [ctrlView addSegCtrlWithItems:@[@"0", @"90", @"180", @"270"]];
    labelMirror = [ctrlView addLable:@"镜像"];
    segMirror = [ctrlView addSegCtrlWithItems:@[@"正向", @"反向"]];
    btnShotScreen = [ctrlView addButton:@"截图"];
    arrayBtnVideo = [NSMutableArray arrayWithObjects:labelContenMode,segContentMode, labelRotate, segRotate, labelMirror, segMirror, btnShotScreen, nil];
    [self showElements:arrayBtnVideo bShow:NO];
    
    btnSticker = [ctrlView addButton:@"贴纸"];
    
    btnPlay = [ctrlView addButton:@"播放"];
    btnPause = [ctrlView addButton:@"暂停"];
    btnResume = [ctrlView addButton:@"继续"];
    btnStop = [ctrlView addButton:@"停止"];
    btnQuit = [ctrlView addButton:@"退出"];

    progressView = [[KSYProgressView alloc] init];
    [ctrlView addSubview:progressView];
    
    labelStat = [self addLabelWithText:nil textColor:[UIColor redColor]];
    [ctrlView addSubview:labelStat];

    [self layoutUI];

    [self.view addSubview: ctrlView];
}

- (void) layoutUI {
    CGFloat yPos = 0;
    
    ctrlView.frame = self.view.frame;
    [ctrlView layoutUI];
    
    videoView.frame  = ctrlView.frame;
    
    [ctrlView putRow:@[btnVideo, btnSticker]];
    
    yPos  = ctrlView.yPos;
    [ctrlView putLable:labelContenMode andView:segContentMode];
    [ctrlView putLable:labelRotate andView:segRotate];
    [ctrlView putLable:labelMirror andView:segMirror];
    [ctrlView putRow1:btnShotScreen];
   
    ctrlView.yPos = yPos;
    
    //下部控件为3行
    ctrlView.yPos = ctrlView.frame.size.height - ctrlView.gap * 2 - ctrlView.btnH * 2;
    [ctrlView putRow:@[progressView]];
    [ctrlView putRow:@[btnPlay, btnPause, btnResume, btnStop, btnQuit]];
    
    labelStat.frame = self.view.frame;
}

- (void) showElements:(NSMutableArray*)elems bShow:(BOOL)bShow
{
    for(UIControl *elem in elems)
        elem.hidden = !bShow;
}

- (void)addSubViews{
    _ksyFaceUnityView = [[KSYFaceUnityView alloc] initWithParent:ctrlView];
    __weak typeof(self) weakSelf = self;
   _ksyFaceUnityView.stickerFilter.videoProcessingCallback = ^(CMSampleBufferRef sampleBuffer){
        if(weakSelf.rawdataInput)
            [weakSelf.rawdataInput processSampleBuffer:sampleBuffer];
    };
}

#pragma mark common
- (NSTimeInterval) getCurrentTime{
    return [[NSDate date] timeIntervalSince1970];
}

- (void) toast:(NSString*)message{
    UIAlertView *toast = [[UIAlertView alloc] initWithTitle:nil
                                                    message:message
                                                   delegate:nil
                                          cancelButtonTitle:nil
                                          otherButtonTitles:nil, nil];
    [toast show];
    
    double duration = 0.5; // duration in seconds
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(duration * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [toast dismissWithClickedButtonIndex:0 animated:YES];
    });
}

- (NSString *) netStatus2Str:(KSYNetworkStatus)networkStatus{
    NSString *netString = nil;
    if(networkStatus == KSYNotReachable)
        netString = @"NO INTERNET";
    else if(networkStatus == KSYReachableViaWiFi)
        netString = @"WIFI";
    else if(networkStatus == KSYReachableViaWWAN)
        netString = @"WWAN";
    else
        netString = @"Unknown";
    return netString;
}

#pragma mark init player
- (void)initPlayerWithURL:(NSURL *)aURL fileList:(NSArray *)fileList{
    lastSize = 0.0;
    //初始化播放器并设置播放地址
    self.player = [[KSYMoviePlayerController alloc] initWithContentURL: aURL fileList:fileList sharegroup:nil];
    [self setupObservers:_player];
    _player.shouldHideVideo = YES;
    _player.shouldLoop = YES;
    _player.logBlock = ^(NSString *logJson){
        NSLog(@"logJson is %@",logJson);
    };

    __weak typeof(self) weakSelf = self;
    _player.videoDataBlock = ^(CMSampleBufferRef sampleBuffer){
        [weakSelf.ksyFaceUnityView.stickerFilter renderFaceUnity:sampleBuffer];
    };
    //播放视频的实时信息
    labelStat.text = [NSString stringWithFormat:@"url %@", aURL];
    _player.controlStyle = MPMovieControlStyleNone;
    [videoView bringSubviewToFront:labelStat];
    videoView.autoresizesSubviews = TRUE;
    _player.view.autoresizingMask = UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight;
    
    NSKeyValueObservingOptions opts = NSKeyValueObservingOptionNew;
    [_player addObserver:self forKeyPath:@"currentPlaybackTime" options:opts context:nil];
    [_player addObserver:self forKeyPath:@"clientIP" options:opts context:nil];
    [_player addObserver:self forKeyPath:@"localDNSIP" options:opts context:nil];

    [_player prepareToPlay];
}

- (void)registerObserver:(NSString *)notification player:(KSYMoviePlayerController*)player {
    [[NSNotificationCenter defaultCenter]addObserver:self
                                            selector:@selector(handlePlayerNotify:)
                                                name:(notification)
                                              object:player];
}

- (void)setupObservers:(KSYMoviePlayerController*)player
{
    [self registerObserver:MPMediaPlaybackIsPreparedToPlayDidChangeNotification player:player];
    [self registerObserver:MPMoviePlayerPlaybackStateDidChangeNotification player:player];
    [self registerObserver:MPMoviePlayerPlaybackDidFinishNotification player:player];
    [self registerObserver:MPMoviePlayerLoadStateDidChangeNotification player:player];
    [self registerObserver:MPMovieNaturalSizeAvailableNotification player:player];
    [self registerObserver:MPMoviePlayerFirstVideoFrameRenderedNotification player:player];
    [self registerObserver:MPMoviePlayerFirstAudioFrameRenderedNotification player:player];
    [self registerObserver:MPMoviePlayerSuggestReloadNotification player:player];
    [self registerObserver:MPMoviePlayerPlaybackStatusNotification player:player];
    [self registerObserver:MPMoviePlayerNetworkStatusChangeNotification player:player];
    [self registerObserver:MPMoviePlayerSeekCompleteNotification player:player];
}

- (void)releaseObservers:(KSYMoviePlayerController*)player
{
    for (NSString *name in registeredNotifications) {
        [[NSNotificationCenter defaultCenter] removeObserver:self
                                                        name:name
                                                      object:player];
    }
}

-(void)handlePlayerNotify:(NSNotification*)notify
{
    if (!_player) {
        return;
    }
    if (MPMediaPlaybackIsPreparedToPlayDidChangeNotification ==  notify.name) {
        labelStat.text = [NSString stringWithFormat:@"player prepared"];
        progressView.totalTimeInSeconds = _player.duration;
        if(_player.shouldAutoplay == NO)
            [_player play];
        serverIp = [_player serverAddress];
        NSLog(@"KSYPlayerVC: %@ -- ip:%@", [[_player contentURL] absoluteString], serverIp);
        reloading = NO;
    }
    if (MPMoviePlayerPlaybackStateDidChangeNotification ==  notify.name) {
        NSLog(@"------------------------");
        NSLog(@"player playback state: %ld", (long)_player.playbackState);
        NSLog(@"------------------------");
    }
    if (MPMoviePlayerLoadStateDidChangeNotification ==  notify.name) {
        NSLog(@"player load state: %ld", (long)_player.loadState);
        if (MPMovieLoadStateStalled & _player.loadState) {
            labelStat.text = [NSString stringWithFormat:@"player start caching"];
            NSLog(@"player start caching");
        }
        
        if (_player.bufferEmptyCount &&
            (MPMovieLoadStatePlayable & _player.loadState ||
             MPMovieLoadStatePlaythroughOK & _player.loadState)){
                NSLog(@"player finish caching");
                NSString *message = [[NSString alloc]initWithFormat:@"loading occurs, %d - %0.3fs",
                                     (int)_player.bufferEmptyCount,
                                     _player.bufferEmptyDuration];
                [self toast:message];
            }
    }
    if (MPMoviePlayerPlaybackDidFinishNotification ==  notify.name) {
        NSLog(@"player finish state: %ld", (long)_player.playbackState);
        NSLog(@"player download flow size: %f MB", _player.readSize);
        NSLog(@"buffer monitor  result: \n   empty count: %d, lasting: %f seconds",
              (int)_player.bufferEmptyCount,
              _player.bufferEmptyDuration);
        //结束播放的原因
        int reason = [[[notify userInfo] valueForKey:MPMoviePlayerPlaybackDidFinishReasonUserInfoKey] intValue];
        if (reason ==  MPMovieFinishReasonPlaybackEnded) {
            labelStat.text = [NSString stringWithFormat:@"player finish"];
        }else if (reason == MPMovieFinishReasonPlaybackError){
            labelStat.text = [NSString stringWithFormat:@"player Error : %@", [[notify userInfo] valueForKey:@"error"]];
        }else if (reason == MPMovieFinishReasonUserExited){
            labelStat.text = [NSString stringWithFormat:@"player userExited"];
            
        }
    }
    if (MPMovieNaturalSizeAvailableNotification ==  notify.name) {
        NSLog(@"video size %.0f-%.0f, rotate:%ld\n", _player.naturalSize.width, _player.naturalSize.height, (long)_player.naturalRotate);
        if(((_player.naturalRotate / 90) % 2  == 0 && _player.naturalSize.width > _player.naturalSize.height) ||
            ((_player.naturalRotate / 90) % 2 != 0 && _player.naturalSize.width < _player.naturalSize.height))
        {
            //如果想要在宽大于高的时候横屏播放，你可以在这里旋转
        }
    }
    
    if (MPMoviePlayerSuggestReloadNotification == notify.name)
    {
        NSLog(@"suggest using reload function!\n");
        if(!reloading)
        {
            reloading = YES;
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^(){
                if (_player) {
                    NSLog(@"reload stream");
                    [_player reload:_reloadUrl flush:YES mode:MPMovieReloadMode_Accurate];
                }
            });
        }
    }
    
    if(MPMoviePlayerPlaybackStatusNotification == notify.name)
    {
        int status = [[[notify userInfo] valueForKey:MPMoviePlayerPlaybackStatusUserInfoKey] intValue];
        if(MPMovieStatusVideoDecodeWrong == status)
            NSLog(@"Video Decode Wrong!\n");
        else if(MPMovieStatusAudioDecodeWrong == status)
            NSLog(@"Audio Decode Wrong!\n");
        else if (MPMovieStatusHWCodecUsed == status )
            NSLog(@"Hardware Codec used\n");
        else if (MPMovieStatusSWCodecUsed == status )
            NSLog(@"Software Codec used\n");
        else if(MPMovieStatusDLCodecUsed == status)
            NSLog(@"AVSampleBufferDisplayLayer  Codec used");
    }
    if(MPMoviePlayerNetworkStatusChangeNotification == notify.name)
    {
        int currStatus = [[[notify userInfo] valueForKey:MPMoviePlayerCurrNetworkStatusUserInfoKey] intValue];
        int lastStatus = [[[notify userInfo] valueForKey:MPMoviePlayerLastNetworkStatusUserInfoKey] intValue];
        NSLog(@"network reachable change from %@ to %@\n", [self netStatus2Str:lastStatus], [self netStatus2Str:currStatus]);
    }
    if(MPMoviePlayerSeekCompleteNotification == notify.name)
    {
        NSLog(@"Seek complete");
    }
}

#pragma mark on Button
- (void)onBtn:(UIButton *)btn{
    if(btn == btnPlay)
        [self onPlayVideo:btn];
    else if(btn == btnPause)
        [self onPauseVideo:btn];
    else if(btn == btnResume)
        [self onResumeVideo:btn];
    else if(btn == btnStop)
        [self onStopVideo:btn];
    else if(btn == btnQuit)
        [self onQuit:btn];
    else if(btn == btnVideo) {
        _ksyFaceUnityView.hidden = YES;
        btnSticker.selected = NO;
        [self showElements:arrayBtnVideo bShow:!btn.selected];
        btn.selected = !btn.selected;
    }
    else if(btn == btnSticker) {
        [self showElements:arrayBtnVideo bShow:NO];
        btnVideo.selected = NO;
        btn.selected = !btn.selected;
        KSYUIView * view = _ksyFaceUnityView;
        view.frame = CGRectMake(0,  ctrlView.gap + CGRectGetMaxY(btnSticker.frame), ctrlView.width, CGRectGetMinY(progressView.frame) - ctrlView.gap - CGRectGetMaxY(btnSticker.frame));
        [view layoutUI];
        view.hidden = !view.hidden;
    }
    else if(btn == btnShotScreen)
        [self onShotScreen:btn];
}

- (IBAction)onPlayVideo:(id)sender {
    if(nil == _player)
        [self initPlayerWithURL:_url fileList:_fileList];
    if(_player)
    {
        NSLog(@"sdk version:%@", [_player getVersion]);
        //如果再次播放的话，设置下次播放的地址
        if(bStopped == YES)
             [_player setUrl:_url];
        [_player prepareToPlay];
        bStopped = NO;
    }
}

- (IBAction)onPauseVideo:(id)sender {
    if (_player) {
        [_player pause];
    }
}

- (IBAction)onResumeVideo:(id)sender {
    if (_player) {
        [_player play];
    }
}

- (IBAction)onStopVideo:(id)sender {
    if (_player) {
        NSLog(@"player download flow size: %f MB", _player.readSize);
        NSLog(@"buffer monitor  result: \n   empty count: %d, lasting: %f seconds",
              (int)_player.bufferEmptyCount,
              _player.bufferEmptyDuration);
        
        [_player reset:NO];
        bStopped = YES;
    }
}

- (IBAction)onQuit:(id)sender {
    if(_player)
    {
        [_player stop];
        [_player removeObserver:self forKeyPath:@"currentPlaybackTime" context:nil];
        [_player removeObserver:self forKeyPath:@"clientIP" context:nil];
        [_player removeObserver:self forKeyPath:@"localDNSIP" context:nil];
        
        [self releaseObservers:_player];
        
        [_player.view removeFromSuperview];
        self.player = nil;
    }
    [_ksyFaceUnityView removeFromSuperview];
    _ksyFaceUnityView = nil;
    [self dismissViewControllerAnimated:FALSE completion:nil];
    labelStat.text = nil;
}

- (IBAction)onShotScreen:(id)sender {
    if (_player) {
        UIImage *thumbnailImage = _player.thumbnailImageAtCurrentTime;
        [KSYUIVC saveImageToPhotosAlbum:thumbnailImage];
    }
}

- (IBAction)onReloadVideo:(id)sender {
    if (_player) {
        [_player reload:_reloadUrl flush:YES mode:MPMovieReloadMode_Accurate];
    }
}

#pragma mark on SegmentedControl
- (void)onSeg:(UISegmentedControl *)seg{
    if(seg == segContentMode)
        [self onContentMode];
    else if(seg == segRotate)
        [self onRotate];
    else if(seg == segMirror)
        [self onMirror];
}

- (void)onContentMode{
    MPMovieScalingMode contentMode = MPMovieScalingModeNone;
    switch(segContentMode.selectedSegmentIndex) {
        case 0:
            contentMode = MPMovieScalingModeNone;
            break;
        case 1:
            contentMode = MPMovieScalingModeAspectFit;
            break;
        case 2:
            contentMode = MPMovieScalingModeAspectFill;
            break;
        case 3:
            contentMode = MPMovieScalingModeFill;
            break;
        default:
            contentMode = MPMovieScalingModeNone;
            break;
    }
    
    if (_player) {
        _player.scalingMode = contentMode;
    }
}

- (void)onRotate{
    int degress = (int)segRotate.selectedSegmentIndex * 90;
    if (_player) {
        _player.rotateDegress = degress;
    }
}

- (void)onMirror{
    if (_player) {
        _player.mirror = segMirror.selectedSegmentIndex;
    }
}

#pragma mark update label
- (void)onTimer:(NSTimer *)t
{
    [self updateCacheProgress];
}

- (void)updateCacheProgress {
    CGFloat duration = _player.duration;
    CGFloat playableDuration = _player.playableDuration;
    if(duration > 0){
        progressView.cacheProgress = playableDuration / duration;
    }
    else{
        progressView.cacheProgress = 0.0;
    }
}

#pragma mark kvo
- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context
{
    if([keyPath isEqual:@"currentPlaybackTime"])
    {
        progressView.playProgress = _player.currentPlaybackTime / _player.duration;
    }
    else if([keyPath isEqual:@"clientIP"])
    {
        NSLog(@"client IP is %@\n", [change objectForKey:NSKeyValueChangeNewKey]);
    }
    else if([keyPath isEqual:@"localDNSIP"])
    {
        NSLog(@"local DNS IP is %@\n", [change objectForKey:NSKeyValueChangeNewKey]);
    }
    else if ([keyPath isEqualToString:@"player"]) {
        if (_player) {
            progressView.hidden = NO;
            __weak typeof(_player) weakPlayer = _player;
            progressView.dragingSliderCallback = ^(float progress){
                typeof(weakPlayer) strongPlayer = weakPlayer;
                double seekPos = progress * strongPlayer.duration;
                //strongPlayer.currentPlaybackTime = progress * strongPlayer.duration;
                //使用currentPlaybackTime设置为依靠关键帧定位
                //使用seekTo:accurate并且将accurate设置为YES时为精确定位
                [strongPlayer seekTo:seekPos accurate:YES];
            };
        } else {
            progressView.hidden = YES;
        }
    }
}

#pragma mark on hand gesture
- (void)registerHandGesture{
    UISwipeGestureRecognizer *leftSwipeRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture:)];
    leftSwipeRecognizer.direction = UISwipeGestureRecognizerDirectionLeft;
    UISwipeGestureRecognizer *rightSwipeRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture:)];
    rightSwipeRecognizer.direction = UISwipeGestureRecognizerDirectionRight;
    UISwipeGestureRecognizer *upSwipeRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture:)];
    upSwipeRecognizer.direction = UISwipeGestureRecognizerDirectionUp;
    UISwipeGestureRecognizer *downSwipeRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeGesture:)];
    downSwipeRecognizer.direction = UISwipeGestureRecognizerDirectionDown;
    [self.view addGestureRecognizer:leftSwipeRecognizer];
    [self.view addGestureRecognizer:rightSwipeRecognizer];
    [self.view addGestureRecognizer:upSwipeRecognizer];
    [self.view addGestureRecognizer:downSwipeRecognizer];
    
    //捏合缩放
    UIPinchGestureRecognizer *pinchGesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinchView:)];
    [self.view addGestureRecognizer:pinchGesture];
    //旋转
    UIRotationGestureRecognizer *rotationGesture = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(rotateView:)];
    [self.view addGestureRecognizer:rotationGesture];
}

//上下左右滑动
- (void)handleSwipeGesture:(UISwipeGestureRecognizer *)swpie {
    if (swpie.direction == UISwipeGestureRecognizerDirectionRight) {
        CGRect originalFrame = labelStat.frame;
        labelStat.frame = CGRectMake(0, originalFrame.origin.y, originalFrame.size.width, originalFrame.size.height);
    }
    if (swpie.direction == UISwipeGestureRecognizerDirectionLeft) {
        CGRect originalFrame = labelStat.frame;
        labelStat.frame = CGRectMake(-originalFrame.size.width, originalFrame.origin.y, originalFrame.size.width, originalFrame.size.height);
    }
}

// 处理捏合缩放手势
- (void) pinchView:(UIPinchGestureRecognizer *)pinchGestureRecognizer
{
    UIView *view = videoView;
    if (pinchGestureRecognizer.state == UIGestureRecognizerStateBegan || pinchGestureRecognizer.state == UIGestureRecognizerStateChanged) {
        view.transform = CGAffineTransformScale(view.transform, pinchGestureRecognizer.scale, pinchGestureRecognizer.scale);
        pinchGestureRecognizer.scale = 1;
    }
}

// 处理旋转手势
- (void) rotateView:(UIRotationGestureRecognizer *)rotationGestureRecognizer
{
    UIView *view = videoView;
    if([rotationGestureRecognizer state] == UIGestureRecognizerStateEnded) {
        view.transform = CGAffineTransformIdentity;
        return;
    }
    
    if (rotationGestureRecognizer.state == UIGestureRecognizerStateBegan || rotationGestureRecognizer.state == UIGestureRecognizerStateChanged) {
        view.transform = CGAffineTransformRotate(view.transform, rotationGestureRecognizer.rotation);
        [rotationGestureRecognizer setRotation:0];
    }
}
@end
