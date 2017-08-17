//
//  LiveStreamingViewController.h
//  TuSDKLiveEngineDemo
//
//  Created by TuSDK on 2017/3/20.
//  Copyright © 2017年 TuSDK. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TuSDKFramework.h"
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>
#import "KSYPresetCfgView.h"

@interface LiveStreamingViewController : UIViewController

/**
 *  推流配置页面
 */
@property (nonatomic, strong) KSYPresetCfgView *cfgview;

/**
 *  控制按钮
 */
@property (readonly, nonatomic) UIButton *mActionButton;

/**
 *  滤镜按钮
 */
@property (readonly, nonatomic) UIButton *mFilterButton;

/**
 *  贴纸按钮
 */
@property (readonly, nonatomic) UIButton *mStickerButton;

/**
 *  切换前后摄像头
 */
@property (readonly, nonatomic) UIButton *mToggleCameraButton;

/**
 *  闪光灯
 */
@property (readonly, nonatomic) UIButton *mFlashButton;

/**
 *  关闭按钮
 */
@property (nonatomic, readonly) UIButton *mCloseButton;

/**
 *  闪光灯索引
 */
@property (nonatomic, readwrite) NSInteger flashModeIndex;

/**
 *  视频队列
 */
@property (nonatomic, strong) dispatch_queue_t sessionQueue;

/**
 *  构建界面元素
 */
- (void)lsqInitView;

/**
 *  更新操作按钮
 *
 *  @param isRunning 是否直播中
 */
- (void)updateShowStatus:(BOOL)isRunning;

/**
 *  更新闪光灯模式
 *
 *  @param mode
 */
- (void)updateFlashModeStatus;

/**
 *  点击事件
 *
 *  @param sender
 */
- (void)onConfigButtonClicked:(id)sender;

/**
 点击底部的运行按钮
 
 @param sender 点击对象
 */
- (void)onActionHandle:(id)sender;

/**
 切换滤镜
 
 @param code 滤镜代号
 */
- (void)switchFilter:(NSString *)code;
@end

