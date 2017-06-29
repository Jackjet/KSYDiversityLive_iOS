#import "KSYFaceUnityWrapper.h"
#include <sys/mman.h>
#include <sys/stat.h>
#import "FURenderer.h"
#import "authpack.h"

static size_t osal_GetFileSize(int fd){
    struct stat sb;
    sb.st_size = 0;
    fstat(fd, &sb);
    return (size_t)sb.st_size;
}

static void* mmap_bundle(NSString* fn_bundle,intptr_t* psize){
    // Load item from predefined item bundle
    NSString *path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:fn_bundle];
    //    path = [[NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject]stringByAppendingPathComponent:fn_bundle];
    const char *fn = [path UTF8String];
    int fd = open(fn,O_RDONLY);
    void* g_res_zip = NULL;
    size_t g_res_size = 0;
    if(fd == -1){
        NSLog(@"faceunity: failed to open bundle");
        g_res_size = 0;
    }else{
        g_res_size = osal_GetFileSize(fd);
        g_res_zip = mmap(NULL, g_res_size, PROT_READ, MAP_SHARED, fd, 0);
        NSLog(@"faceunity: %@ mapped %08x %ld\n", path, (unsigned int)g_res_zip, g_res_size);
    }
    *psize = g_res_size;
    return g_res_zip;
    return nil;
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
    fuDestroyAllItems();
}

#pragma mark - load sticker
-(void)loadItem:(NSString *)itemName{
    dispatch_sync(self.queue, ^{
        if(![EAGLContext setCurrentContext:_gl_context]){
            NSLog(@"faceunity: failed to create / set a GLES2 context");
        }
        
        intptr_t size;
        void* data = mmap_bundle(itemName, &size);
        int itemId = fuCreateItemFromPackage(data, (int)size);
        
        [_lock lock];
        _lastItem = curItems[0];
        curItems[0] = itemId;
        [_lock unlock];
    });
}

- (void)destroyItem:(int)item {
    dispatch_sync(self.queue, ^{
        fuDestroyItem(item);
    });
}

#pragma mark - load filter
- (void)loadFilter
{
    intptr_t size = 0;
    
    void *data = mmap_bundle(@"face_beautification.bundle", &size);
    
    curItems[1] = fuCreateItemFromPackage(data, (int)size);
}

#pragma mark - load filter
- (void)loadHeart{
    intptr_t size = 0;
    
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
    
    intptr_t size = 0;
    void* v3data = mmap_bundle(@"v3.bundle", &size);
    [[FURenderer shareRenderer] setupWithData:v3data ardata:NULL authPackage:g_auth_package authSize:sizeof(g_auth_package)];
}

-(void)setBeautifyFilter
{
    //设置美颜效果
    // 滤镜 "nature", "delta", "electric", "slowlived", "tokyo", "warm"
    fuItemSetParams(curItems[1], "filter_name", [[filterArray objectAtIndex:_filterIndex] UTF8String]);
    // 瘦脸 （大于等于0的浮点数，0为关闭效果，1为默认效果，大于1为进一步增强效果）
    fuItemSetParamd(curItems[1], "cheek_thinning", _cheekThinning);
    // 大眼 （大于等于0的浮点数，0为关闭效果，1为默认效果，大于1为进一步增强效果）
    fuItemSetParamd(curItems[1], "eye_enlarging", _eyeEnlarging);
    // 美白 （大于等于0的浮点数，0为关闭效果，1为默认效果，大于1为进一步增强效果）
    fuItemSetParamd(curItems[1], "color_level", _colorLevel);
    // 磨皮 取值范围为0-6
    fuItemSetParamd(curItems[1], "blur_level", _blurLevel);
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
        CVPixelBufferRef output_pixelBuffer = [[FURenderer shareRenderer] renderPixelBuffer:img withFrameId:0 items:curItems itemCount:3];
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
    [self destroyItem:_lastItem];
}

@end
