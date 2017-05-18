//
//  XDWScreenRecorder.h
//  XDWScreenRecorder
//
//  Created by zangyanan on 16/12/18.
//  Copyright © 2016年 xindawn. All rights reserved.
//



#import <Foundation/Foundation.h>
#import "XDWScreenRecorderConfig.h"

FOUNDATION_EXTERN NSString * XDWScreenRecorderErrorDomain;

typedef NS_ENUM(NSUInteger, XDWScreenRecorderErrorCode) {
    /// AirPlay端口冲突
    XDWScreenRecorderErrorCodePortConflict,
    /// 网络未连接
    XDWScreenRecorderErrorCodeNetworkDisconnection,
    /// 连接AirPlay超时
    XDWScreenRecorderErrorCodeAirPlaySelectTimeout,
    /// 其他错误
    XDWScreenRecorderErrorCodeOther
};

NS_ASSUME_NONNULL_BEGIN

@protocol XDWScreenRecorderDelegate;

@interface XDWScreenRecorder : NSObject

@property(nonatomic, strong, readonly) XDWScreenRecorderConfig *config;
@property(nonatomic, weak) id<XDWScreenRecorderDelegate> delegate;

- (nullable instancetype)init UNAVAILABLE_ATTRIBUTE;
+ (nullable instancetype)new UNAVAILABLE_ATTRIBUTE;

- (instancetype)initWithConfig:(XDWScreenRecorderConfig *)config;
- (void)start;
- (void)stop;

@end

@protocol XDWScreenRecorderDelegate <NSObject>
@optional
- (void)screenRecorder:(XDWScreenRecorder *)screenRecorder didStartRecordingWithVideoSize:(CGSize)videoSize;
- (void)screenRecorder:(XDWScreenRecorder *)screenRecorder startError:(NSError *)error;
- (void)screenRecorder:(XDWScreenRecorder *)screenRecorder videoBuffer:(CVPixelBufferRef)buffer timestamp:(NSTimeInterval)timestamp;
- (void)screenRecorderDidStopRecording:(XDWScreenRecorder *)screenRecorder;
@end

NS_ASSUME_NONNULL_END
