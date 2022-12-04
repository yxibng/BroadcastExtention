//
//  TSRtcChannel.h
//  TSPasS_iOS
//
//  Created by yxibng on 2020/11/4.
//

#import <Foundation/Foundation.h>
#import "TSRtcEngineKit.h"
#import "TSRtcVideoStream.h"



NS_ASSUME_NONNULL_BEGIN

@class TSRtcChannel;
@protocol TSRtcChannelDelegate;

__attribute__((visibility("default")))
@interface
TSRtcEngineKit()
/**
 创建并获取一个 TSRtcChannel 对象。通过创建多个TSRtcChannel对象，再调用各 TSRtcChannel 对象中的 joinChannel 方法，实现同时加入多个频道。
 加入多个频道后，你可以同时订阅各个频道的音、视频流；但是同一时间只能在一个频道发布一路音、视频流。
 外部必须保存返回的实例，否则该实例会被自动销毁。
 
 @param channelId 能标识频道的频道名
 @param profile 频道使用场景
 @param delegate 频道事件回调
 @return TSRtcChannel 实例
 
 */
- (TSRtcChannel *_Nullable)createRtcChannel:(NSString *_Nonnull)channelId
                                    profile:(TSChannelProfile)profile
                                   delegate:(id<TSRtcChannelDelegate>)delegate;

- (TSRtcChannel *_Nullable)createRtcChannel:(NSString *_Nonnull)channelId
                                    profile:(TSChannelProfile)profile
                                   delegate:(id<TSRtcChannelDelegate>)delegate
                                     config:(NSString *_Nullable)config;

@end


@protocol TSRtcChannelDelegate <NSObject>

@optional
/**
 频道内警告回调。
 
 @param rtcChannel TSRtcChannel 实例
 @param warningCode 警告代码
 @param message 警告描述
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel didOccurWarning:(int)warningCode message:(NSString *)message;
/**
 频道内错误回调。
 
 @param rtcChannel TSRtcChannel 实例
 @param errorCode 错误代码
 @param message 错误描述
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel didOccurError:(int)errorCode message:(NSString *)message;

/**
 该回调方法表示该客户端成功加入了指定的频道。
 @param rtcChannel TSRtcChannel 实例
 @param uid 用户 uid
 @param elapsed 从调用 sharedEngineWithConfig 方法到触发该回调的时间间隔（毫秒）。
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel didJoinChannelWithUid:(NSString *)uid elapsed:(NSInteger)elapsed;
/**
 重新加入频道回调。
 有时候由于网络原因，客户端可能会和服务器失去连接，SDK 会进行自动重连，自动重连成功后触发此回调方法。
 @param rtcChannel TSRtcChannel 实例
 @param uid 用户 uid
 @param elapsed 从调用 sharedEngineWithConfig 方法到触发该回调的时间间隔（毫秒）。
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel didRejoinChannelWithUid:(NSString *)uid elapsed:(NSInteger)elapsed;
/**
 离开频道回调。
 App 调用 leaveChannel 方法时，SDK提示 App 离开频道成功。在该回调方法中，App 可以得到此次通话的总通话时长、SDK 收发数据的流量等信息。App 通过该回调获取通话时长以及 SDK 接收到或发送的数据统计信息。
 @param rtcChannel TSRtcChannel 实例
 @param stats 通话的统计数据，详见 TSChannelStats
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel didLeaveChannelWithStats:(TSChannelStats *_Nonnull)stats;
/**
 直播场景下用户角色已切换回调。
 直播场景下，当用户切换角色时会触发此回调，即主播切换为观众时，或观众切换为主播时。
 该回调由本地用户调用 setClientRole 改变用户角色触发的。
 
 @param rtcChannel TSRtcChannel 实例
 @param oldRole 切换前的角色
 @param newRole 切换后的角色
 @param elapsed 从调用 sharedEngineWithConfig 方法到触发该回调的时间间隔（毫秒）。
 
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel didClientRoleChanged:(TSClientRole)oldRole newRole:(TSClientRole)newRole elapsed:(NSInteger)elapsed;
/**
 远端用户（通信场景）/主播（直播场景）加入当前频道回调。

 - 通信场景下，该回调提示有远端用户加入了频道，并返回新加入用户的uid；如果加入之前，已经有其他用户在频道中了，新加入的用户也会收到这些已有用户加入频道的回调。
 - 直播场景下，该回调提示有主播加入了频道，并返回该主播的uid。如果在加入之前，已经有主播在频道中了，新加入的用户也会收到已有主播加入频道的回调。我们建议连麦主播不超过 17 人。

 该回调在如下情况下会被触发：
 - 远端用户/主播调用 joinChannel 方法加入频道
 - 直播场景的观众加入频道后调用 setClientRole 将用户角色改变为主播
 
 @param rtcChannel TSRtcChannel 实例
 @param uid 新加入频道的远端用户/主播 ID
 @param elapsed 从调用 sharedEngineWithConfig 方法到触发该回调的时间间隔（毫秒）。
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel didJoinedOfUid:(NSString *_Nonnull)uid elapsed:(NSInteger)elapsed;
/**
 远端用户（通信场景）/主播（直播场景）离开当前频道回调。
 提示有远端用户/主播离开了频道（或掉线）。用户离开频道有两个原因，即正常离开和超时掉线：
 - 正常离开的时候，远端用户/主播会发送类似“再见”的消息。接收此消息后，判断用户离开频道。
 - 超时掉线的依据是，在一定时间内（通信场景为 20 秒，直播场景稍有延时），用户没有收到对方的任何数据包，则判定为对方掉线。在网络较差的情况下，有可能会误报。
 
 @param rtcChannel TSRtcChannel 实例
 @param uid 离线用户或主播的用户 ID
 @param reason 离线原因,详见`TSUserOfflineReason`
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel didOfflineOfUid:(NSString *_Nonnull)uid reason:(TSUserOfflineReason)reason;

/**
 音频发布状态改变回调。

 - joinChannel和publish都调用后会触发该回调。
 - 在已发布情况下, 调用unPublish接口会触发该回调。
 
 @param rtcChannel TSRtcChannel 实例
 @param state 当前的发布状态,详见`TSStreamPublishState`
 @param elapsed 从调用 sharedEngineWithConfig 方法到触发该回调的时间间隔（毫秒）。

 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel onAudioPublishStateChangedTo:(TSStreamPublishState)state elapsed:(NSInteger)elapsed;
/**
 音频订阅状态发生改变回调。
 
 @param rtcChannel TSRtcChannel 实例
 @param uid 远端用户的 ID
 @param state 当前的订阅状态,详见`TSStreamSubscribeState`
 @param elapsed 从调用 sharedEngineWithConfig 方法到触发该回调的时间间隔（毫秒）。
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel onAudioSubscribeStateChangedOf:(NSString *)uid newState:(TSStreamSubscribeState)state elapsed:(NSInteger)elapsed;


#pragma mark -


/**
 远端视频拉流码率预测
 @param rtcChannel TSRtcChannel 实例
 @param uid 远端用户的 ID
 @param streamName 流的名字，如果是默认流streamName为空，如果是多路流则为用户自己制定的streamName
 @param isLowVideo 是否是小流的预测
 @param newBitrate 预测的新的码率
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel onPredictedBitrateChangedOf:(NSString *)uid
                     streamName:(NSString *_Nullable)streamName
                     isLowVideo:(BOOL)isLowVideo
                     newBitrate:(int)newBitrate;
/**
 视频发布状态改变回调。

 - joinChannel和publish都调用后会触发该回调。
 - 在已发布情况下, 调用unPublish接口会触发该回调。
 
 @param rtcChannel TSRtcChannel 实例
 @param state 当前的发布状态,详见`TSStreamPublishState`
 @param elapsed 从调用 sharedEngineWithConfig 方法到触发该回调的时间间隔（毫秒）。

 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel onVideoPublishStateChangedTo:(TSStreamPublishState)state elapsed:(NSInteger)elapsed;
/**
 视频订阅状态发生改变回调。
 
 @param rtcChannel TSRtcChannel 实例
 @param uid 远端用户的 ID
 @param streamName 流的名字，如果是默认流streamName为空，如果是多路流则为用户自己制定的streamName
 @param state 当前的订阅状态,详见`TSStreamSubscribeState`
 @param elapsed 从调用 sharedEngineWithConfig 方法到触发该回调的时间间隔（毫秒）。
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel onVideoSubscribeStateChangedOf:(NSString *)uid streamName:(NSString *)streamName newState:(TSStreamSubscribeState)state elapsed:(NSInteger)elapsed;
/**
 已显示首帧远端视频回调。
 第一帧远端视频显示在视图上时，触发此调用。 App 可在此调用中获知出图时间（elapsed）。
 
 @param rtcChannel TSRtcChannel 实例
 @param uid 远端用户的 ID
 @param streamName 流的名字，如果是默认流streamName为空，如果是多路流则为用户自己制定的streamName
 @param size 视频流分辨率
 @param elapsed 从调用 sharedEngineWithConfig 方法到触发该回调的时间间隔（毫秒）。
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel firstRemoteVideoFrameOfUid:(NSString *)uid streamName:(NSString *)streamName size:(CGSize)size elapsed:(NSInteger)elapsed;
/**
 远端视频大小发生改变回调。
 
 @param rtcChannel TSRtcChannel 实例
 @param uid 远端用户的 ID
 @param streamName 流的名字，如果是默认流streamName为空，如果是多路流则为用户自己制定的streamName
 @param size 视频流分辨率
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel videoSizeChangedOfUid:(NSString *)uid streamName:(NSString *)streamName size:(CGSize)size;
/**
 
 通话中每个远端用户的网络上下行 last mile 质量报告回调。
 该回调描述每个用户在通话中的 last mile 网络状态，其中 last mile 是指设备到边缘服务器的网络状态。
 该回调每 2 秒触发一次。如果远端有多个用户，该回调每 2 秒会被触发多次。
 
 @param rtcChannel TSRtcChannel 实例
 @param uid 用户 ID。表示该回调报告的是持有该 ID 的用户的网络质量。
 @param txQuality 该用户的上行网络质量，基于发送码率、上行丢包率、平均往返时延和网络抖动计算。该值代表当前的上行网络质量，帮助判断是否可以支持当前设置的视频编码属性。假设上行带宽是 1000 Kbps，那么支持直播场景下 640 × 480 的分辨率、15 fps 的帧率没有问题，但是支持 1280 × 720 的分辨率就会有困难。详见 TSNetworkQuality 。
 @param rxQuality 该用户的下行网络质量，基于下行网络的丢包率、平均往返延时和网络抖动计算。详见 TSNetworkQuality.
 
 
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel networkQualityOfUid:(NSString *)uid txQuality:(TSNetworkQuality)txQuality rxQuality:(TSNetworkQuality)rxQuality;

/**

 网络连接丢失回调。
 SDK 在调用 joinChannel 后无论是否加入成功，只要 10 秒和服务器无法连接就会触发该回调。
 如果 SDK 在断开连接后，20 分钟内还是没能重新加入频道，SDK 会停止尝试重连。
 @param rtcChannel TSRtcChannel 实例
 
 */
- (void)rtcChannelConnectionDidLost:(TSRtcChannel *_Nonnull)rtcChannel;

/**
 网络连接状态已改变回调。
 该回调在网络连接状态发生改变的时候触发，并告知用户当前的网络连接状态和引起网络状态改变的原因。
 
 @param rtcChannel TSRtcChannel 实例
 @param state 连接状态，详见`TSConnectionStateType`
 @param reason 变更的原因，详见 `TSConnectionChangedReason`
 
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel connectionChangedToState:(TSConnectionStateType)state reason:(TSConnectionChangedReason)reason;

/**
 当前通话统计回调。
 SDK 定期向 App 报告当前通话的统计信息，每两秒触发一次。
 @param rtcChannel TSRtcChannel 实例
 @param stats 统计数据，详见`TSChannelStats`
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel reportRtcStats:(TSChannelStats *_Nonnull)stats;
/**
 通话中远端音频流的统计信息回调。
 该回调描述远端用户在通话中端到端的音频流统计信息。该回调函数针对每个发送音频流的远端用户/主播每 2 秒触发一次。如果远端有多个用户/主播发送音频流，该回调每 2 秒会被触发多次。
 @param rtcChannel TSRtcChannel 实例
 @param stats 远端音频统计数据, 详见`TSRtcRemoteAudioStats`
 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel remoteAudioStats:(TSRtcRemoteAudioStats *_Nonnull)stats;
/**
 通话中远端视频流的统计信息回调。
 该回调描述远端用户在通话中端到端的视频流统计信息， 针对每个远端用户/主播每 2 秒触发一次。如果远端同时存在多个用户/主播， 该回调每 2 秒会被触发多次。
 @param rtcChannel TSRtcChannel 实例
 @param stats  远端视频统计数据, 详见`TSRtcRemoteVideoStats`

 */
- (void)rtcChannel:(TSRtcChannel *_Nonnull)rtcChannel remoteVideoStats:(TSRtcRemoteVideoStats *_Nonnull)stats;


#pragma mark -
- (void)rtcChannel:(TSRtcChannel *)rtcChannel remoteVideoOnlineWithUid:(NSString *)uid streamName:(NSString *)streamName metaInfo:(NSDictionary *)metaInfo;
- (void)rtcChannel:(TSRtcChannel *)rtcChannel remoteVideoMetaInfoUpdateWithUid:(NSString *)uid streamName:(NSString *)streamName metaInfo:(NSDictionary *)metaInfo;

- (void)rtcChannel:(TSRtcChannel *)rtcChannel remoteAudioOnlineWithUid:(NSString *)uid metaInfo:(NSDictionary *)metaInfo;
- (void)rtcChannel:(TSRtcChannel *)rtcChannel remoteAudioMetaInfoUpdateWithUid:(NSString *)uid metaInfo:(NSDictionary *)metaInfo;

@end


__attribute__((visibility("default")))
@interface TSRtcChannel : NSObject

@property (weak, nonatomic) id<TSRtcChannelDelegate> delegate;

/**
 销毁 TSRtcChannel 对象
 @return 0 方法调用成功，<0 方法调用失败
 */
- (int)destroy;

#pragma mark -
/**
 通过 UID 加入频道。

 通过该方法加入频道后，SDK 默认不发布本地音视频流到本频道，用户需要调用 [TSRtcChannel publish] "publish" 方法进行发布。
 @param uid 用户ID，频道内需要保持唯一。
 @return 0 方法调用成功，<0 方法调用失败
 
 注意：
  - 该方法不支持相同的用户重复加入同一个频道。
  - 我们建议不同频道中使用不同的 UID。
  - 如果想要从不同的设备同时接入同一个频道，请确保每个设备上使用的 UID 是不同的。
 */
- (int)joinChannelByUid:(NSString *_Nonnull)uid;
/**
 离开频道，即挂断或退出通话。

 当调用 joinChannel 方法后，必须调用 leaveChannel 结束通话，否则无法开始下一次通话。 不管当前是否在通话中，都可以调用 leaveChannel，没有副作用。该方法会把会话相关的所有资源释放掉。 该方法是异步操作，调用返回时并没有真正退出频道。在真正退出频道后，SDK 会触发 onLeaveChannel 回调。 成功调用该方法离开频道后，本地会触发 didLeaveChannelWithStats 回调；通信场景下的用户和直播场景下的主播离开频道后，远端会触发 didOfflineOfUid 回调。
 
 @return 0 方法调用成功，<0 方法调用失败
 */
- (int)leaveChannel;

/**
 设置直播场景下的用户角色。

 在加入频道前，用户需要通过本方法设置观众（默认）或主播。在加入频道后，用户可以通过本方法切换用户角色。
 直播场景下，如果你在加入频道后调用该方法切换用户角色，调用成功后，本地会触发 didClientRoleChanged 回调；远端会触发 didJoinOfUid 回调或 didOfflineOfUid (BECOME_AUDIENCE) 回调。
 
 @param role 直播场景里的用户角色, 详见`TSClientRole`
 @return 0 方法调用成功，<0 方法调用失败
 注意：
 - 该方法仅适用于直播场景。通信场景调用无效
 - 如果从观众切换到主播，此时需要发布流，需要调用publish接口。
 */
- (int)setClientRole:(TSClientRole)role;

#pragma mark -
/**
 设置是否默认接收所有音频流。

 该方法在加入频道前后都可调用。如果在加入频道后调用 setDefaultMuteAllRemoteAudioStreams (YES)，会接收不到设置后加入频道的用户的音频流。

 停止接收音频流后，如果想要恢复接收，请调用 muteRemoteAudioStream (NO)， 并指定你想要接收的远端用户 uid；如果想恢复接收多个用户的音频流，则需要多次调用 muteRemoteAudioStream。 setDefaultMuteAllRemoteAudioStreams (NO) 只能恢复接收后面加入频道的用户的音频流。
 @param mute  YES: 默认停止接收所有音频流；NO: 默认接收所有音频流（默认）。
 @return 0 方法调用成功，<0 方法调用失败

 */
- (int)setDefaultMuteAllRemoteAudioStreams:(BOOL)mute;
/**
 接收／停止接收当前所有音频流。如果是停止接收, 则后续上线的音频流也都不会接收。
 
 @param mute YES: 停止接收所有音频流；NO: 继续接收所有音频流（默认）。
 @return 0 方法调用成功，<0 方法调用失败
 */
- (int)muteAllRemoteAudioStreams:(BOOL)mute;
/**
 接收／停止接收指定音频流。
 如果之前有调用过 muteAllRemoteAudioStreams (true) 停止订阅所有远端音频，在调用本 API 之前请确保你已调用 muteAllRemoteAudioStreams (false)。 muteAllRemoteAudioStreams 是全局控制，muteRemoteAudioStream 是精细控制。
 @param uid 指定用户的 ID
 @param mute YES: 停止接收指定音频流；NO: 继续接收指定音频流（默认）。
 @return 0 方法调用成功，<0 方法调用失败
 */
- (int)muteRemoteAudioStream:(NSString *_Nonnull)uid
                        mute:(BOOL)mute;


#pragma mark -
/**
 
 设置是否默认停止接收视频流。
 该方法在加入频道前后都可调用。如果在加入频道后调用 setDefaultMuteAllRemoteVideoStreams (true)，会接收不到设置后加入频道的用户的视频流。

 停止接收视频流后，如果想要恢复接收，请调用 muteRemoteVideoStream (false)， 并指定你想要接收的远端用户 uid；如果想恢复接收多个用户的视频流，则需要多次调用 muteRemoteVideoStream。setDefaultMuteAllRemoteVideoStreams (false) 只能恢复接收后面加入频道的用户的视频流。
 
 
 @param mute true: 默认停止接收所有远端视频；false: 默认接收所有远端视频（默认）。
 @return 0 方法调用成功，<0 方法调用失败
 */
- (int)setDefaultMuteAllRemoteVideoStreams:(BOOL)mute;
/**
 接收／停止接收当前所有远端视频流。 如果是停止接收, 则后续上线的视频流也都不会接收。
 如果是停止接收所有远端视频流，则后续上线的视频流都不会接收。
 
 @param mute  true: 停止接收所有远端视频流；false: 允许接收所有远端视频流（默认）。
 @return 0 方法调用成功，<0 方法调用失败
 */
- (int)muteAllRemoteVideoStreams:(BOOL)mute;
/**
 接收／停止接收指定远端用户的视频流。
 如果之前调用过 muteAllRemoteVideoStreams (true) 停止接收放所有远端视频流，在调用本 API 之前请确保你已调用 muteAllRemoteVideoStreams (false) 。muteAllRemoteVideoStreams 是全局控制，muteRemoteVideoStream 是精细控制。
 @param uid 指定用户的 ID
 @param streamName 指定用户的流的名字。如果是默认流,该字段为空。如果是通过IRtcStream推送的流，则需要填写对应的streamName。
 @param mute  true: 停止接收指定远端用户的视频流；false: 允许接收指定远端用户的视频流（默认）。
 @return 0 方法调用成功，<0 方法调用失败
 */
- (int)muteRemoteVideoStream:(NSString *_Nonnull)uid
                  streamName:(NSString *_Nullable)streamName
                        mute:(BOOL)mute;


/**
 该方法绑定远端用户和显示视图，并设置远端用户视图在本地显示时的渲染模式和镜像模式，只影响本地用户看到的视频画面。
 调用该接口时需要指定远端视频的 uid 和 streamName，可以在进频道前提前设置好。
 如果 App 不能事先知道对方的 uid，可以在 APP 收到 onUserJoined 事件时设置。
 
 @param canvas 远端用户视频画布信息， 详见TSRtcVideoCanvas
 @return 0 方法调用成功，<0 方法调用失败
 
 */
- (int)setupRemoteVideo:(TSRtcVideoCanvas *_Nonnull)canvas;


/**
 自定义远端渲染视图
 
 @param videoRenderer 自定义的视频渲染器，详见 TSVideoSinkProtocol
 @param uid 指定用户的 ID
 @param streamName 指定用户的流的名字。如果是默认流,该字段为空。如果是通过TSRtcVideoStream推送的流，则需要填写对应的streamName
 @return 0 方法调用成功，<0 方法调用失败
 */
- (int)setRemoteVideoRenderer:(id<TSVideoSinkProtocol> _Nullable)videoRenderer
                          uid:(NSString *_Nonnull)uid
                   streamName:(NSString *_Nullable)streamName;

/**
 设置默认订阅的视频流类型。

 在网络条件受限的情况下，如果发送端没有调用 enableDualStreamMode (false) 关闭双流模式， 接收端可以选择接收大流还是小流。其中，大流可以接为高分辨率高码率的视频流， 小流则是低分辨率低码率的视频流。

 正常情况下，用户默认接收大流。如需默认接收所有用户的视频小流，可以调用本方法进行切换。SDK 会根据视频窗口的大小动态调整对应视频流的大小，以节约带宽和计算资源。视频小流默认 的宽高比和视频大流的宽高比一致。根据当前大流的宽高比，系统会自动分配小流的分辨率、帧率及码率。
 
 @param streamType 视频流类型,详见 `TSVideoStreamType`
 @return 0 方法调用成功，<0 方法调用失败
 */
- (int)setRemoteDefaultVideoStreamType:(TSVideoStreamType)streamType;

/**
 更新远端视图显示模式。
 初始化远端用户视图后，你可以调用该方法更新远端用户视图在本地显示时的渲染和镜像模式。该方法只影响本地用户看到的视频画面。
 
 @param uid 指定用户的 ID
 @param streamName 指定用户的流的名字。如果是默认流,该字段为空。如果是通过TSRtcVideoStream推送的流，则需要填写对应的streamName
 @param renderMode 本地视图的渲染模式, 详见 TSVideoRenderMode
 @param mirrorMode 本地视图的镜像模式，Note: 如果你使用前置摄像头，默认启动本地用户视图镜像模式；如果你使用后置摄像头，默认关闭本地视图镜像模式。
 @return 0 方法调用成功，<0 方法调用失败
 
 注意：
 - 请在调用 setupRemoteVideo 方法初始化远端视图后，调用该方法。
 - 你可以在通话中多次调用该方法，多次更新远端用户视图的显示模式。
 
 */
- (int)setRemoteRenderModeForUser:(NSString *_Nonnull)uid
                       streamName:(NSString *_Nullable)streamName
                       renderMode:(TSVideoRenderMode)renderMode
                       mirrorMode:(TSVideoMirrorMode)mirrorMode;

/**
 设置订阅的视频流类型。

 在网络条件受限的情况下，如果发送端没有调用 enableDualStreamMode (false) 关闭双流模式， 接收端可以选择接收大流还是小流。其中，大流可以接为高分辨率高码率的视频流， 小流则是低分辨率低码率的视频流。

 正常情况下，用户默认接收大流。如需接收小流，可以调用本方法进行切换。SDK 会根据视频窗口的大小动态调整对应视频流的大小，以节约带宽和计算资源。 视频小流默认的宽高比和视频大流的宽高比一致。根据当前大流的宽高比，系统会自动分配小流的分辨率、帧率及码率。
 
 @param uid 指定用户的 ID
 @param streamName 指定用户的流的名字。如果是默认流,该字段为空。如果是通过TSRtcVideoStream推送的流，则需要填写对应的streamName
 @param streamType 视频流类型,详见 `TSVideoStreamType`
 @return 0 方法调用成功，<0 方法调用失败
 
 */
- (int)setRemoteVideoStreamTypeForUser:(NSString *_Nonnull)uid
                            streamName:(NSString *_Nullable)streamName
                            streamType:(TSVideoStreamType)streamType;


/**
 创建并获取一个TSRtcVideoStream对象。
 你可以通过多次调用该方法，创建多个TSRtcVideoStream对象，在调用各TSRtcVideoStream对象的publish方法实现在一个频道里面推送多个视频流。
 外部必须保存返回的实例，否则该实例会被自动销毁。
 
 @param source 自定义推流视频源， 详见`TSVideoSourceProtocol`
 @param streamName 指定用户的流的名字。
 @param bufferType 视频流类型,详见 `TSVideoBufferType`
 @param delegate  监听和报告指定视频流的事件
 @param encodeConfig 视频编码参数，详见`TSVideoEncoderConfiguration`
 @return TSRtcVideoStream 实例
 */
- (TSRtcVideoStream *_Nullable)setupCustomVideoSource:(id<TSVideoSourceProtocol> _Nonnull)source
                                           streamName:(NSString *_Nonnull)streamName
                                           bufferType:(TSVideoBufferType)bufferType
                                             delegate:(id<TSRtcVideoStreamDelegate> _Nullable)delegate
                                         encodeConfig:(TSVideoEncoderConfiguration *_Nonnull)encodeConfig;



#pragma mark -
/**
 将本地音视频流发布到本频道。

 - 该方法仅支持将音视频流发布到当前 IRtcChannel类所对应的频道。
 - 直播场景下，该方法仅适用于角色为主播的用户。观众角色的用户调用publish无效。你可以调用该 IRtcChannel 类下的 setClientRole 方法设置用户角色。
 - SDK 只支持用户同一时间在一个频道发布本地音视频流。如果重复调用，会返回 ERR_REFUSED
 @return 0 方法调用成功，<0 方法调用失败
 
 */
- (int)publish;
/**
 停止将本地音视频流发布到本频道。
 当前IRtcChannel未调用过publish接口时, 调用unPublish接口无效， 会返回 ERR_REFUSED
 @return 0 方法调用成功，<0 方法调用失败
 */
- (int)unpublish;



- (int)publish:(TSPublishMediaType)mediaType;
- (int)unpublish:(TSPublishMediaType)mediaType;

/**
 同步获取网络状态
 @return 当前网络状况，参考：TSConnectionStateType
 */
- (TSConnectionStateType)getConnectionState;

@end

NS_ASSUME_NONNULL_END
