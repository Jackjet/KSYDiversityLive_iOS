//
//  ViewController.m
//  lianmai
//
//  Created by dx on 17/8/1.
//  Copyright © 2017年 NewAuto video team. All rights reserved.
//
#import "ViewController.h"
#import "KSYNVGPUStreamerKit.h"

@interface ViewController ()<NvsStreamingContextDelegate>{
}
// 推流地址 完整的URL
@property NSURL * hostURL;
@property (weak, nonatomic) IBOutlet UIView *messageBox;
@property (weak, nonatomic) IBOutlet UITextView *messageBoxText;
@property (weak, nonatomic) IBOutlet UIButton *messageBoxOkButton;
@property (weak, nonatomic) IBOutlet UIButton *scene1;
@property (weak, nonatomic) IBOutlet UIButton *scene2;

@end

#define SCENE_COUNT     2

@implementation ViewController
{
    KSYNVGPUStreamerKit *_streamKit;//推流kit
    UILabel  *_streamState;//推流状态
    bool m_keyColorSelected;
    unsigned int deviceIndex;
}

- (void)initSettings
{
    //streamkit initialized
    _streamKit = [[KSYNVGPUStreamerKit alloc] initWithDefaultCfg];
    _streamKit.videoOrientation = [[UIApplication sharedApplication] statusBarOrientation];
    _streamKit.previewDimension = [self.cfgview capResolutionSize];
    _streamKit.streamDimension  = [self.cfgview strResolutionSize];
    _streamKit.cameraPosition = [self.cfgview cameraPos];
    _streamKit.gpuOutputPixelFormat = kCVPixelFormatType_32BGRA;
    _streamKit.capturePixelFormat   = kCVPixelFormatType_32BGRA;
    [self setStreamCfg];
    NSString *version = [_streamKit.streamerBase getKSYVersion];
    NSLog(@"ksyun sdk version: %@", version);
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(streamStateChanged) name:KSYStreamStateDidChangeNotification object:nil];
    
    _streamKit.nvCap.context.delegate = self;
}

- (void)setStreamCfg{
    if (_cfgview) {
        _streamKit.streamerBase.videoCodec       = [_cfgview videoCodec];
        _streamKit.streamerBase.videoInitBitrate = [_cfgview videoKbps]*6/10;//60%
        _streamKit.streamerBase.videoMaxBitrate  = [_cfgview videoKbps];
        _streamKit.streamerBase.videoMinBitrate  = 0; //
        _streamKit.streamerBase.audioCodec       = [_cfgview audioCodec];
        _streamKit.streamerBase.audiokBPS        = [_cfgview audioKbps];
        _streamKit.streamerBase.videoFPS         = [_cfgview frameRate];
        _streamKit.streamerBase.bwEstimateMode   = [_cfgview bwEstMode];
        _streamKit.streamerBase.logBlock = ^(NSString* str){
            //NSLog(@"%@", str);
        };
        _hostURL = [NSURL URLWithString:[_cfgview hostUrl]];
        NSLog(@"streamer url:%@", _hostURL);
    }
}

- (void)streamStateChanged{
    NSLog(@"stream State %@", [_streamKit.streamerBase getCurStreamStateName]);
    switch (_streamKit.streamerBase.streamState) {
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
    m_keyColorSelected = false;
    
    self.messageBox.layer.cornerRadius = 5;
    self.messageBox.layer.masksToBounds = YES;
    //推流连接状态
    _streamState = [[UILabel alloc] initWithFrame:CGRectMake(self.view.bounds.size.width/2 - 100, 20, 200, 50)];
    _streamState.textColor = [UIColor redColor];
    _streamState.textAlignment = NSTextAlignmentCenter;
    //视图
    [self.view addSubview:_streamState];
    [self initSettings];
    [self.view addSubview:_streamKit.preview];
    [self.view sendSubviewToBack:_streamKit.preview];
    _streamKit.preview.frame = self.view.bounds;
    UITapGestureRecognizer*tapGesture = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tapPreview:)];
    [_streamKit.preview addGestureRecognizer:tapGesture];
    
    deviceIndex = 0;
    if (_streamKit.cameraPosition == AVCaptureDevicePositionFront){
        deviceIndex = 1;
    }
    
    //美摄视频采集
    [_streamKit.nvCap startNVCap:deviceIndex];
    //开启音频采集
    [_streamKit startAudioCap];
}

- (void)didCaptureDevicePreviewStarted:(unsigned int)captureDeviceIndex
{
    NSString *text = @"本示例用于演示拍摄视频时的虚拟场景，需要对视频进行抠像，请确保您的背景是一块比较单一的颜色，比如是一块蓝布或者绿布，或者是颜色比较单一的墙面。";
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"使用提示"
                                                                             message:text
                                                                      preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"我知道了" style:UIAlertActionStyleDefault handler:^(UIAlertAction *action){
        self.messageBoxText.text = @"请在视频中的单色背景部分点击一下以便将背景去掉。点击后背景部分会变为黑色，如果觉得抠像效果不满意可再点击背景的其它位置。满意后请点击'确认'";
        self.messageBox.hidden = FALSE;
    }];
    [alertController addAction:okAction];
    [self presentViewController:alertController animated:YES completion:nil];
}

//采集设备就绪
- (void)didCaptureDeviceCapsReady:(unsigned int)captureDeviceIndex {
    //获取采集视频尺寸大小
    NvsSize previewSize = [_streamKit.nvCap.context getCapturePreviewVideoSize:deviceIndex];
    _streamKit.captureDimension = CGSizeMake(previewSize.height,previewSize.width);
    [_streamKit updatePreDimension];
    _streamKit.kitReady = true;
    NSLog(@"格式配置完成");
}

- (void) tapPreview:(UIGestureRecognizer *)sender{
    if (m_keyColorSelected)
        return;
    
    CGPoint pos = [sender locationInView:_streamKit.preview];
    [_streamKit getTapRect:pos];
}

- (IBAction)onMessageBoxDone:(id)sender
{
    if (!m_keyColorSelected) {
        m_keyColorSelected = true;
        self.messageBoxText.text = @"现在请从左下角选择一个虚拟场景";
        self.messageBoxOkButton.enabled = FALSE;
        self.scene1.enabled = YES;
        self.scene2.enabled = YES;
        return;
    }
}

- (IBAction)onScene1Tapped:(id)sender
{
    _messageBox.hidden = YES;
    UIButton *button = (UIButton *)sender;
    [_streamKit.nvCap selectScene:(unsigned int)button.tag];
}

- (IBAction)onScene2Tapped:(id)sender
{
    _messageBox.hidden = YES;
    UIButton *button = (UIButton *)sender;
    [_streamKit.nvCap selectScene:(unsigned int)button.tag];
}

- (IBAction)OnStream:(id)sender {
    if (_streamKit.streamerBase.streamState == KSYStreamStateIdle ||
        _streamKit.streamerBase.streamState == KSYStreamStateError) {
        [_streamKit.streamerBase startStream:_hostURL];
    }
    else {
        [_streamKit.streamerBase stopStream];
    }
}

- (IBAction)OnBack:(id)sender{
    [self close];
    [self dismissViewControllerAnimated:FALSE completion:nil];
}

- (void)close{
    [_streamKit stopPreview];
    _streamKit.nvCap.context.delegate = nil;
    [_streamKit closeNvKit];
    _streamKit = nil;
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)dealloc {
    [self close];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
