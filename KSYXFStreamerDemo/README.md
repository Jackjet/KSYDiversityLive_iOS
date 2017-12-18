## iOS 金山云 - 讯飞语言听写示例

金山云开放平台，提供SDK全链路数据，可以和容易和第三方数据处理服务商合作。通过金山直播SDK和讯飞SDK,可在推流的同时，使用语言听写功能，并将听写内容作为字幕叠加到推流画面上的功能。


### 1.参考资料
金山云直播SDK请参考 https://github.com/ksvc/KSYLive_iOS

美摄SDK请参考http://www.xfyun.cn/

### 2.准备工作

1. 集成讯飞SDK，下载SDK，将文件夹lib和文件夹iflyMSC.framework文件拷贝到自己的项目中，参考讯飞工程集成

2. 集成金山云SDK，采用pod的方式加入，如下：

（1）在含有KSYXFStreamerDemo.xcodeproj的文件夹中新建Podfile文件
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

1. 添加金山直播推流类KSYGPUStreamerKit, 运行过程中提供对预览preview和推流streamerBase的访问， 控制音频的采集和推流处理.

2. 配置KSYStreamerBase的一些主要参数，添加推流按钮来开启和关闭推流.

3. 处理audioProcessingCallback回调的数据转化为pcm数据，供讯飞语音识别转化为文字.

4. AVAudioSession+KSYDemoAudioSession.h 解决一边识别一边返回结果，然后把返回结果转化文字.

5. 文字设置可参考kit的textLabel的实现，实现自定义样式试图.


### 4. 接口介绍

* 摄像头采集音频数据

```
/**
@abstract 音频CMSampleBufferRef 转化 NSData（pcm） 数据
@param sampleBuffer 为 audioProcessingCallback 回调的音频数据
@discussion 
*/
- (NSData *)dataFromBuffer:(CMSampleBufferRef )sampleBuffer;
```


* 讯飞写入pcm

```
/*!
*  写入音频流
*
*  此方法的使用示例如下:
* <pre><code>[_iFlySpeechRecognizer setParameter:@"-1" value:@"audio_source"];
* [_iFlySpeechRecognizer startListening];
* [_iFlySpeechRecognizer writeAudio:audioData1];
* [_iFlySpeechRecognizer writeAudio:audioData2];
* ...
* [_iFlySpeechRecognizer stopListening];
* </code></pre>
*
*  @param audioData 音频数据
*
*  @return 写入成功返回YES，写入失败返回NO
*/
- (BOOL) writeAudio:(NSData *) audioData;
```

* IFlySpeechRecognizerDelegate 代理方法处理文字结果(具体使用，请参考讯飞文档)

```
- (void) onResults:(NSArray *) results isLast:(BOOL)isLast
```

* 水印处理 

```
         _streamerKit.textLabel.text = resultString;
        [_streamerKit updateTextLabel];
```
* 推流

```
/**
 @abstract 启动推流 （step2）
 @param      url 目标地址
 @discussion 实现直播功能时, url为 rtmp 服务器地址 “rtmp://xxx.xx/appname/streamKey"
 @discussion 设置完成推流参数之后，将媒体流推送到 publishURL 对应的地址
 @discussion 实现本地录制功能时, url为本地文件地址 "/var/xxx/xx.mp4"
 @discussion 本地录制支持mp4和flv两种输出格式, 通过url的文件后缀指定
 @discussion 推流参数主要是视频编码器，音视频码率的设置
 @see hostURL, videoCodec,videokBPS,audiokBPS
 */
- (void) startStream: (NSURL*)     url;
```
### 5. 联系方式
* 金山云

- 主页：[金山云](http://v.ksyun.com)
- 邮箱：<zengfanping@kingsoft.com>
- QQ讨论群：574179720
- Issues: <https://github.com/ksvc/KSYDiversityLive_iOS/issues>

[KSYLive_iOS]:https://github.com/ksvc/KSYLive_iOS

