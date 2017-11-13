## 金山云多样化SDK接入
### 1. 目标
[KSYDiversityLive][KSYDiversityLive]基于组件化思路，通过和多种数据处理服务商的联动，共同构建和繁荣移动直播生态链。

金山云SDK通过共享全链路处理能力，开放数据接入处理，实现以下几个方面的多样化：  

1. 功能模式的多样化；
1. 应用场景的多样化；
1. 合作方式的多样化；

### 2. 基础
[KSYDiversityLive][KSYDiversityLive]基于[libksygpulive][libksygpulive]，依托[libksygpulive][libksygpulive]的组件化功能，将金山云长期积累的音视频处理、编码和传输能力分享出来，和第三方组建结合提供更强大的功能。

其中由金山云组合的功能，完成组合工作的所有代码都采用open source形式在此开放。

同时也欢迎从事音视频处理的企业、个人共同参与，一起构建更繁荣的移动直播市场。

### 3. 场景适配
[KSYDiversityLive][KSYDiversityLive]会覆盖越来越多的场景，不仅限于以下。也欢迎大家踊跃提新的适配场景：
* 混响
* 声音增强
* 美颜处理
* 人脸识别
* 视频增强
* 实时音视频通信
* 第三方视频输入

### 4. 落地功能
[KSYDiversityLive][KSYDiversityLive]已经完成了以下功能。同时也欢迎大家提新的功能需求。

* [ReplayKit游戏录屏直播](KSYReplayKit)
* [Airplay录屏直播](KSYAirPlayDemo)
* [应用内录屏](https://github.com/ksvc/KSYMediaPlayer_iOS/wiki/playerRecord)
* [短视频编辑](https://github.com/ksvc/KSYMediaEditorKit_iOS)
* 动态贴纸：[商汤科技](SenseTime)、[项芯科技](FaceUnitySDK)、[开为科技](KSYKiwiface)
* 无人机直播，已经支持多家无人机、云台直播方案，请直接联系商务
* 多种美颜：[涂图美颜](TuSDK)、[开为科技](KSYKiwiface)
* [画中画](https://github.com/ksvc/KSYLive_iOS/wiki/pip)
* [大眼瘦脸](KSYKiwiface)
* [背景替换](KSYNVSDK)

### 5. 开发合作
任何企业、个人都可以参与，提供的SDK付费、免费都可以。不限于以下的合作场景：

1. 企业商用SDK，金山云提供品牌露出和鉴权、付费接口露出；  
1. 个人完成的SDK；  
1. 基于[libksygpulive][libksygpulive]的任意工具、SDK、plugin；  

### 5.1 原则
第三方音视频处理SDK与[libksygpulive][libksygpulive]组合，完成数据流通路上的协作。串联第三方SDK和[libksygpulive][libksygpulive]的代码，全部**开源提供**。方便大家修改和适配其他SDK。


### 6.大事记

1. 2016年10月20日，基于Replaykit(iOS10+)的[游戏录屏](KSYReplayKit)代码开放 
1. 2016年10月28日，支持[Faceunity动态贴纸方案](FaceUnitySDK)；
1. 2016年10月28日，支持[涂图SDK美颜方案](TuSDK)；
1. 2016年12月2日，支持[商汤科技动态贴纸方案](SenseTime);
1. 2016年12月5日，基于[声网连麦方案](https://github.com/ksvc/KSYDiversityLive_iOS/releases/tag/agora_v1.0.0)上线；
1. 2017年5月18日，基于xindawn的[Airplay任意录屏方案(iOS 7.0+)](https://github.com/ksvc/KSYDiversityLive_iOS/releases/tag/KSYAirPlayDemo_v1.0)上线；
1. 2017年7月1日，基于金山云自研的[Airplay手游录屏方案](https://github.com/ksvc/KSYAirStreamer_iOS/releases)上线；
1. 2017年8月4日，[开为科技动态贴纸方案](KSYKiwiface)直播上线；
1. 2017年8月11日，[美摄背景替换](KSYNVSDK)直播上线；

### 7. 反馈与建议
#### 7.1 反馈模板  

| 类型    | 描述|
| :---: | :---:| 
|SDK名称|KSYDiversityLive_iOS|
|SDK版本| v2.5.0|
|设备型号| iphone7  |
|OS版本| iOS 10 |
|问题描述| 描述问题出现的现象  |
|操作描述| 描述经过如何操作出现上述问题                     |
|额外附件| 文本形式控制台log、crash报告、其他辅助信息（界面截屏或录像等） |

#### 7.2 联系方式
- 主页：[金山云](http://v.ksyun.com)
- 邮箱：<zengfanping@kingsoft.com>
* QQ讨论群：
    * 574179720 [视频云技术交流群]
    * 621137661 [视频云iOS技术交流]
    * 以上两个加一个QQ群即可
- Issues: <https://github.com/ksvc/KSYDiversityLive_iOS/issues>

[libksygpulive]:https://github.com/ksvc/KSYLive_iOS
[KSYDiversityLive]:https://github.com/ksvc/KSYDiversityLive_iOS
