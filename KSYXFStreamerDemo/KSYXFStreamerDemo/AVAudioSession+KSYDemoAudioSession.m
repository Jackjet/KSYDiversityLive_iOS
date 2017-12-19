//
//  AVAudioSession+KSYDemoAudioSession.m
//  KSYiflyMSCDemo
//
//  Created by yuyang on 2017/12/18.
//  Copyright © 2017年 Ksyun. All rights reserved.
//

#import "AVAudioSession+KSYDemoAudioSession.h"
// 一边识别一边返回结果，然后把返回结果转化文字，通过金山的sdk加水印推流出去。
// 解决下面的错误：[avas] AVAudioSession.mm:1074:-[AVAudioSession setActive:withOptions:error:]: Deactivating an audio session that has running I/O. All I/O should be stopped or paused prior to deactivating the audio session.
@implementation AVAudioSession (KSYDemoAudioSession)
- (BOOL)setActive:(BOOL)active withOptions:(AVAudioSessionSetActiveOptions)options error:(NSError **)outError NS_AVAILABLE_IOS(6_0){
    
    return YES; //默认是 NO，修改成 YES
    
}
@end
