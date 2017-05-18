//
//  XDWScreenRecorderConfig.h
//  XDWScreenRecorder
//
//  Created by zangyanan on 16/12/18.
//  Copyright © 2016年 xindawn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface XDWScreenRecorderConfig : NSObject
/// AirPlay的名字
@property(nonatomic, assign) char *airPlayName;
/// 接收设备的最佳显示宽度和高度，不低于屏幕的宽度和高度
@property(nonatomic, assign) CGSize videoSize;
/// 希望接收到ios发送端的视频帧率
@property(nonatomic, assign) int framerate;
/// 设置airtunes 服务的监听端口，这个端口监听音乐和屏幕镜像服务
@property(nonatomic, assign) int airTunesPort;
/// 设置视频数据的接收端口，默认是7100,当有多个airplay服务的应用程序运行时，可以设置为其他端口，避免冲突
@property(nonatomic, assign) int airVideoPort;
/// 激活码，免费激活码为”000000000”(9个0),个人用途免费使用
@property(nonatomic, assign) char *activeCode;
/// 默认总是竖屏，是否旋转到横屏(0,90,270)
@property(nonatomic, assign) int autoRotate;
@end
