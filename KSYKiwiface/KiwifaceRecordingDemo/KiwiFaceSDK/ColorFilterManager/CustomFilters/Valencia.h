
#import "KWRenderProtocol.h"
#import <GPUImage/GPUImage.h>
@interface ValenciaFilter : GPUImageThreeInputFilter

@end

@interface Valencia : GPUImageFilterGroup<KWRenderProtocol>
{
    GPUImagePicture *imageSource1;
    GPUImagePicture *imageSource2;
}

@property (nonatomic, readonly) BOOL needTrackData;

@end
