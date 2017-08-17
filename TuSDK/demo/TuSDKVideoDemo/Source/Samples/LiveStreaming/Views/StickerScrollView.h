//
//  StickerScrollView.h
//  TuSDKVideoDemo
//
//  Created by tutu on 2017/3/10.
//  Copyright © 2017年 TuSDK. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TuSDKFramework.h"


@protocol StickerViewClickDelegate <NSObject>

/**
 点击新的贴纸组的代理方法

 @param stickGroup 贴纸组对象；
 */
- (void)clickStickerViewWith:(TuSDKPFStickerGroup *)stickGroup;

@end




@interface StickerScrollView : UIView

//贴纸栏事件代理
@property (nonatomic, assign) id<StickerViewClickDelegate> stickerDelegate;

@end
