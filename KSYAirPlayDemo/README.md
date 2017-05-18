## 金山云iOS手游录屏直播方案（付费版本）
当前手游录屏直播有两个方案：
1. [ReplayKit][ReplayKit] 方案中，已经提供了完整的[ReplayKit][ReplayKit]的调用代码，使用免费。
1. [Airplay][Airplay] 方案中，基于Airplay录屏部分由[xindawn][xindawn]提供，其余音频采集、编码、推流由[金山云直播SDK][libksygpulive]提供。需要向[xindawn][xindawn]付费，金山云不收取任何费用。

优缺点：
1. [ReplayKit][ReplayKit]需要游戏支持，[Airplay][Airplay]对录屏对象没有要求。
1. [ReplayKit][ReplayKit]最低支持iOS 10，[Airplay][Airplay]最低支持i0S 7，适配性更强。
1. [ReplayKit][ReplayKit]免费，[Airplay][Airplay]收费。
1. [ReplayKit][ReplayKit]可以上架AppStore，[Airplay][Airplay]只能通过企业帐号发布。

### 1. 简单概括AirPlay屏幕直播使用场景:
调用[xindawn][xindawn]提供的Airplay库来采集屏幕内容, 基于[libksygpulive][libksygpulive]制作一款AirPlay屏幕直播App，大致过程：

1. 玩家开启AirPlay屏幕直播App. 
2. 打开AirPlay开关，在打开开关前，可以修改推流地址.
3. 进入游戏界面, AirPlay屏幕直播APP在后台将屏幕内容和声音直播出去.

从以上使用场景可以看出, 任何一款游戏都可以支持AirPlay屏幕直播App的直播.

另外在直播的过程中, 游戏的主窗口内的全部内容都会被直播出去.

在直播过程中，只有麦克采集的声音可以被直播出去（如果选择通过AirPlay发送音频，将导致玩家无法听到任何声音）.

### 2. 基本概念
作为直播App的开发者, 基本不需要关心游戏部分的开发, 而只需要关心直播部分的开发. 没有了以前内置录屏[ReplayKit][ReplayKit]的耦合关系.

#### 2.1 [libksygpulive][libksygpulive]
[libksygpulive][libksygpulive]提供了直播相关的功能的SDK，包括摄像头采集、视频处理、音频采集、音频处理，编码和RTMP推流。

#### 2.2 [xindawn][xindawn]
[xindawn][xindawn]提供了Airplay Server功能，接收iPhone Airplay发送的音视频数据。[xindawn][xindawn]将数据转送到[libksygpulive][libksygpulive]，角色类似KSYAVFCapture。[libksygpulive][libksygpulive]系统框图请见[wiki](https://github.com/ksvc/KSYLive_iOS/wiki)。

### 3. 准备工作
1. 创建工程
2. 将Build Phases下Enable Bitcode配置成NO
3. 将Capabilities下Background Modes中Audio，AirPlay，and Picture in Picture选项选中
4. 编辑 Podfile, 添加KSYLive_iOS的依赖, 执行pod install后 改为打开 workspace.

至此, 准备工作就做好了, 可以开始写代码了. 

屏幕直播类XDWScreenRecorder提供了两个接口函数和一组回调函数，两个接口函数用于开启和关闭屏幕直播，回调函数用于处理音频采集开始、结束和接收图像数据.

### 4. 集成工作

1. 添加推流类 KSYGPUStreamerKit, KSYGPUStreamerKit 中主要是保存 KSYStreamerBase, 运行过程中提供对KSYStreamerBase的访问.

2. 配置KSYStreamerBase的一些主要参数，在开启和关闭AirPlay开关的同时分别开启和关闭推流.

3. 在XDWScreenRecorder类的各个回调接口中通过KSYGPUStreamerKit 进行采集和推流

* didStartRecordingWithVideoSize 接口配置推流分辨率和启动音频采集.
* videoBuffer 接口将收到的图像buffer 送入 KSYStreamerBase推流
* screenRecorderDidStopRecording 接口结束音频采集

[xindawn]:http://xindawn.com/
[libksygpulive]:https://github.com/ksvc/KSYLive_iOS
[ReplayKit]:https://github.com/ksvc/KSYDiversityLive_iOS/tree/master/KSYReplayKit
[Airplay]:https://github.com/ksvc/KSYDiversityLive_iOS/edit/master/KSYAirPlayDemo
