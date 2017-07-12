#import "KSYFaceunityFilter.h"
#include <sys/mman.h>
#include <sys/stat.h>
#import "FURenderer.h"
#import "authpack.h"

static int g_frame_id = 0;

static int osal_GetFileSize(int fd){
    struct stat sb;
    sb.st_size = 0;
    fstat(fd, &sb);
    return (int)sb.st_size;
}
static void* mmap_bundle(NSString* fn_bundle,int* psize){
    // Load item from predefined item bundle
    NSString *path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:fn_bundle];
    //    path = [[NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject]stringByAppendingPathComponent:fn_bundle];
    const char *fn = [path UTF8String];
    int fd = open(fn,O_RDONLY);
    void* g_res_zip = NULL;
    int g_res_size = 0;
    if(fd == -1){
        NSLog(@"faceunity: failed to open bundle");
        g_res_size = 0;
    }else{
        g_res_size = osal_GetFileSize(fd);
        g_res_zip = mmap(NULL, g_res_size, PROT_READ, MAP_SHARED, fd, 0);
        NSLog(@"faceunity: %@ mapped %08x %d\n", path, (unsigned int)g_res_zip, g_res_size);
        close(fd);
    }
    *psize = g_res_size;
    return g_res_zip;
}


@interface KSYFaceunityFilter()
{
    
    // items' bundle name
    NSArray *itemArray;
    
    int curItems[3];
}

@property KSYGPUPicOutput* pipOut;
@property EAGLContext* gl_context;

@property dispatch_queue_t queue;
@end

@implementation KSYFaceunityFilter


-(id) initWithArray:(NSArray *) items
{
    self = [super initWithFmt:kCVPixelFormatType_32BGRA];
    
    if(self)
    {
        _queue = dispatch_queue_create("com.ksyun.faceunity.queue", DISPATCH_QUEUE_SERIAL);

        itemArray = [items copy];
        
        [self initFaceUnity];
        
        [self loadItem:items.firstObject];
        
        [self loadFilter];
        
        [self loadHeart];
        
        _pipOut = [[KSYGPUPicOutput alloc]initWithOutFmt:kCVPixelFormatType_32BGRA];
        __weak KSYFaceunityFilter *weak_filter = self;
        _pipOut.videoProcessingCallback = ^(CVPixelBufferRef pixelBuffer, CMTime timeInfo){
            [weak_filter renderFaceUnity:pixelBuffer timeInfo:timeInfo];
        };
    }
    return self;
}

#pragma -Faceunity Load Data
- (void)loadItem:(NSString *)itemName
{
    dispatch_sync(self.queue, ^{
        if(![EAGLContext setCurrentContext:_gl_context]){
            NSLog(@"faceunity: failed to create / set a GLES2 context");
        }
        int size;
        void* data = mmap_bundle(itemName, &size);
        
        int itemHandle = [FURenderer createItemFromPackage:data size:size];
        
        if (curItems[0] != 0) {
            NSLog(@"faceunity: destroy item");
            [FURenderer destroyItem:curItems[0]];
        }
        
        curItems[0] = itemHandle;
    });
}

- (id) init {
    return [self initWithArray:nil];
}

-(void)dealloc{
    _pipOut = nil;
    [FURenderer destroyAllItems];
    for (int i = 0; i < sizeof(curItems) / sizeof(int); i++) {
        curItems[i] = 0;
    }
}

#pragma mark - load filter
- (void)loadFilter
{
    int size = 0;
    
    void *data = mmap_bundle(@"face_beautification.bundle", &size);
    
    curItems[1] = [FURenderer createItemFromPackage:data size:size];
}

#pragma mark - load filter
- (void)loadHeart{
    int size = 0;
    
    void *data = mmap_bundle(@"heart.bundle", &size);
    
    curItems[2] = fuCreateItemFromPackage(data, (int)size);
}

#pragma mark - Render CallBack
-(void)renderFaceUnity:(CVPixelBufferRef)pixelBuffer
              timeInfo:(CMTime)timeInfo
{
    dispatch_sync(self.queue, ^{
        if(![EAGLContext setCurrentContext:_gl_context]){
            NSLog(@"faceunity: failed to create / set a GLES2 context");
        }
        /*设置美颜效果（滤镜、磨皮、美白、瘦脸、大眼....）*/
        // 瘦脸 （大于等于0的浮点数，0为关闭效果，1为默认效果，大于1为进一步增强效果）
        [FURenderer itemSetParam:curItems[1] withName:@"cheek_thinning" value:@(1.0)]; //瘦脸
        // 大眼 （大于等于0的浮点数，0为关闭效果，1为默认效果，大于1为进一步增强效果）
        [FURenderer itemSetParam:curItems[1] withName:@"eye_enlarging" value:@(1.0)]; //大眼
        // 美白 （大于等于0的浮点数，0为关闭效果，1为默认效果，大于1为进一步增强效果）
        [FURenderer itemSetParam:curItems[1] withName:@"color_level" value:@(0.5)]; //美白
        // 滤镜 "nature", "delta", "electric", "slowlived", "tokyo", "warm"
        [FURenderer itemSetParam:curItems[1] withName:@"filter_name" value:@"nature"]; //滤镜
        // 磨皮 取值范围为0-6
        [FURenderer itemSetParam:curItems[1] withName:@"blur_level" value:@(5.0)]; //磨皮
        [FURenderer itemSetParam:curItems[1] withName:@"face_shape" value:@(3)]; //瘦脸类型
        [FURenderer itemSetParam:curItems[1] withName:@"face_shape_level" value:@(0.5)]; //瘦脸等级
        [FURenderer itemSetParam:curItems[1] withName:@"red_level" value:@(0.5)]; //红润
        
        CVPixelBufferRef output_pixelBuffer = [[FURenderer shareRenderer] renderPixelBuffer:pixelBuffer withFrameId:g_frame_id items:curItems itemCount:3 flipx:YES];//flipx 参数设为YES可以使道具做水平方向的镜像翻转
        g_frame_id += 1;
        [self processPixelBuffer:output_pixelBuffer time:timeInfo];
    });
}

-(void)initFaceUnity
{
    _gl_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if(![EAGLContext setCurrentContext:_gl_context]){
        NSLog(@"faceunity: failed to create / set a GLES2 context");
    }
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        int size = 0;
        void* v3data = mmap_bundle(@"v3.bundle", &size);
        [[FURenderer shareRenderer] setupWithData:v3data ardata:NULL authPackage:&g_auth_package authSize:sizeof(g_auth_package) shouldCreateContext:YES];
    });
}

#pragma GPUImageInput
- (void)newFrameReadyAtTime:(CMTime)frameTime atIndex:(NSInteger)textureIndex {
    [_pipOut newFrameReadyAtTime:frameTime atIndex:textureIndex];
}

- (void)setInputFramebuffer:(GPUImageFramebuffer *)newInputFramebuffer atIndex:(NSInteger)textureIndex {
    [_pipOut setInputFramebuffer:newInputFramebuffer atIndex:textureIndex];
}

- (NSInteger)nextAvailableTextureIndex {
    return 0;
}

- (void)setInputSize:(CGSize)newSize atIndex:(NSInteger)textureIndex {
    [_pipOut setInputSize:newSize atIndex:textureIndex];
}

- (void)setInputRotation:(GPUImageRotationMode)newInputRotation
                 atIndex:(NSInteger)textureIndex {
    [_pipOut setInputRotation:newInputRotation atIndex:textureIndex];
}

- (GPUImageRotationMode)  getInputRotation {
    return [_pipOut getInputRotation];
}

- (CGSize)maximumOutputSize {
    return [_pipOut maximumOutputSize];
}

- (void)endProcessing {
    
}
- (BOOL)shouldIgnoreUpdatesToThisTarget {
    return NO;
}

- (BOOL)wantsMonochromeInput {
    return NO;
}
- (void)setCurrentlyReceivingMonochromeInput:(BOOL)newValue {
    
}

- (void)setChoosedIndex:(NSInteger)choosedIndex{
    if (_choosedIndex == choosedIndex) {
        return;
    }
    _choosedIndex = choosedIndex;
    
    [self loadItem:itemArray[choosedIndex]];
}

@end
