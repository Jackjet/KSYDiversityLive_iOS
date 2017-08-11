//
//  KSYNVCaptureScenceKit.h
//
//  Created by ksyun.
//  Copyright © 2017 ksyun. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <NvsCaptureVideoFx.h>
#import "NvsStreamingContext.h"

@interface KSYNVCapture : NSObject

@property (nonatomic, assign) NvsStreamingContext *context;

@property (nonatomic, readonly) NvsVideoFrameReceiver *videoFrameReceiver;

- (NvsColor) pickColor:(CVPixelBufferRef) pixelBuffer selRect:(CGRect)selRect;

- (void)setCaptureSceneColor:(NvsColor)color;

-(void)startNVCap:(unsigned int)cameraPosition;

- (void)selectScene:(unsigned int)sceneIndex;

- (void) closeNvCap;

@end
