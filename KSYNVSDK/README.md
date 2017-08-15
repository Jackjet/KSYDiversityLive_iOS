## IOS 背景替换推流功能 （KSYLive_IOS + 美摄视频采集和抠图）

金山云开放平台，提供SDK全链路数据，可以和容易和第三方数据处理服务商合作。以下是金山直播SDK和美摄SDK实现的背景替换推流功能。

### 1.参考资料
金山云直播SDK请参考 https://github.com/ksvc/KSYLive_iOS

美摄SDK请参考https://www.meishesdk.com/doc

### 2.准备工作

1. 集成美摄SDK，下载SDK，将文件夹lib和文件夹include文件拷贝到自己的项目中，参考capturescene工程集成背景替换

2. 集成金山云SDK，采用pod的方式加入，如下：

（1）在含有KSYNVStreamer.xcodeproj的文件夹中新建Podfile文件
终端中进入目标文件
```
cd destination
vim Podfile
```
（2）在Podfile添加pod项目
```
pod 'libksygpulive/libksygpulive'
```

### 3. 集成工作

1. 添加金山直播推流类KSYNVGPUStreamerKit, 运行过程中提供对预览preview和推流streamerBase的访问，同时添加音频采集类KSYAUAudioCapture,  控制音频的采集和推流处理.

2. 配置KSYStreamerBase的一些主要参数，添加推流按钮来开启和关闭推流.

3. 添加美摄视频采集和背景替换类KSYNVCapture，运行过程中提供对视频的采集和背景替换.

4. 处理后的视频数据通过didVideoFrameReceived函数回调出来，推流视频在这个函数里面完成

* pixelBuffer 是背景替换后的数据流
* capToGpu 将数据流传到gpu上，方便后续添加水印或是filter的处理

### 4. 接口介绍

* 金山推流音频数据

```
/**
@abstract 处理一段音频数据
@param sampleBuffer Buffer to process
@discussion 应当在开始推流前定期调用此接口，与processVideoSampleBuffer 交错进行
*/
- (void)processAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer;
```

* 金山推流视频数据

```
/**
@abstract  处理一个视频帧
@param pixelBuffer 待编码的像素数据
@param timeStamp   待编码的时间戳
@discussion 应当在开始推流前定期调用此接口，比如按照采集帧率调用
@discussion 支持的图像格式包括: BGR0,NV12,YUVA444P,YUV420P
*/
- (void)processVideoPixelBuffer:(CVPixelBufferRef)pixelBuffer
timeInfo:(CMTime)timeStamp;
```

* 美摄背景替换

```
/*!
*  \brief 对采集设备应用拍摄场景资源包
*  \param captureSceneId 拍摄场景资源包的ID
*  \return 返回BOOL值。值为YES则应用成功，NO则失败。
*  \since 1.2.0
*  \sa getCurrentCaptureSceneId
*  \sa removeCurrentCaptureScene
*/
- (BOOL)applyCaptureScene:(NSString *)captureSceneId;
```

* 美摄采集视频后回调

```
/*!
*  \brief 通知接收者一个视频帧已经渲染完成
注意：这个回调函数是在引擎的某个线程调用的，并非主线程！
*  \param receiver 该回调接口对应的NvsVideoFrameReceiver对象
*  \param pixelBuffer 视频帧的CVPixelBuffer对象
*  \param videoSize 视频帧的尺寸，单位为像素
*  \param timelinePos 视频帧的时间戳，单位为微秒
*/
- (void)didVideoFrameReceived:(NvsVideoFrameReceiver *)receiver
pixelBuffer:(CVPixelBufferRef)pixelBuffer
videoSize:(CGSize)videoSize
timelinePos:(int64_t)timelinePos;
```

### 5. 商务合作
* 金山云

金山云直播SDK可以免费用于用于商业应用

* 美摄

请登录官网联系美摄商务：https://www.meishesdk.com/aboutus


[kiwi]:https://www.meishesdk.com/aboutus
