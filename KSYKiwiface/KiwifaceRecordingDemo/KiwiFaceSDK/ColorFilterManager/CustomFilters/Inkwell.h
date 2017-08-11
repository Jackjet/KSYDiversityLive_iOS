#import "KWRenderProtocol.h"
#import <GPUImage/GPUImage.h>

@interface InkwellFilter : GPUImageTwoInputFilter

@end

@interface Inkwell : GPUImageFilterGroup<KWRenderProtocol>
{
    GPUImagePicture *imageSource;
}

@property (nonatomic, readonly) BOOL needTrackData;

@end
