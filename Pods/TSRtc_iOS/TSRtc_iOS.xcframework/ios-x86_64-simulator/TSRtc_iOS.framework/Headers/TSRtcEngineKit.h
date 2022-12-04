//
//  TSRtcEngineKit.h
//  TSPasS_iOS
//
//  Created by yxibng on 2020/11/3.
//

#import <Foundation/Foundation.h>
#import "TSEnumerates.h"
#import "TSConstants.h"
#import "TSObjects.h"


NS_ASSUME_NONNULL_BEGIN

@class TSRtcEngineKit;

@protocol TSRtcEngineDelegate <NSObject>

@optional
/**
 发生警告回调。
 该回调方法表示 SDK 运行时出现了（网络或媒体相关的）警告。通常情况下，SDK 上报的警告信息 App 可以忽略，SDK 会自动恢复。
 
 @param engine TSRtcEngineKit 实例
 @param warningCode 警告代码
 @param message 警告描述
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine didOccurWarning:(int)warningCode message:(NSString *_Nonnull)message;
/**
 发生错误回调。
 该回调方法表示 SDK 运行时出现了（网络或媒体相关的）错误。通常情况下，SDK 上报的错误意味着 SDK 无法自动恢复，需要 App 干预或提示用户。
 
 @param engine TSRtcEngineKit 实例
 @param errorCode 错误代码
 @param message 错误描述
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine didOccurError:(int)errorCode message:(NSString *_Nonnull)message;


#pragma mark -
/**
 本地视频推流，预测的新的码率
 @param engine TSRtcEngineKit 实例
 @param newBitrate 预测的新的码率
 @param isLowVideo 是否预测的是小流
 */
- (void)rtcEngine:(TSRtcEngineKit *)engine onPredictedBitrateChanged:(int)newBitrate isLowVideo:(BOOL)isLowVideo;
/**
 本地视频状态发生改变回调
 本地视频的状态发生改变时（包括本地摄像头采集状态），SDK 会触发该回调返回当前的本地视频状态。
 在本地视频出现故障时，你可以通过该回调了解当前视频的状态以及出现故障的原因，方便排查问题。
 
 @param engine TSRtcEngineKit 实例
 @param state 本地视频状态, 详见`TSLocalVideoStreamState`
 @param error 本地视频出错原因,详见`TSLocalVideoStreamError`
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine localVideoStateChange:(TSLocalVideoStreamState)state error:(TSLocalVideoStreamError)error;
/**
 已显示本地视频首帧回调。
 本地视频首帧显示在本地视图上时，SDK 会触发此回调。
 已经在本地预览，此时关闭预览再调用启动预览也会触发该回调。
 @param engine TSRtcEngineKit 实例
 @param size 本地渲染视频的分辨率大小
 @param elapsed 从调用 `sharedEngineWithConfig` 方法到触发该回调的时间间隔（毫秒）。
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine firstLocalVideoFrameWithSize:(CGSize)size elapsed:(NSInteger)elapsed;

/**
 已发布本地视频频首帧回调。
 SDK 会在以下时机触发该回调：
 - 开启本地视频的情况下，在加入频道后调用`publish`后产生。
 
 @param engine TSRtcEngineKit 实例
 @param elapsed 从调用 `sharedEngineWithConfig` 方法到触发该回调的时间间隔（毫秒）。
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine firstLocalVideoFramePublished:(NSInteger)elapsed;

/**
 本地编码分辨率大小改变。
 @param engine TSRtcEngineKit 实例
 @param size 本地视频编码分辨率
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine localVideoSizeChanged:(CGSize)size;


/**
 本地音频状态发生改变回调。
 本地音频的状态发生改变时（包括本地麦克风录制状态）， SDK 会触发该回调报告当前的本地音频状态。在本地音频出现故障时， 该回调可以帮助你了解当前音频的状态以及出现故障的原因，方便你排查问题。
 @param engine TSRtcEngineKit 实例
 @param state 当前的本地音频状态, 详见`TSLocalAudioState`
 @param error 本地音频出错原因, 详见`TSLocalAudioError`
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine localAudioStateChange:(TSLocalAudioState)state error:(TSLocalAudioError)error;
/**
 已发布本地音频首帧回调。
 SDK 会在以下时机触发该回调：
 - 开启本地音频的情况下，在加入频道后调用`publish`后产生。
 
 @param engine TSRtcEngineKit 实例
 @param elapsed 从调用 `sharedEngineWithConfig` 方法到触发该回调的时间间隔（毫秒）。
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine firstLocalAudioFramePublished:(NSInteger)elapsed;
/**
 
 提示频道内远端用户谁正在说话、说话者音量的回调。
 该回调报告频道内所有用户的瞬时音量、他们的音量及本地用户是否在说话。
 该回调默认禁用。可以通过 enableAudioVolumeIndication 方法开启；SDK 都会按 enableAudioVolumeIndication 方法中设置的时间间隔触发 reportAudioVolumeIndicationOfSpeakers 回调。每次触发用户会收到 reportAudioVolumeIndicationOfSpeakers 回调包含远端所有用户的音量信息，详见下方参数描述。
 
 @param engine TSRtcEngineKit 实例
 @param speakers TSRtcAudioVolumeInfo 数组
 @param totalVolume 混音后的）总音量。在本地用户的回调中，totalVolume 为本地用户混音后的音量。 在远端用户的回调中，totalVolume 为所有说话者混音后的总音量。
 
 注意：
  - 若需使用该回调 speakers 数组中的 vad 参数（即人声检测功能），请在 enableAudioVolumeIndication 方法中设置 report_vad 为 true。
  - 远端说话者静音后，远端的音量提示回调中将不再包含该用户；如果远端所有用户都将自己静音，SDK 不再报告远端用户的音量提示回调。
 
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine reportAudioVolumeIndicationOfSpeakers:(NSArray<TSRtcAudioVolumeInfo *> * _Nonnull)speakers totalVolume:(NSInteger)totalVolume;
/**
 语音路由已发生变化回调。
 当语音路由发生变化时，SDK 会触发此回调。
 @param engine TSRtcEngineKit 实例
 @param routing 当前的语音路由, 详见`TSAudioOutputRouting`
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine didAudioRouteChanged:(TSAudioOutputRouting)routing;

/**
 通话中本地音频流的统计信息回调。
 该回调描述本地设备发送音频流的统计信息。SDK 每 2 秒触发该回调一次。
 @param engine TSRtcEngineKit 实例
 @param stats 本地音频统计数据,详见`TSRtcLocalAudioStats`
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine localAudioStats:(TSRtcLocalAudioStats * _Nonnull)stats;
/**
 
 本地视频流统计信息回调。
 该回调描述本地设备发送视频流的统计信息，每 2 秒触发一次。
 
 @param engine TSRtcEngineKit 实例
 @param stats 本地视频流统计信息,详见`TSRtcLocalVideoStats`
 
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine localVideoStats:(TSRtcLocalVideoStats * _Nonnull)stats;
/**
 本地网络类型发生改变回调。
 本地网络连接类型发生改变时，SDK 会触发该回调，并在回调中明确当前的网络连接类型。
 你可以通过该回调获取正在使用的网络类型；当连接中断时，该回调能辨别引起中断的原因是网络切换还是网络条件不好。
 
 @param engine TSRtcEngineKit 实例
 @param type 本地网络类型,详见`TSNetworkType`
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine networkTypeChangedToType:(TSNetworkType)type;

/**

 本地网络上下行质量回调
 该回调每 2 秒触发一次。
 
 @param engine TSRtcEngineKit 实例
 @param txQuality 该用户的上行网络质量，基于发送码率、上行丢包率、平均往返时延和网络抖动计算。该值代表当前的上行网络质量，帮助判断是否可以支持当前设置的视频编码属性。假设上行带宽是 1000 Kbps，那么支持直播场景下 640 × 480 的分辨率、15 fps 的帧率没有问题，但是支持 1280 × 720 的分辨率就会有困难。详见`TSNetworkQuality`
 @param rxQuality 该用户的下行网络质量，基于下行网络的丢包率、平均往返延时和网络抖动计算。详见`TSNetworkQuality`
 
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine localNetworkTxQuality:(TSNetworkQuality)txQuality rxQuality:(TSNetworkQuality)rxQuality;
/**
 通话前网络质量探测报告回调。
 通话前网络上下行 Last mile 质量探测报告回调。在调用 stopLastmileProbeTest 之后，SDK 触发该回调。
 
 @param engine TSRtcEngineKit 实例
 @param result 上下行 Last mile 质量探测结果。详见`TSLastmileProbeResult`
 */
- (void)rtcEngine:(TSRtcEngineKit * _Nonnull)engine lastmileProbeTestResult:(TSLastmileProbeResult * _Nonnull)result;


#if TARGET_OS_OSX
/**
  设备变更回调。
  @param engine TSRtcEngineKit 实例
  @param deviceId 设备ID
  @param deviceType 设备类型
  @param state 状态
 */

- (void)rtcEngine:(TSRtcEngineKit *_Nonnull)engine device:(NSString *_Nonnull)deviceId type:(TSMediaDeviceType)deviceType stateChanged:(TSDeviceChangeType)state;
#endif


@end


__attribute__((visibility("default"))) @interface TSRtcEngineKit : NSObject

/**
 初始化一个 TSRtcEngineKit 对象
 
 @param config TSRtcEngineConfig实例的配置，参考`TSRtcEngineConfig`。
 @param delegate TSRtcEngineDelegate
 @return 方法调用成功，返回一个 TSRtcEngineKit 对象。
 
 注意：
 
 请确保在调用其他 API 前先调用该方法创建并初始化 TSRtcEngineKit。
 
 一个 App ID 只能用于创建一个 TSRtcEngineKit。如需更换 App ID，必须先调用 destroy 销毁当前 TSRtcEngineKit，并在 destroy 成功返回后，再调用 sharedEngineWithConfig 重新创建 TSRtcEngineKit。
 
 */
+ (instancetype _Nonnull)sharedEngineWithConfig:(TSRtcEngineConfig *_Nonnull)config delegate:(id<TSRtcEngineDelegate> _Nonnull)delegate;

/**
 销毁 TSRtcEngineKit 实例。
 调用 destroy 方法后，你将无法再使用 SDK 的其它方法和回调。
 如需再次使用实时音视频通信功能，必须重新调用 sharedEngineWithConfig 方法创建一个新的 TSRtcEngineKit 实例。
 */
+ (void)destroy;

#pragma mark -
/**
 开始通话前网络质量探测。
 开始通话前网络质量探测，向用户反馈上下行网络的带宽、丢包、网络抖动和往返时延数据。
 该方法主要用于以下场景：
 - 用户加入频道前，可以调用该方法判断和预测目前的当前网络质量是否足够好。
 
 @param config 网络探测配置,参考 `TSLastmileProbeConfig`
 @return 0 调用成功，<0 调用失败。
 
 注意：
  - 调用该方法后，需要调用stopLastmileProbeTest关闭测试
  - 如果未调用stopLastmileProbeTest就进入频道，SDK会自动关闭测试。
  - 加入频道后调用该方法无效。
  - 确保`startLastmileProbeTest`和`startLastmileProbeTest`调用间隔至少10秒, 否则返回的结果可能不完整或者准确度不足。
 */
- (int)startLastmileProbeTest:(TSLastmileProbeConfig *_Nullable)config;
/**
 停止通话前网络质量探测。
 停止通话前网络质量探测，将会收到 `lastmileProbeTestResult`回调
 @return 0 调用成功，<0 调用失败。
 */
- (int)stopLastmileProbeTest;

#pragma mark - Audio
/**
 开关本地音频采集。
 当用户加入频道时，语音功能默认是开启的。该方法可以关闭或重新开启本地语音功能，即停止或重新开始本地音频采集。
 该方法不影响接收或播放远端音频流，enableLocalAudio(NO) 适用于只听不发的用户场景。
 语音功能关闭或重新开启后，会收到回调 `localAudioStateChange`的回调。
 
 @param enalbed YES 开启采集本地音频采集， NO 关闭本地音频 采集
 @return 0 调用成功，<0 调用失败。
 
 该方法与 muteLocalAudioStream 的区别在于
 - enableLocalAudio: 开启或关闭本地语音采集及处理。
 - muteLocalAudioStream: 停止或继续发送本地音频流
 */
- (int)enableLocalAudio:(BOOL)enalbed;
/**
 设置音频编码属性。
 @param profile 设置采样率，码率，编码模式和声道数,参考`TSAudioProfile`
 @param scenario 设置音频应用场景,不同的音频场景下，设备的系统音量是不同的。详见如何区分媒体音量和通话音量。参考`TSAudioScenario`
 @return 0 调用成功，<0 调用失败。
 注意：
  - 该方法需要在 joinChannel 之前设置好， joinChannel 之后设置不生效
  - 通信和直播场景下，音质（码率）会有网络自适应的调整，通过该方法设置的是一个最高码率
 
 */
- (int)setAudioProfile:(TSAudioProfile)profile
              scenario:(TSAudioScenario)scenario;

/**
 开关本地音频发送。
 该方法用于允许/禁止往网络发送本地音频流。
 @param mute YES, 停止发送本地音频流。NO，恢复发送本地音频流。
 @return 0 调用成功，<0 调用失败。
 
 注意：
 该方法不影响录音状态，因为并没有禁用录音设备
 */
- (int)muteLocalAudioStream:(BOOL)mute;
/**
 启用说话者音量提示。
 @param interval 指定音量提示的时间间隔, 单位ms，推荐 200ms。interval <=0 代表关闭音量提示。
 @param smooth 平滑系数，指定音量提示的灵敏度。取值范围为 [1,10]，建议值为 3。数字越大，波动越灵敏；数字越小，波动越平滑。
 @param report_vad YES:开启本地人声检测功能。开启后，reportAudioVolumeIndicationOfSpeakers 回调的 vad 参数会报告是否在本地检测到人声。
 NO:（默认）关闭本地人声检测功能。除引擎自动进行本地人声检测的场景外，reportAudioVolumeIndicationOfSpeakers 回调的 vad 参数不会报告是否在本地检测到人声。

 @discussion 关闭音量提示设置（interval <=0, smooth = 3, report_vad = NO)
 @return 0 调用成功，<0 调用失败。
 */
- (int)enableAudioVolumeIndication:(NSInteger)interval
                            smooth:(NSInteger)smooth
                        report_vad:(BOOL)report_vad;


#pragma mark -
/**
 设置自定义的音频源。
 
 实时通信过程中，SDK 会启动默认的音频输入设备，即内置的麦克风，采集音频。但是，如果你需要自定义音频输入设备， 你可以先通过 TSAudioSourceProtocol 自定义音频源，再调用该方法将自定义的音频源加入到 SDK 中。
 @param audioSource 自定义的音频源。详细定义见 TSAudioSourceProtocol。
 @return 0 调用成功，<0 调用失败。
 
 必须要在流启动之前调用setLocalAudioSource, 流启动的时机
 - joinChannel会启动流；
 - startPreview的时候会启动流；
 */
- (int)setLocalAudioSource:(id<TSAudioSourceProtocol> _Nullable)audioSource;

#if TARGET_OS_IOS
/**
 设置默认的语音路由
 该方法设置接收到的语音从听筒或扬声器出声。如果用户不调用本方法，语音默认从听筒出声。
 @param defaultToSpeaker YES: 语音路由为外放（扬声器）。如果设备连接了耳机或蓝牙，则无法切换到外放。NO:（默认）语音路由为听筒。
 @return 0 调用成功，<0 调用失败。
 注意：
 - 该方法仅适用于iOS 平台。
 - 该方法需要在 joinChannel 前设置，否则不生效。如需在加入频道后更改默认语音路由，请调用 setEnableSpeakerphone 方法。
 */
- (int)setDefaultAudioRouteToSpeakerphone:(BOOL)defaultToSpeaker;
/**
 启用/关闭扬声器播放
 该方法设置是否将语音路由设到扬声器（外放）。
 @param enableSpeaker YES: 切换到外放。如果设备连接了耳机或蓝牙，则无法切换到外放。
 NO: 切换到听筒。如果设备连接了耳机，则语音路由走耳机。
 @return 0 调用成功，<0 调用失败。
 */
- (int)setEnableSpeakerphone:(BOOL)enableSpeaker;
/**
 查询扬声器启用状态
 @return YES:  正在使用扬声器 NO: 未使用扬声器
 */
- (BOOL)isSpeakerphoneEnabled;

#endif


#pragma mark - Video
/**
 是否开启码率预测 和 自动调整功能
 @param enablePrediction 是否开启码率预测
 @param enableAutoAdjust 是否开启自动调整功能
 @return 0 调用成功，<0 调用失败。
 注意:
    需要在进入频道前调用，进入到后调用会失败。
 */
- (int)enableBitratePrediction:(BOOL)enablePrediction enableAutoAdjust:(BOOL)enableAutoAdjust;

/**
 开关本地视频采集。
 该方法禁用或重新启用本地视频采集，不影响接收远端视频。
 
 @param enalbed YES: 开启本地视频采集和渲染 (默认)；NO: 关闭使用本地摄像头设备。关闭后，远端用户会接收不到本地用户的视频流；但本地用户依然可以接收远端用户的视频流。设置为 false 时，该方法不需要本地有摄像头。
 @return 0 调用成功，<0 调用失败。
 
 调用该方法，会收到`localVideoStateChange`的回调。
 
 该方法与 muteLocalVideoStream 的区别在于
 - enableLocalVideo: 开启或关闭本地视频采集及处理。
 - muteLocalVideoStream: 停止或继续发送本地视频流
 */
- (int)enableLocalVideo:(BOOL)enalbed;
/**
 设置本地视频的编码属性。
 @param config 视频编码参数配置,参考`TSVideoEncoderConfiguration`
 @return 0 调用成功，<0 调用失败。
 */
- (int)setVideoEncoderConfiguration:(TSVideoEncoderConfiguration * _Nonnull)config;
/**
 开关本地视频发送。
 调用该方法时，SDK 不再发送本地视频流，但摄像头仍然处于工作状态。相比于 `enableLocalVideo` 用于控制本地视频流发送的方法，该方法响应速度更快。该方法不影响本地视频流获取，没有禁用摄像头。
 @param mute  YES: 停止发送本地视频流；NO: 继续发送本地视频流（默认）。
 @return 0 调用成功，<0 调用失败。
 */
- (int)muteLocalVideoStream:(BOOL)mute;
/**
 开关双流模式。
 该方法设置单流（默认）或者双流模式。发送端开启双流模式后，接收端可以选择接收大流还是小流。其中，大流指高分辨率、高码率的视频流，小流指低分辨率、低码率的视频流。
 @param enabled  YES: 双流。 NO: 单流。
 @return 0 调用成功，<0 调用失败。
 */
- (int)enableDualStreamMode:(BOOL)enabled;
/**
 更新本地视图显示模式。
 初始化本地用户视图后，你可以调用该方法更新本地用户视图的渲染和镜像模式。该方法只影响本地用户看到的视频画面，不影响本地发布视频。
 @param renderMode 本地视图的渲染模式, 参考`TSVideoRenderMode`
 @param mirrorMode 本地视图的镜像模式，详见 `TSVideoMirrorMode` Note: 如果你使用前置摄像头，默认启动本地用户视图镜像模式；如果你使用后置摄像头，默认关闭本地视图镜像模式。
 @return 0 调用成功，<0 调用失败。
 
 注意：
  - 请在调用 setupLocalVideo 方法初始化本地视图后，调用该方法
  - 你可以在通话中多次调用该方法，多次更新本地用户视图的显示模式
 */
- (int)setLocalRenderMode:(TSVideoRenderMode)renderMode mirrorMode:(TSVideoMirrorMode)mirrorMode;
/**
 初始化本地视图。
 该方法初始化本地视图并设置本地用户视频显示信息，只影响本地用户看到的视频画面，不影响本地发布视频。调用该方法绑定本地视频流的显示视窗(view)，并设置本地用户视图的渲染模式和镜像模式。
 在 App 开发中，通常在初始化后调用该方法进行本地视频设置，然后再加入频道。退出频道后，绑定仍然有效，如果需要解除绑定，可以指定空(NULL)View 调用 setupLocalVideo。
 
 @param local 视频画布信息, 参考 `TSRtcVideoCanvas`
 @return 0 调用成功，<0 调用失败。

 - 建议你在加入频道前调用该方法。
 - 如果你希望在通话中更新本地用户视图的渲染或镜像模式，请使用setLocalRenderMode方法。
 */
- (int)setupLocalVideo:(TSRtcVideoCanvas * _Nonnull)local;
/**
 开启本地视频预览。
 该方法用于在进入频道前启动本地视频预览。调用该 API 前，必须：
 - 调用 enableLocalVideo 开启视频功能。
 - 调用 setupLocalVideo 设置预览窗口及属性；
 启用了本地视频预览后，如果调用 leaveChannel 退出频道，本地预览依然处于启动状态，如需要关闭本地预览，需要调用 stopPreview
 
 @return 0 调用成功，<0 调用失败。
 */
- (int)startPreview;
/**
 停止视频预览。
 @return 0 调用成功，<0 调用失败。
 */
- (int)stopPreview;
#if TARGET_OS_IOS
/**
 切换前置/后置摄像头
 该方法仅适用于 iOS 平台。
 @return 0 调用成功，<0 调用失败。
 */
- (int)switchCamera;
#endif


#pragma mark -
/**
 设置自定义的视频源。
 
 实时通信过程中，SDK 会启动默认的视频输入设备，即内置的摄像头，采集视频。但是，如果你需要自定义视频输入设备， 你可以先通过 TSVideoSourceProtocol 自定义视频源，再调用该方法将自定义的视频源加入到 SDK 中。
 @param videoSource 自定义的视频源。详细定义见 TSVideoSourceProtocol。
 @return 0 调用成功，<0 调用失败。
 
 必须要在流启动之前调用setLocalVideoSource, 流启动的时机
 - joinChannel会启动流；
 - startPreview的时候会启动流；
 */
- (int)setLocalVideoSource:(id<TSVideoSourceProtocol> _Nullable)videoSource;
/**
 自定义本地视频渲染器。
 
 该方法设置本地视频渲染器。实时通讯过程中，SDK 通常会启动默认的视频渲染器进行视频渲染。当需要自定义视频渲染设备时，App 可以先通过 TSVideoSinkProtocol 自定义渲染器，然后调用该方法将视频渲染器加入到 SDK 中。
 
 @param videoRenderer 自定义的本地视频渲染器。详细定义见 TSVideoSinkProtocol。
 @return 0 调用成功，<0 调用失败。
 */
- (int)setLocalVideoRenderer:(id<TSVideoSinkProtocol> _Nullable)videoRenderer;

#pragma mark -
/**
 获取警告或错误描述。
 @param code onWarning 或 onError 提供的警告码或错误码。
 @return code对应的描述
 */
+ (NSString * _Nullable)getErrorDescription:(NSInteger)code;
/**
 查询 SDK 版本号。
 @return 当前的 SDK 版本号，格式为字符串
 */
+ (NSString * _Nonnull)getSdkVersion;

#pragma mark - macos

#if TARGET_OS_OSX
/**
 获取系统中所有的音视频设备。
 
 @param type 要枚举的设备类型，参考 TSMediaDeviceType
 @return 调用成功时，返回 TSRtcDeviceInfo NSArray 对象，对应枚举类型的所有的音视频设备。
 */

- (NSArray<TSRtcDeviceInfo *> * _Nullable)enumerateDevices:(TSMediaDeviceType)type;
/**
 该方法通过 type 获取当前音、视频采集或播放设备。
 
 @param type  设备的类型，包括音、视频采集或播放设备，详见 TSMediaDeviceType
 @return 调用成功时，返回 TSRtcDeviceInfo 对象，调用失败时，返回 nil
 */
- (TSRtcDeviceInfo * _Nullable)getDeviceInfo:(TSMediaDeviceType)type;
/**
 该方法通过 deviceId 参数指定音、视频采集或播放设备。
 
 @param type  设备的类型，包括音、视频采集或播放设备，详见 TSMediaDeviceType
 @param deviceId  设备的 Device ID，可通过 enumerateDevices获取。
 @return 0: 方法调用成功 < 0: 方法调用失败
 */
- (int)setDevice:(TSMediaDeviceType)type deviceId:(NSString * _Nonnull)deviceId;
/**
 获取音频采集或者播放设备音量。

 @param type 设备的类型，音频采集或播放设备，详见 TSMediaDeviceType
 @return 调用成功，返回设备的音量, < 0: 方法调用失败
 */
- (int)getDeviceVolume:(TSMediaDeviceType)type;
/**
 设置音频采集或播放设备音量

 @param type 设备的类型，音频采集或播放设备，详见 TSMediaDeviceType
 @param volume 设置的音量，[0,100]
 @return 0: 方法调用成功 < 0: 方法调用失败
 */
- (int)setDeviceVolume:(TSMediaDeviceType)type volume:(int)volume;


/**
 该方法测试音频采集设备是否能正常工作。
 调用该方法后，SDK 会按设置的时间间隔触发 reportAudioVolumeIndicationOfSpeakers 回调，报告 uid = "" 及采集设备的音量信息。

 @param indicationInterval SDK 返回 reportAudioVolumeIndicationOfSpeakers 回调的时间间隔，单位为毫秒。建议设置到大于 200 毫秒。 不得少于 10 毫秒，否则会收不到 reportAudioVolumeIndicationOfSpeakers 回调。
 @return 0: 方法调用成功 < 0: 方法调用失败

 @discussion 该方法需要在加入频道前调用。如果需要停止测试，调用 `stopRecordingDeviceTest`。
 */
- (int)startRecordingDeviceTest:(int)indicationInterval;

/**
 停止麦克风测试
 该方法停止麦克风测试。调用 `startRecordingDeviceTest` 后，必须调用该方法停止测试。

 @return 0: 方法调用成功 < 0: 方法调用失败
 */
- (int)stopRecordingDeviceTest;

/**
 启动音频播放设备测试。
 该方法测试音频播放设备是否能正常工作。启动测试后，SDK 播放指定的音频文件，测试者如果能听到声音，说明播放设备能正常工作。
 
 @param audioFileName 指定音频文件的绝对路径, 支持的文件格式：mp3。 支持的采样率：8000，16000，32000，44100，48000
 @return 0: 方法调用成功 < 0: 方法调用失败
 
 @discussion 该方法需要在加入频道前调用。如果需要停止测试，调用 `stopPlaybackDeviceTest`。
 */
- (int)startPlaybackDeviceTest:(NSString *_Nonnull)audioFileName;
/**
 停止播放设备测试
 该方法停止播放设备测试。调用 startPlaybackDeviceTest 后，必须调用该方法停止测试。

 @return 0: 方法调用成功 < 0: 方法调用失败
 */
- (int)stopPlaybackDeviceTest;

/**
 启动视频采集设备测试
 sdk 内部会启动视频采集，将采集到的数据在 view 上面预览。
 
 @param view NSView 实例，用于显示图像的窗口。
 @return 0: 方法调用成功 < 0: 方法调用失败
 
 @discussion 该方法需要在加入频道前调用。如果需要停止测试，调用 `stopCaptureDeviceTest`。
 */

- (int)startCaptureDeviceTest:(NSView *_Nonnull)view;
/**
 停止视频采集设备测试
 停止视频采集设备测试，如果之前调用了 startCaptureDeviceTest，必须通过该方法停止测试。
 @return 0: 方法调用成功 < 0: 方法调用失败
 */
- (int)stopCaptureDeviceTest;

- (int)startAudioDeviceLoopbackTest:(int)indicationInterval;
- (int)stopAudioDeviceLoopbackTest;

#endif



@end

NS_ASSUME_NONNULL_END
