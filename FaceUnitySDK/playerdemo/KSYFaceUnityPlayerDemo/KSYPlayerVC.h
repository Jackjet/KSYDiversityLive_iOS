//
//  FirstViewController.h
//  QYLive
//
//  Created by yiqian on 11/3/15.
//  Copyright (c) 2015 qyvideo. All rights reserved.
//
#import "KSYUIVC.h"
#import "KSYFaceUnityView.h"

@interface KSYPlayerVC : KSYUIVC

- (instancetype)initWithURL:(NSURL *)url fileList:(NSArray *)fileList;

/// FaceUnity贴纸配置页面
@property (nonatomic, readonly) KSYFaceUnityView *ksyFaceUnityView;

@end

