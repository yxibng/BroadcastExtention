//
//  TSObjects.h
//  TSPasS_iOS
//
//  Created by yxibng on 2020/11/4.
//

#import <Foundation/Foundation.h>
#import "TSEnumerates.h"
#import "TSMediaIO.h"

#if TARGET_OS_IOS
#import <UIKit/UIKit.h>
typedef UIView VIEW_CLASS;
typedef UIColor COLOR_CLASS;
#elif TARGET_OS_OSX
#import <AppKit/AppKit.h>
typedef NSView VIEW_CLASS;
typedef NSColor COLOR_CLASS;
#endif


NS_ASSUME_NONNULL_BEGIN

__attribute__((visibility("default"))) @interface TSRtcEngineConfig : NSObject
/**
 TSPaaS 为 app 开发者签发的 App ID，详见获取 App ID。使用同一个 App ID 的 app 才能进入同一个频道进行通话或直播。一个 App ID 只能用于创建一个 TSRtcEngineKit。如需更换 App ID，必须先调用 destroy 销毁当前 TSRtcEngineKit，并在 destroy 成功返回后，再调用 sharedEngineWithConfig 重新创建 TSRtcEngineKit。
 */
@property (copy, nonatomic) NSString * _Nullable appId;
/**
 服务器的访问区域。该功能为高级设置，适用于有访问安全限制的场景。
 默认： TSRegionCodeGlobal
 */
@property (nonatomic, assign) TSRegionCode regionCode;
/**
 环境配置, json 字符串，在私有化部署环境情况下使用。一般情况下不配置。

 envConfig 是一个json字符串，可以包含`api`, `log` ,`enbaleReleaseAbort`选项。
 `api`, `log` 都是可选字段，若字段不存在， 会使用sdk设置的默认值， 否则使用用户设置的值。

 - `api`: 根据这个地址去切换目标环境， 如果不设置使用sdk内置的api地址
 - `log`: 根据这个地址去切换日志收集的接口， 如果不设置使用sdk内置的log地址
 
 `enbaleReleaseAbort`为可选字段，默认为`true`, 在用户使用不合约束的调用时，sdk内部会触发`abort`, 设置为`false`关闭触发`abort`.
 */
@property (nonatomic, copy) NSString * _Nullable envConfig;

/**
 编码偏好, 默认 TSCodecPriorityAuto
 */
@property (nonatomic, assign) TSCodecPriority codecPriority;

/*
默认日志路径：
 - iOS：App Sandbox/Library/caches/tsrtcsdk.log
 - macOS
    *开启沙盒：App Sandbox/Library/Logs/tsrtcsdk.log，例如 /Users/<username>/Library/Containers/<App BundleIdentifier>/Data/Library/Logs/tsrtcsdk.log
    *关闭沙盒：～/Library/Logs/tsrtcsdk.log
*/
@property (nonatomic, copy, nullable) NSString *logFilePath;

//默认 TSLogLevelInfo
@property (nonatomic, assign) TSLogLevel logLevel;

// 单个日志文件的大小，单位为 KB。默认 10 * 1024KB
@property (nonatomic, assign) NSUInteger logFileSize;

@end



#pragma mark -
__attribute__((visibility("default"))) @interface TSRtcVideoCanvas : NSObject
/**
 视频显示视窗
 VIEW_CLASS 为统称，具体为：
 iOS: UIView
 MacOS: NSView
 */
@property (strong, nonatomic) VIEW_CLASS* _Nullable view;
/**
 视图渲染模式, 详见 TSVideoRenderMode, 默认 TSVideoRenderModeFit
 */
@property (assign, nonatomic) TSVideoRenderMode renderMode;
/**
 能标识频道的频道名
 */
@property (copy, nonatomic) NSString* _Nullable channelId;
/**
 用户 ID，指定需要显示视窗的 uid
 */
@property (copy, nonatomic) NSString* uid;
/**
 流的名字，如果是默认流streamName为空，如果是多路流则为用户自己制定的streamName
 详见，自定义推送多路流。
 */
@property (copy, nonatomic) NSString* _Nullable streamName;//扩展
/**
 视图镜像模式,详见 TSVideoMirrorMode， 默认 TSVideoMirrorModeAuto
 */
@property (assign, nonatomic) TSVideoMirrorMode mirrorMode;
@end




#pragma mark -
__attribute__((visibility("default"))) @interface TSVideoEncoderConfiguration: NSObject
/**
 视频编码的宽和髙，默认值为 640 x 360。用户可以自行设置分辨率。
 */
@property (assign, nonatomic) CGSize dimensions;
/**
 视频编码的帧率: FRAME_RATE 。默认值为 15。
 参考`TSVideoFrameRate`
 */
@property (assign, nonatomic) NSInteger frameRate;
/**
 视频的最小帧率。默认值为 -1。
 */
@property (assign, nonatomic) NSInteger minFrameRate;
/**
 视频编码码率，单位为 Kbps:
 你可以根据场景需要参照下表手动设置你想要的码率。若设置的视频码率超出合理范围，SDK 会自动按照合理区间处理码率。你也可以直接选择如下任意一种模式进行设置：
 当bitrate设置为0: (推荐) 标准码率模式。该模式下SDK会自动根据设置的分辨率和帧率设置码率
 
 |分辨率|    帧率|    码流(Kbps)|
 ---|---|---
 |120*120    |15    |50|
 |160*120    |15    |60|
 |180*180    |15    |75|
 |240*180    |15    |100|
 |320*180    |15    |135|
 |240*240    |15    |135|
 |320*240    |15    |180|
 |424*240    |15    |240|
 |360*360    |15    |200|
 |360*360    |30    |300|
 |480*360    |15    |270|
 |480*360    |30    |405|
 |640*360    |15    |360|
 |640*360    |30    |540|
 |480*480    |15    |315|
 |480*480    |30    |475|
 |640*480    |15    |420|
 |640*480    |30    |630|
 |840*480    |15    |550|
 |840*480    |30    |825|
 |960*720    |15    |810|
 |960*720    |30    |1215|
 |1280*720   |15    |1080|
 |1280*720   |30    |1620|
 |1920*1080  |15    |2025|
 |1920*1080  |30    |3100|
 
 */
@property (assign, nonatomic) NSInteger bitrate;
/**
 最低编码码率，单位为 Kbps。
 */
@property (assign, nonatomic) NSInteger minBitrate;
/**
 视频编码的方向模式，参考`TSVideoOutputOrientationMode`
 */
@property (assign, nonatomic) TSVideoOutputOrientationMode orientationMode;
/**
 设置本地发送视频的镜像模式，只影响远端用户看到的视频画面。 参考`TSVideoMirrorMode`
 */
@property (assign, nonatomic) TSVideoMirrorMode mirrorMode;
/**
 码率控制方式, 默认固定码率。如果是硬编码只支持ABR，如果是软编码则支持VBR和CBR。
 */
@property (assign, nonatomic) TSBitrateControlType bitrateControlType;
/**
 小流视频宽度, 最多不能超过大流的百之五十, 为0则使用推荐值
 */
@property (assign, nonatomic) NSInteger lowWidth;
/**
 小流视频高度, 最多不能超过大流的百分之五十, 为0则使用推荐值
 */
@property (assign, nonatomic) NSInteger lowHeight;
/**
 小流码率,单位kbps, 宽高同为0时, 使用推荐值, 否则根据宽高进行计算
 */
@property (assign, nonatomic) NSInteger lowBitrate;
/**
 小流帧率, 不超过大流帧率, 不低于5. (可选值: 5 10 15 30 60)
 */
@property (assign, nonatomic) NSInteger lowFrameRate;

- (instancetype _Nonnull)initWithSize:(CGSize)size
                            frameRate:(TSVideoFrameRate)frameRate
                              bitrate:(NSInteger)bitrate
                      orientationMode:(TSVideoOutputOrientationMode)orientationMode
                           mirrorMode:(TSVideoMirrorMode)mirrorMode;

- (instancetype _Nonnull)initWithWidth:(NSInteger)width
                                height:(NSInteger)height
                             frameRate:(TSVideoFrameRate)frameRate
                               bitrate:(NSInteger)bitrate
                       orientationMode:(TSVideoOutputOrientationMode)orientationMode
                            mirrorMode:(TSVideoMirrorMode)mirrorMode;
@end

#pragma mark -

__attribute__((visibility("default"))) @interface TSLastmileProbeConfig : NSObject
/** 是否探测上行网络
 */
@property (assign, nonatomic) BOOL probeUplink;
/**
 是否探测下行网络。
 */
@property (assign, nonatomic) BOOL probeDownlink;
/**
 用户期望的最高发送码率，单位为 bps，范围为 [100000, 5000000]。
 */
@property (assign, nonatomic) NSUInteger expectedUplinkBitrate;
/**
 用户期望的最高接收码率，单位为 bps，范围为 [100000, 5000000]。
 */
@property (assign, nonatomic) NSUInteger expectedDownlinkBitrate;
@end



#pragma mark -
__attribute__((visibility("default"))) @interface TSRtcAudioVolumeInfo : NSObject
/**
 用户uid
 */
@property (copy, nonatomic) NSString* uid;
/**
 用户channelId
 */
@property (copy, nonatomic) NSString * _Nonnull channelId;
/**
 说话者音量
 */
@property (assign, nonatomic) NSUInteger volume;
/**
 */
@property (assign, nonatomic) NSUInteger vad;
@end


#pragma mark -
__attribute__((visibility("default"))) @interface TSRtcRemoteAudioStats : NSObject
/**
 */
@property (copy, nonatomic) NSString* uid;
/**
 */
@property (assign, nonatomic) NSUInteger audioLossRate;
/**
 */
@property (assign, nonatomic) NSUInteger numChannels;
/**
 */
@property (assign, nonatomic) NSUInteger receivedSampleRate;
/**
 */
@property (assign, nonatomic) NSUInteger receivedBitrate;
/**
 */
@property (assign, nonatomic) NSUInteger totalFrozenTime;
/**
 */
@property (assign, nonatomic) NSUInteger frozenRate;
/**
 */
@property (assign, nonatomic) NSUInteger totalActiveTime;
/**
 */
@property (assign, nonatomic) NSInteger publishDuration;
@end

__attribute__((visibility("default"))) @interface TSRtcLocalAudioStats : NSObject
/**
 */
@property (assign, nonatomic) NSUInteger numChannels;
/**
 */
@property (assign, nonatomic) NSUInteger sentSampleRate;
/**
 */
@property (assign, nonatomic) NSUInteger sentBitrate;
/**
 */
@property (assign, nonatomic) NSUInteger txPacketLossRate;
@end


#pragma mark -
__attribute__((visibility("default"))) @interface TSRtcLocalVideoStats : NSObject
/**
 */
@property (copy, nonatomic) NSString* uid;
/**
 */
@property (copy, nonatomic, nullable) NSString* streamName;
/**
 */
@property (assign, nonatomic) NSUInteger sentBitrate;
/**
 */
@property (assign, nonatomic) NSUInteger sentFrameRate;
/**
 */
@property (assign, nonatomic) NSUInteger encoderOutputFrameRate;
/**
 */
@property (assign, nonatomic) NSUInteger rendererOutputFrameRate;
/**
 */
@property (assign, nonatomic) NSUInteger sentTargetBitrate;
/**
 */
@property (assign, nonatomic) NSUInteger sentTargetFrameRate;
/**
 */
@property (assign, nonatomic) NSUInteger encodedBitrate;
/**
 */
@property (assign, nonatomic) NSUInteger encodedFrameWidth;
/**
 */
@property (assign, nonatomic) NSUInteger encodedFrameHeight;
/**
 */
@property (assign, nonatomic) NSUInteger encodedFrameCount;
/**
 */
@property (assign, nonatomic) NSInteger txPacketLossRate;
/**
 */
@property (assign, nonatomic) NSInteger captureFrameRate;
@end

/** Statistics of the remote video stream.
 */
__attribute__((visibility("default"))) @interface TSRtcRemoteVideoStats : NSObject
/**
 */
@property (copy, nonatomic) NSString* uid;
/**
 */
@property (copy, nonatomic, nullable) NSString* streamName;
/**
 */
@property (assign, nonatomic) NSUInteger width;
/**
 */
@property (assign, nonatomic) NSUInteger height;
/**
 */
@property (assign, nonatomic) NSUInteger receivedBitrate;
/**
 */
@property (assign, nonatomic) NSUInteger decoderOutputFrameRate;
/**
 */
@property (assign, nonatomic) NSUInteger packetLossRate;
/**
 */
@property (assign, nonatomic) TSVideoStreamType rxStreamType;
/**
 */
@property (assign, nonatomic) NSUInteger totalFrozenTime;
/**
 */
@property (assign, nonatomic) NSUInteger frozenRate;
/**
 */
@property (assign, nonatomic) NSUInteger totalActiveTime;
/**
 */
@property (assign, nonatomic) NSInteger publishDuration;
/**
 */
@end


#pragma mark -
__attribute__((visibility("default"))) @interface TSChannelStats: NSObject
/** 本地用户通话时长（毫秒）。累计值。
 */
@property (assign, nonatomic) NSInteger duration;
/** 发送字节数（bytes）。
 */
@property (assign, nonatomic) NSInteger txBytes;
/** 接收字节数（bytes）。
 */
@property (assign, nonatomic) NSInteger rxBytes;
/** 发送音频字节数（bytes），累计值。
 */
@property (assign, nonatomic) NSInteger txAudioBytes;
/** 发送视频字节数（bytes），累计值。
 */
@property (assign, nonatomic) NSInteger txVideoBytes;
/** 接收音频字节数（bytes），累计值。
 */
@property (assign, nonatomic) NSInteger rxAudioBytes;
/** 接收视频字节数（bytes），累计值。
 */
@property (assign, nonatomic) NSInteger rxVideoBytes;
/** 发送码率（Kbps）。
 */
@property (assign, nonatomic) NSInteger txKBitrate;
/** 接收码率（Kbps）。
 */
@property (assign, nonatomic) NSInteger rxKBitrate;
/** 音频接收码率 (Kbps）。
 */
@property (assign, nonatomic) NSInteger txAudioKBitrate;
/** 音频的发送码率 (Kbps）。
 */
@property (assign, nonatomic) NSInteger rxAudioKBitrate;
/** 视频接收码率 (Kbps）。
 */
@property (assign, nonatomic) NSInteger txVideoKBitrate;
/** 视频发送码率 (Kbps）。
 */
@property (assign, nonatomic) NSInteger rxVideoKBitrate;
/** 客户端-接入服务器延时 (毫秒)。
 */
@property (assign, nonatomic) NSInteger lastmileDelay;
/** 使用抗丢包技术前，客户端上行发送到服务器丢包率 (%)
 */
@property (assign, nonatomic) NSInteger txPacketLossRate;
/** 使用抗丢包技术前，客户端上行发送到服务器丢包率 (%)
 */
@property (assign, nonatomic) NSInteger rxPacketLossRate;
/** 当前频道内的用户人数。
 */
@property (assign, nonatomic) NSInteger userCount;
/** 当前 App 的 CPU 使用率 (%)。
 */
@property (assign, nonatomic) double cpuAppUsage;
/** 当前系统的 CPU 使用率 (%)。
    在多核环境中，该成员指多核 CPU 的平均使用率。 计算方式为 100 - 任务管理中显示的系统空闲进程 CPU（）。
 */
@property (assign, nonatomic) double cpuTotalUsage;

@property (assign, nonatomic) NSInteger gatewayRtt;
/**当前 App 的内存占比 (%)
 */
@property (assign, nonatomic) double memoryAppUsageRatio;
/**当前系统的内存占比 (%)
 */
@property (assign, nonatomic) double memoryTotalUsageRatio;
/** 当前 App 的内存大小 (KB)
 */
@property (assign, nonatomic) NSInteger memoryAppUsageInKbytes;

@end

#pragma mark - 

__attribute__((visibility("default"))) @interface TSLastmileProbeOneWayResult : NSObject
/**
 丢包率
 */
@property (assign, nonatomic) NSUInteger packetLossRate;
/**
 网络抖动（ms）
 */
@property (assign, nonatomic) NSUInteger jitter;
/**
 可用网络带宽预估（bps）
 */
@property (assign, nonatomic) NSUInteger availableBandwidth;
/**
 网络质量
 */
@property (assign, nonatomic) TSNetworkQuality quality;

@end



__attribute__((visibility("default"))) @interface TSLastmileProbeResult : NSObject
/**
 Last mile 质量探测结果的状态
 */
@property (assign, nonatomic) TSLastmileProbeResultState state;
/**
 往返时延 (ms)。
 */
@property (assign, nonatomic) NSUInteger rtt;
/**
 上行网络质量报告
 */
@property (strong, nonatomic) TSLastmileProbeOneWayResult *_Nonnull uplinkReport;
/**
 下行网络质量报告
 */
@property (strong, nonatomic) TSLastmileProbeOneWayResult *_Nonnull downlinkReport;

@end


#if TARGET_OS_OSX

__attribute__((visibility("default"))) @interface TSRtcDeviceInfo : NSObject
/**
 Device Type
 */
@property (assign, nonatomic) TSMediaDeviceType type;

/** Device ID.
 */
@property (copy, nonatomic) NSString * _Nullable deviceId;

/** Device name.
 */
@property (copy, nonatomic) NSString * _Nullable deviceName;
@end

#endif





NS_ASSUME_NONNULL_END
