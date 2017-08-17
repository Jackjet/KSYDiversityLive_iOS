//
//  KSYTuVCapture.h
//
//  Created by ksyun.
//  Copyright © 2017 ksyun. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "TuSDKFramework.h"

@interface KSYTuVCapture : NSObject

// 实时相机
@property (nonatomic, readonly) TuSDKLiveVideoCamera *camera;

//开始采集
- (void) startCapture;

//停止采集
- (void) stopCapture;

@end
