IOS 贴纸、大眼瘦脸和美颜功能 （KSYLive_IOS + kiwi）
金山云开放平台，提供SDK全链路数据，可以和容易和第三方数据处理服务商合作。以下是金山直播SDK和kiwi实现的贴纸功能（人脸识别+贴纸\大眼瘦脸\美颜）。

## 参考资料
金山云直播SDK请参考 https://github.com/ksvc/KSYLive_iOS

开为科技SDK请参考https://github.com/kiwi-face/KiwiFaceSDK_Demo_iOS

## 准备工作

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
pod 'libksygpulive/libksygpulive'
```
其中GPUImage是kiwi提供的

## 集成工作

1. 添加推流类KSYStreamerBase, 运行过程中提供对KSYStreamerBase的访问，同时添加音频采集类KSYAUAudioCapture,  控制音频的采集和推流处理.

2. 配置KSYStreamerBase的一些主要参数，添加推流按钮来开启和关闭推流.

3. 处理后的视频数据通过willOutputSampleBuffer函数回调出来，推流视频在这个函数里面完成

* sampleBuffer 是加filter后的数据流，但是直接出来的数据需要进行旋转或镜像处理
* processVideoPixelBuffer:timeInfo 是推流函数，将旋转或镜像处理后视频数据推到观众端

## 接口介绍

* 音频数据处理

```
/**
@abstract 处理一段音频数据
@param sampleBuffer Buffer to process
@discussion 应当在开始推流前定期调用此接口，与processVideoSampleBuffer 交错进行
*/
- (void)processAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer;
```

* 视频数据处理

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
