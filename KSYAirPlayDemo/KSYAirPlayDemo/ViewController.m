//
//  ViewController.m
//  KSYAirPlayDemo
//
//  Created by 江东 on 17/5/12.
//  Copyright © 2017年 江东. All rights reserved.
//

#import "ViewController.h"
#import <XDWScreenRecorder/XDWScreenRecorder.h>
#import <libksygpulive/KSYGPUStreamerKit.h>
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

@interface ViewController ()<UIGestureRecognizerDelegate, XDWScreenRecorderDelegate>{
}
@property(nonatomic, strong) XDWScreenRecorder *screenRecorder;
@property (nonatomic, strong) KSYGPUStreamerKit * kit;
@end

@implementation ViewController

- (XDWScreenRecorder *)screenRecorder {
    if (!_screenRecorder) {
        XDWScreenRecorderConfig *screenRecorderConfig = [[XDWScreenRecorderConfig alloc] init];
        screenRecorderConfig.videoSize = CGSizeMake(1280, 1280);
        screenRecorderConfig.framerate = 15;
        screenRecorderConfig.airTunesPort = 57000;
        screenRecorderConfig.airVideoPort = 8101;
        screenRecorderConfig.activeCode = "000000000";
        screenRecorderConfig.airPlayName = "XBMC-GAMEBOX(XinDawn)";
        screenRecorderConfig.autoRotate = 0; //0 or 90 or 270
        
        _screenRecorder = [[XDWScreenRecorder alloc] initWithConfig:screenRecorderConfig];
        _screenRecorder.delegate = self;
    }
    return _screenRecorder;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    NSString * devCode = [[[[[UIDevice currentDevice] identifierForVendor] UUIDString] substringToIndex:3] lowercaseString];
    NSString *rtmpSrv = @"rtmp://120.92.224.235/live";
    _rtmpUrl.text = [  NSString stringWithFormat:@"%@/%@", rtmpSrv, devCode];
    UITapGestureRecognizer *tapGes = [[UITapGestureRecognizer alloc] initWithTarget:self.view action:@selector(endEditing:)];
    tapGes.delegate = self;
    [self.view addGestureRecognizer:tapGes];
    
    _kit = [[KSYGPUStreamerKit alloc] initWithDefaultCfg];
    _kit.streamerBase.videoFPS = 15;
    _kit.streamerBase.videoCodec = KSYVideoCodec_VT264;
    _kit.streamerBase.audioCodec = KSYAudioCodec_AT_AAC;
    _kit.streamerBase.audiokBPS  = 64;
    _kit.streamerBase.videoMaxBitrate  = 4096;
    _kit.streamerBase.videoInitBitrate = 2048;
    _kit.streamerBase.videoMinBitrate  = 1024;
    
    [self addObservers];
}

- (void) addObservers {
    //KSYStreamer state changes
    NSNotificationCenter* dc = [NSNotificationCenter defaultCenter];
    [dc addObserver:self
           selector:@selector(onStreamStateChange:)
               name:KSYStreamStateDidChangeNotification
             object:nil];
    [dc addObserver:self
           selector:@selector(onNetStateEvent:)
               name:KSYNetStateEventNotification
             object:nil];
}
- (void) rmObservers {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void) onStreamStateChange :(NSNotification *)notification{
    NSLog(@"stream State %@", [_kit.streamerBase getCurStreamStateName]);
    if(_kit.streamerBase.streamState == KSYStreamStateError) {
        [self onStreamError: _kit.streamerBase.streamErrorCode];
    }
}
- (void) onStreamError:(KSYStreamErrorCode) errCode{
    NSLog(@"stream Error %@", [_kit.streamerBase getCurKSYStreamErrorCodeName]);
    if (errCode == KSYStreamErrorCode_CONNECT_BREAK ||
        errCode == KSYStreamErrorCode_ENCODE_FRAMES_FAILED ||
        errCode == KSYStreamErrorCode_CODEC_OPEN_FAILED) {
        // Reconnect
        dispatch_time_t delay = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC));
        dispatch_after(delay, dispatch_get_main_queue(), ^{
            [_kit.streamerBase startStream:[NSURL URLWithString:_rtmpUrl.text]];
        });
    }
}
- (void) onNetStateEvent     :(NSNotification *)notification{
    NSLog(@"netevent %lu", (unsigned long)_kit.streamerBase.netStateCode);
}

- (IBAction)airplaySwitch:(UISwitch *)sender {
    if (sender.isOn) {
        NSLog(@"url:%@",[NSURL URLWithString:_rtmpUrl.text]);
        [_kit.streamerBase startStream:[NSURL URLWithString:_rtmpUrl.text]];
        [self.screenRecorder start];
    } else {
        [_kit.streamerBase stopStream];
        [self.screenRecorder stop];
    }
}

- (void)dealloc {
    [self rmObservers];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

#pragma mark - XDWScreenRecorderDelegate

- (void)screenRecorder:(XDWScreenRecorder *)screenRecorder didStartRecordingWithVideoSize:(CGSize)videoSize {
    NSLog(@"width: %.f, height: %.f", videoSize.width, videoSize.height);
    _kit.streamDimension = videoSize;
    [[AVAudioSession sharedInstance]  setCategory:AVAudioSessionCategoryPlayAndRecord withOptions:AVAudioSessionCategoryOptionMixWithOthers error:nil];
    [[AVAudioSession sharedInstance] setMode:AVAudioSessionModeVoiceChat error:nil];
    [[AVAudioSession sharedInstance]  overrideOutputAudioPort:AVAudioSessionPortOverrideSpeaker error:nil];
    [_kit startAudioCap];
}

- (void)screenRecorder:(XDWScreenRecorder *)screenRecorder startError:(NSError *)error {
    
}

- (void)screenRecorder:(XDWScreenRecorder *)screenRecorder videoBuffer:(CVPixelBufferRef)buffer timestamp:(NSTimeInterval)timestamp {
    [_kit.streamerBase processVideoPixelBuffer:buffer timeInfo:CMTimeMake(getABSTime(),NSEC_PER_SEC)];
}

- (void)screenRecorderDidStopRecording:(XDWScreenRecorder *)screenRecorder {
    [_kit.aCapDev  stopCapture];
    [[AVAudioSession sharedInstance]  setCategory:AVAudioSessionCategoryPlayback withOptions:AVAudioSessionCategoryOptionMixWithOthers error:nil];
    [[AVAudioSession sharedInstance]  overrideOutputAudioPort:AVAudioSessionPortOverrideNone error:nil];
}

@end
