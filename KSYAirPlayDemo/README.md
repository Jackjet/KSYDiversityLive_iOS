iOS AirPlay屏幕直播 (Airplay + KSYLive_iOS)

调用第三方的Airplay库来直播屏幕内容的功能, 下面就简单介绍一下如何使用KSYLive_iOS制作一款AirPlay屏幕直播App.

## 简单概括AirPlay屏幕直播使用场景:
1. 玩家开启AirPlay屏幕直播App. 
2. 打开AirPlay开关，在打开开关前，可以修改推流地址.
3. 进入游戏界面, AirPlay屏幕直播APP在后台将屏幕内容和声音直播出去.

从以上使用场景可以看出, 任何一款游戏都可以支持AirPlay屏幕直播App的直播.

另外在直播的过程中, 游戏的主窗口内的全部内容都会被直播出去.

在直播过程中，只有麦克采集的声音可以被直播.

## 基本概念
作为直播App的开发者, 基本不需要关心游戏部分的开发, 而只需要关心直播部分的开发. 没有了以前内置录屏的耦合关系.

### [KSYLive_iOS](https://github.com/ksvc/KSYLive_iOS)
KSYLive_iOS 是一个提供了直播相关的功能的SDK

## 准备工作
1. 创建工程
2. 将Build Phases下Enable Bitcode配置成NO
3. 将Capabilities下Background Modes中Audio，AirPlay，and Picture in Picture选项选中
4. 编辑 Podfile, 添加KSYLive_iOS的依赖, 执行pod install后 改为打开 workspace.

至此, 准备工作就做好了, 可以开始写代码了. 

屏幕直播类XDWScreenRecorder提供了两个接口函数和一组回调函数，两个接口函数用于开启和关闭屏幕直播，回调函数用于处理音频采集开始、结束和接收图像数据.

## 集成工作

1. 添加推流类 KSYGPUStreamerKit, KSYGPUStreamerKit 中主要是保存 KSYStreamerBase, 运行过程中提供对KSYStreamerBase的访问.

2. 配置KSYStreamerBase的一些主要参数，在开启和关闭AirPlay开关的同时分别开启和关闭推流.

3. 在XDWScreenRecorder类的各个回调接口中通过KSYGPUStreamerKit 进行采集和推流

* didStartRecordingWithVideoSize 接口配置推流分辨率和启动音频采集.
* videoBuffer 接口将收到的图像buffer 送入 KSYStreamerBase推流
* screenRecorderDidStopRecording 接口结束音频采集
