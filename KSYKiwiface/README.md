
## iOS 贴纸、大眼瘦脸和美颜功能 （KSYLive_IOS + kiwi）

金山云开放平台，提供SDK全链路数据，可以和容易和第三方数据处理服务商合作。以下是金山直播SDK和[kiwi][kiwi]实现的贴纸功能（人脸识别+贴纸\大眼瘦脸\美颜）。

### 1. 参考资料
金山云直播SDK请参考 https://github.com/ksvc/KSYLive_iOS

开为科技SDK请参考  http://kiwiar.com/

### 2.准备工作

1. 下载kiwi的工程，GPUImage和视频采集采用kiwi的方式

2. 在kiwi工程中添加金山云直播SDK，采用pod的方式加入，如下：

（1）在含有KiwifaceRecordingDemo.xcodeproj的文件夹中新建Podfile文件
终端中进入目标文件
```
cd destination
vim Podfile
```
（2）在Podfile添加pod项目
```
pod 'GPUImage', :path => './'
pod 'libksygpulive/libksygpulive','2.9.0'
```
其中GPUImage是kiwi提供的

### 3. 集成工作

1. 添加金山直播推流类KSYGPUStreamerKit, 运行过程中提供对预览preview和KSYStreamerBase的访问，同时添加音频采集类KSYAUAudioCapture,  控制音频的采集和推流处理.

2. 配置KSYStreamerBase的一些主要参数，添加推流按钮来开启和关闭推流.

3. 添加kiwi视频采集和kiwiSdk，运行过程中提供对视频的采集和kiwiSdk处理.

4. 处理后的视频数据通过willOutputSampleBuffer函数回调出来，推流视频在这个函数里面完成

* sampleBuffer 是视频采集回调出来的数据流，通过renderer添加filter处理，但是处理后的数据需要进行旋转或镜像处理
* capToGpu 将数据流传到gpu上进行推流，方便后续添加水印或是filter的处理

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

* kiwi视频采集后回调

```
/**
@abstract  视频采集后回调视频帧
@param sampleBuffer 视频采集回调数据
*/
- (void)willOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
```

* kiwi视频filter处理

```
/**
@abstract  视频采集后回调视频帧
@param pixelBuffer 视频采集回调数据进行filter处理
@param rotation 数据图像方向
@param isMirrored 数据图像是否需要做镜像处理
*/
- (void)processPixelBuffer:(CVPixelBufferRef)pixelBuffer withRotation:(cv_rotate_type)rotation mirrored:(BOOL)isMirrored;
```
### 5. 商务合作
* 金山云

金山云直播SDK可以免费用于用于商业应用

* kiwi

请登录官网联系kiwi商务：http://kiwiar.com/


[kiwi]:http://kiwiar.com/
