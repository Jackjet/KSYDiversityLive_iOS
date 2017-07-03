## 播放端集成FaceUnity说明

在播放端集成动态贴纸，可增加观众端的趣味性，且每个观众可自行调节美颜、滤镜以及贴纸效果，高度个性定制化。

* demo位于https://github.com/ksvc/KSYDiversityLive_iOS/tree/master/FaceUnitySDK/playerdemo


### 1. 集成方法

在播放端集成FaceUnity贴纸，依赖项包括:

* [KSYMediaPlayer SDK](https://github.com/ksvc/KSYMediaPlayer_iOS)
* FaceUnity SDK
* [GPUImage](https://github.com/BradLarson/GPUImage)
* FaceUnitySDK/prebuilt/目录下的头文件和库，定义了KSYGPUPicInput和KSYGPUView类。**如果您的应用中集成的不是单独的播放版SDK，而是含有推流功能的[融合版SDK](https://github.com/ksvc/KSYLive_iOS), 这种情况下不需要该依赖项，它们已经包含在融合版中**

推荐使用Pod来管理上述依赖项，具体可参考playerdemo中的Podfile文件。

### 2. 示例代码

播放器集成FaceUnity贴纸的主要思路如下，详细的代码请参考demo：

1. 使用播放器[videoDataBlock](https://ksvc.github.io/KSYMediaPlayer_iOS/doc/html/Classes/KSYMoviePlayerController.html#//api/name/videoDataBlock)方法，获得原始YUV数据;
2. 将原始数据送入FaceUnity SDK进行处理;
3. 获得处理后数据，进行渲染

> 注意：集成动态贴纸时，不再通过播放器内部进行渲染，因此暂时不支持画面旋转和镜像功能

#### 2.1 创建渲染的相关对象

```
//创建KSYGPUPicInput对象
KSYGPUPicInput *rawdataInput =  [[KSYGPUPicInput alloc] initWithFmt:kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange];

//创建渲染KSYGPUView对象，并通过addSubView添加到要显示画面的UIView上
KSYGPUView *gpuimageView = [[KSYGPUView alloc] init];
[gpuimageView setFrame:videoView.bounds];
[videoView addSubview:gpuimageView];

//将KSYGPUView对象添加到KSYGPUPicInput对象上
[rawdataInput addTarget:gpuimageView];
```

#### 2.2 创建FaceUnity封装对象

```
KSYFaceUnityWrapper *stickerFilter = [[KSYFaceUnityWrapper alloc]initWithArray:item_names];
    
//经过FaceUnity贴纸处理后的数据直接送入KSYGPUPicInput
stickerFilter.videoProcessingCallback = ^(CMSampleBufferRef sampleBuffer){
     [rawdataInput processSampleBuffer:sampleBuffer];
};

```

#### 2.3 播放器设置
由于集成贴纸时采用的是外部渲染的方式，因此需要将播放器的shouldHideVideo属性设置为YES，并且播放器的view对象，不再需要添加到要显示画面的UIView上

```
KSYMoviePlayerController *player = [[KSYMoviePlayerController alloc] initWithContentURL: aURL fileList:fileList sharegroup:nil];

//集成贴纸时不需要播放器内部渲染
player.shouldHideVideo = YES;

//播放返回的数据送入FaceUnity SDK进行处理
player.videoDataBlock = ^(CMSampleBufferRef sampleBuffer){
   [stickerFilter renderFaceUnity:sampleBuffer];
};    
```
