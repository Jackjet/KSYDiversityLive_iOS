//
//  KWVideoShowViewController.h
//  KiwifaceRecordingDemo
//
//  Created by zhaoyichao on 2017/2/4.
//  Copyright © 2017年 kiwiFaceSDK. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "KWSDK_UI.h"
#import "KSYPresetCfgView.h"

@interface KWVideoShowViewController : UIViewController
/**
 *  推流配置页面
 */
@property (nonatomic, strong) KSYPresetCfgView *cfgview;

/**
 sdk UI action object
 */
@property (nonatomic, strong) KWSDK_UI *kwSdkUI;

@property (nonatomic,copy)NSString *modelPath;

@end
