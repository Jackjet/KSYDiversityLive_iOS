//
//  ViewController.m
//  KSYiflyMSCDemo
//
//  Created by yuyang on 2017/12/14.
//  Copyright © 2017年 Ksyun. All rights reserved.
//






#import "ViewController.h"
#import <GPUImage/GPUImage.h>
#import "KSYGPUStreamerKit.h"
#import <iflyMSC/IFlyMSC.h>
#import <CoreMedia/CoreMedia.h>
#import "AVAudioSession+KSYDemoAudioSession.h"


#define PUTONGHUA   @"mandarin"
#define YUEYU       @"cantonese"
#define ENGLISH     @"en_us"
#define CHINESE     @"zh_cn"
#define SICHUANESE  @"lmz"

#define kStreamerUrl @"rtmp://120.92.224.235/live"
#define kPlayerUrl   @"rtmp://120.92.224.235/live"

@interface ViewController ()<IFlySpeechRecognizerDelegate>
@property (nonatomic, strong) IFlySpeechRecognizer *iFlySpeechRecognizer;
@property (nonatomic, strong) KSYGPUStreamerKit *streamerKit;
@property (nonatomic,assign)BOOL isAudioPlay;
@property (nonatomic,copy) NSString *hostUrl;
@property (weak, nonatomic) IBOutlet UITextField *streamerUrlText;
@property (weak, nonatomic) IBOutlet UILabel *playLable;


@end

@implementation ViewController


- (IBAction)startStream:(id)sender {
    
    if (self.streamerUrlText.text.length==0) {
        return;
    }
    if (_streamerKit.streamerBase.streamState == KSYStreamStateIdle ||
        _streamerKit.streamerBase.streamState == KSYStreamStateError) {
        [self.streamerKit.streamerBase startStream:[NSURL URLWithString:self.streamerUrlText.text]];
        NSLog(@"开始推流");
    }
}
- (IBAction)stopStream:(id)sender {
    NSLog(@"停止推流");
    [self.streamerKit.streamerBase stopStream];
    
}
- (IBAction)showText:(id)sender {
    [self startListening];
}
- (IBAction)hiddenText:(id)sender {
    [self stopListening];
}

- (void)stopListening
{
    [_iFlySpeechRecognizer stopListening];
    _iFlySpeechRecognizer = nil;
    self.isAudioPlay = NO;
    _streamerKit.textLabel.text = @" ";
    [_streamerKit updateTextLabel];
    NSLog(@"关闭字幕");
}

- (void)startListening
{
    if (!_iFlySpeechRecognizer) {
        [self initIFlySpeech];
    }
    BOOL ret  = [_iFlySpeechRecognizer startListening];
    if (ret) {
        self.isAudioPlay = YES;
        NSLog(@"打开字幕");
    }
}



- (void)handleSampleBufferRef:(CMSampleBufferRef )sampleBuffer
{
    NSData *data =[self dataFromBuffer:sampleBuffer];
    if (data.length!=0) {
        if (!_iFlySpeechRecognizer) {
            [self initIFlySpeech];
        }
        int ret = [_iFlySpeechRecognizer writeAudio:data];
        if (!ret)
        {
            [_iFlySpeechRecognizer stopListening];
        }
        
    }
    
}


- (void)viewDidLoad {
    [super viewDidLoad];
    self.isAudioPlay = NO;
    self.streamerUrlText.text =[self getSteamer];
    self.playLable.text = [NSString stringWithFormat:@"播放地址:%@",[self getPlayerUrl]];
}


- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    [self initIFlySpeech];
    [self prepareStreamerKit];
    __weak typeof(self)weakSelf = self;
    _streamerKit.audioProcessingCallback = ^(CMSampleBufferRef sampleBuffer) {
        __strong typeof(weakSelf)strongSelf =weakSelf;
        if (strongSelf.isAudioPlay) {
            [strongSelf handleSampleBufferRef:sampleBuffer];
        }
        
    };
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (void)prepareStreamerKit{
    
    _streamerKit = [[KSYGPUStreamerKit alloc] initWithInterruptCfg];
    //采集参数
    _streamerKit.capPreset           = AVCaptureSessionPreset640x480;
    _streamerKit.previewDimension   = CGSizeMake(640, 360);
    _streamerKit.videoOrientation   = UIInterfaceOrientationPortrait;
    _streamerKit.streamOrientation  = UIInterfaceOrientationPortrait;
    _streamerKit.previewOrientation = UIInterfaceOrientationPortrait;
    _streamerKit.capturePixelFormat   = kCVPixelFormatType_32BGRA;
    //推流参数
    _streamerKit.streamerBase.videoCodec = KSYVideoCodec_AUTO;
    _streamerKit.streamerBase.bwEstimateMode = KSYBWEstMode_Default;
    _streamerKit.streamerBase.audioCodec = KSYAudioCodec_AAC_HE;
    
    _streamerKit.textLabel = self.lable;
    _streamerKit.textRect = CGRectMake(0, 0.8, 0, 20/self.view.frame.size.height);
    // 开启预览
    [_streamerKit startPreview:self.view];
    
}

- (UILabel *)lable
{
    UILabel  *lable = [[UILabel alloc]init];
    lable.textColor = [UIColor blackColor];
    lable.font = [UIFont systemFontOfSize:16];
    return lable;
}


//具体操作请参考讯飞api
- (void)initIFlySpeech
{
    //创建语音识别对象
    _iFlySpeechRecognizer = [IFlySpeechRecognizer sharedInstance];
    
    _iFlySpeechRecognizer.delegate =self;
    
    [_iFlySpeechRecognizer setParameter:@"" forKey:[IFlySpeechConstant PARAMS]];
    
    //set recognition domain
    [_iFlySpeechRecognizer setParameter:@"iat" forKey:[IFlySpeechConstant IFLY_DOMAIN]];
    
    [_iFlySpeechRecognizer setParameter:@"json" forKey:[IFlySpeechConstant RESULT_TYPE]];
    
    if (_iFlySpeechRecognizer != nil) {
        
        //set timeout of recording
        [_iFlySpeechRecognizer setParameter:@"30000" forKey:[IFlySpeechConstant SPEECH_TIMEOUT]];
        //set VAD timeout of end of speech(EOS)
        [_iFlySpeechRecognizer setParameter:@"3000" forKey:[IFlySpeechConstant VAD_EOS]];
        //set VAD timeout of beginning of speech(BOS)
        [_iFlySpeechRecognizer setParameter:@"3000" forKey:[IFlySpeechConstant VAD_BOS]];
        //set network timeout
        [_iFlySpeechRecognizer setParameter:@"20000" forKey:[IFlySpeechConstant NET_TIMEOUT]];
        
        //set sample rate, 16K as a recommended option
        [_iFlySpeechRecognizer setParameter:@"16000" forKey:[IFlySpeechConstant SAMPLE_RATE]];
        
        //set language
        [_iFlySpeechRecognizer setParameter:CHINESE forKey:[IFlySpeechConstant LANGUAGE]];
        //set accent
        [_iFlySpeechRecognizer setParameter:PUTONGHUA forKey:[IFlySpeechConstant ACCENT]];
        
        //set whether or not to show punctuation in recognition results
        [_iFlySpeechRecognizer setParameter:@"0" forKey:[IFlySpeechConstant ASR_PTT]];
        
    }
    
}


#pragma mark IFlySpeechRecognizerDelegate
- (void) onError:(IFlySpeechError *) errorCode
{
    
}

- (void) onResults:(NSArray *) results isLast:(BOOL)isLast
{
    NSMutableString *resultString = [[NSMutableString alloc] init];
    NSDictionary *dic = results[0];
    
    for (NSString *key in dic) {
        [resultString appendFormat:@"%@",key];
    }
    resultString = (NSMutableString*) [self stringFromJson:resultString];
    if (isLast){ //如果超时 继续监听
        [_iFlySpeechRecognizer startListening];
    }
    if (resultString.length!=0) {
        _streamerKit.textLabel.text = resultString;
        [_streamerKit updateTextLabel];
    }
    
    
}

- (NSData *)dataFromBuffer:(CMSampleBufferRef )sampleBuffer
{
    NSInteger audioDataSize = CMSampleBufferGetTotalSampleSize(sampleBuffer);
    //sampleBuffer 的size
    CMBlockBufferRef blockBufferRef = CMSampleBufferGetDataBuffer(sampleBuffer);
    //不应取CMBlockBufferRef length
    Byte buffer[audioDataSize];
    CMBlockBufferCopyDataBytes(blockBufferRef, 0, audioDataSize, buffer);
    NSData *data = [NSData dataWithBytes:buffer length:audioDataSize];
    return  data;
}
/**
 parse JSON data
 params,for example：
 {"sn":1,"ls":true,"bg":0,"ed":0,"ws":[{"bg":0,"cw":[{"w":"白日","sc":0}]},{"bg":0,"cw":[{"w":"依山","sc":0}]},{"bg":0,"cw":[{"w":"尽","sc":0}]},{"bg":0,"cw":[{"w":"黄河入海流","sc":0}]},{"bg":0,"cw":[{"w":"。","sc":0}]}]}
 **/
- (NSString *)stringFromJson:(NSString*)params
{
    if (params == NULL) {
        return nil;
    }
    
    NSMutableString *tempStr = [[NSMutableString alloc] init];
    NSDictionary *resultDic  = [NSJSONSerialization JSONObjectWithData:
                                [params dataUsingEncoding:NSUTF8StringEncoding] options:kNilOptions error:nil];
    
    if (resultDic!= nil) {
        NSArray *wordArray = [resultDic objectForKey:@"ws"];
        
        for (int i = 0; i < [wordArray count]; i++) {
            NSDictionary *wsDic = [wordArray objectAtIndex: i];
            NSArray *cwArray = [wsDic objectForKey:@"cw"];
            
            for (int j = 0; j < [cwArray count]; j++) {
                NSDictionary *wDic = [cwArray objectAtIndex:j];
                NSString *str = [wDic objectForKey:@"w"];
                [tempStr appendString: str];
            }
        }
    }
    return tempStr;
}

- (NSString *) getUuid{
    return [[[[UIDevice currentDevice] identifierForVendor] UUIDString] lowercaseString];
}


- (NSString *)getSteamer
{
    
    return [NSString stringWithFormat:@"%@/%@", kStreamerUrl,[[self getUuid]substringToIndex:3 ]];
}

- (NSString *)getPlayerUrl
{
    return [NSString stringWithFormat:@"%@/%@", kPlayerUrl,[[self getUuid]substringToIndex:3 ]];
}



@end
