//
//  Freud.h
//  KwSDK
//
//  Created by 伍科 on 17/5/18.
//  Copyright © 2017年 PLMediaStreamingSDK. All rights reserved.
//

#import <GPUImage/GPUImage.h>
#import "KWRenderProtocol.h"
#import <GPUImage/GPUImage.h>

@interface FreudFilter : GPUImageTwoInputFilter

@end

@interface Freud : GPUImageFilterGroup<KWRenderProtocol>
{
    GPUImagePicture *imageSource;
}

@property (nonatomic, readonly) BOOL needTrackData;

@end
