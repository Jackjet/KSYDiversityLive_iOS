#import "KSYFaceUnityWrapper.h"
#include <sys/mman.h>
#include <sys/stat.h>
#import "FURenderer.h"
#import "authpack.h"

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

@interface KSYFaceUnityWrapper()
{
    int _lastItem;
    
    // items' bundle name
    NSArray *itemArray;
    
    NSLock *_lock;
    
    int curItems[3];
    
    NSArray *filterArray;
}

@property EAGLContext* gl_context;

@property dispatch_queue_t queue;
@end

@implementation KSYFaceUnityWrapper

@synthesize cheekThinning=_cheekThinning;
@synthesize eyeEnlarging=_eyeEnlarging;
@synthesize colorLevel=_colorLevel;
@synthesize blurLevel=_blurLevel;
@synthesize filterIndex = _filterIndex;

- (id) init {
    return [self initWithArray:nil];
}

-(id) initWithArray:(NSArray *) items
{
    if(self)
    {
        _queue = dispatch_queue_create("com.ksyun.faceunity.queue", DISPATCH_QUEUE_SERIAL);
        
        itemArray = [items copy];
        
        [self initFaceUnity];

        if (curItems[0] == 0) {
            self.stickerIndex = 1;
        }

        if (curItems[1] == 0) {
            [self loadFilter];
        }
        
        if (curItems[2] == 0) {
            [self loadHeart];
        }
        
        filterArray = [[NSArray alloc] initWithObjects:@"nature", @"delta", @"electric", @"slowlived", @"tokyo", @"warm", nil];
    }
    return self;
}

-(void)dealloc{
    _lock = nil;
    [FURenderer destroyAllItems];
    for (int i = 0; i < sizeof(curItems) / sizeof(int); i++) {
        curItems[i] = 0;
    }}

#pragma mark - load sticker
-(void)loadItem:(NSString *)itemName{
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

#pragma mark - FaceUnity init/render
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

-(void)setBeautifyFilter
{
    // 滤镜 "nature", "delta", "electric", "slowlived", "tokyo", "warm"
    [FURenderer itemSetParam:curItems[1] withName:@"filter_name" value:@([[filterArray objectAtIndex:_filterIndex] UTF8String])];
    // 瘦脸 （大于等于0的浮点数，0为关闭效果，1为默认效果，大于1为进一步增强效果）
    [FURenderer itemSetParam:curItems[1] withName:@"cheek_thinning" value:@(_cheekThinning)];
    // 大眼 （大于等于0的浮点数，0为关闭效果，1为默认效果，大于1为进一步增强效果）
    [FURenderer itemSetParam:curItems[1] withName:@"eye_enlarging" value:@(_eyeEnlarging)];
    // 美白 （大于等于0的浮点数，0为关闭效果，1为默认效果，大于1为进一步增强效果）
    [FURenderer itemSetParam:curItems[1] withName:@"color_level" value:@(_colorLevel)];
    // 磨皮 取值范围为0-6
    [FURenderer itemSetParam:curItems[1] withName:@"blur_level" value:@(_blurLevel)];
}

-(void)renderFaceUnity:(CMSampleBufferRef)inSampleBuffer
{
    CVPixelBufferRef img = CMSampleBufferGetImageBuffer(inSampleBuffer);
    dispatch_sync(self.queue, ^{
        if(![EAGLContext setCurrentContext:_gl_context]){
            NSLog(@"faceunity: failed to create / set a GLES2 context");
        }
        [self setBeautifyFilter];
        
        // itemCount
        CVPixelBufferRef output_pixelBuffer = [[FURenderer shareRenderer] renderPixelBuffer:img withFrameId:0 items:curItems itemCount:3 flipx:YES];
        CMSampleTimingInfo timingInfo;
        CMSampleBufferGetSampleTimingInfo(inSampleBuffer, 0, &timingInfo);

        CMSampleBufferRef sampleBuffer;
        CMSampleBufferCreateForImageBuffer(kCFAllocatorDefault, output_pixelBuffer, YES, NULL, NULL,
                                           CMSampleBufferGetFormatDescription(inSampleBuffer),
                                           &timingInfo, &sampleBuffer);
        
        if(_videoProcessingCallback)
            _videoProcessingCallback(sampleBuffer);
        
        CFRelease(sampleBuffer);
    });
    return ;
}

#pragma mark - property
- (void)setStickerIndex:(int)stickerIndex{
    if (_stickerIndex == stickerIndex) {
        return;
    }
    _stickerIndex = stickerIndex;
    
    [self loadItem:itemArray[stickerIndex]];
}

@end
