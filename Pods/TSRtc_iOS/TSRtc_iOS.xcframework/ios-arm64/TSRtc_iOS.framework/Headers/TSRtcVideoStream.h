//
//  TSRtcVideoStream.h
//  TSRtc_macOS
//
//  Created by yxibng on 2020/11/11.
//

#import <Foundation/Foundation.h>
#import "TSObjects.h"
#import "TSEnumerates.h"


NS_ASSUME_NONNULL_BEGIN

@class TSRtcVideoStream;

/**
 TSRtcVideoStreamDelegate 类监听和报告指定视频流的事件和数据。
 */
@protocol TSRtcVideoStreamDelegate <NSObject>

@optional
/**
 
 视频发布状态改变回调。

 SDK 会在以下时机触发该回调：

 - joinChannel和publish都调用后会触发该回调。
 - 在已发布情况下, 调用unPublish接口会触发该回调。
 
 @param videoStream TSRtcVideoStream 实例
 @param state 当前的发布状态，详见 TSStreamPublishState
 @param elapsed 从调用 `sharedEngineWithConfig` 方法到触发该回调的时间间隔（毫秒）。
 */
- (void)videoStream:(TSRtcVideoStream *)videoStream onVideoPublishStateChangedTo:(TSStreamPublishState)state elapsed:(NSInteger)elapsed;
/**
 本地视频状态发生改变回调
 本地视频的状态发生改变时，SDK 会触发该回调返回当前的本地视频状态。
 在本地视频出现故障时，你可以通过该回调了解当前视频的状态以及出现故障的原因，方便排查问题。
 @param videoStream TSRtcVideoStream 实例
 @param state 本地视频状态，详见 TSLocalVideoStreamState
 @param elapsed 从调用 `sharedEngineWithConfig` 方法到触发该回调的时间间隔（毫秒）。
 */
- (void)videoStream:(TSRtcVideoStream *)videoStream onLocalVideoStateChangedTo:(TSLocalVideoStreamState)state elapsed:(NSInteger)elapsed;
/**
 已发布当前视频首帧回调。
 SDK 会在以下时机触发该回调：
 - joinChannel和publish都调用后会触发该回调。
 
 @param videoStream TSRtcVideoStream 实例
 @param elapsed 从调用 `sharedEngineWithConfig` 方法到触发该回调的时间间隔（毫秒）。
 */
- (void)videoStream:(TSRtcVideoStream *)videoStream onFirstVideoFramePublishedElapsed:(NSInteger)elapsed;
/**
 当前视频大小发生改变回调。
 
 @param videoStream TSRtcVideoStream 实例
 @param size 视频流的分辨率大小。
 */
- (void)videoStream:(TSRtcVideoStream *)videoStream onVideoSizeChanged:(CGSize)size;

/**
 本地视频推流，预测的新的码率
 @param videoStream TSRtcVideoStream 实例
 @param newBitrate 预测的新的码率
 @param isLowVideo 是否预测的是小流
 */
- (void)videoStream:(TSRtcVideoStream *)videoStream onPredictedBitrateChanged:(int)newBitrate isLowVideo:(BOOL)isLowVideo;

@end




__attribute__((visibility("default"))) @interface TSRtcVideoStream : NSObject
/**
 接收推流状态变更的代理对象
 */
@property (nonatomic, weak)id<TSRtcVideoStreamDelegate>delegate;
/**
 获取当前视频流的流名字。为空返回 长度为0的字符串
 */
@property (nonatomic, copy, readonly, nonnull) NSString *streamName;
/**
 获取当前视频流所属的频道的频道Id。为空返回 长度为0的字符串
 */
@property (nonatomic, copy, readonly, nonnull) NSString *channelId;
/**
 销毁 TSRtcVideoStream 对象。
 @return  0: 方法调用成功, <0 方法调用失败
 
 注意：
 - 销毁之后，不可以再调用 TSRtcVideoStream 的任何方法。
 
 */
- (int)destroy;
/**
 将当前视频流发布到频道内。

 该方法的调用需满足以下要求，否则 SDK 会返回 ERR_REFUSED (3) :

 - 该方法将当前视频流发布到该TSRtcVideoStream所属的频道。
 - 直播场景下，该方法仅适用于角色为主播的用户。你可以调用该 IChannel 类下的 setClientRole 方法设置用户角色。
 @return  0: 方法调用成功, <0 方法调用失败
 
 */
- (int)publish;
/**
 停止将当前视频流发布到频道内。
 当前视频流未调用过publish接口时, 调用unPublish接口无效， SDK会返回 ERR_REFUSED
 @return  0: 方法调用成功, <0 方法调用失败
 */
- (int)unPublish;
/**
 开关视频发送。

 当视频流已经发布到对应频道内时，成功调用该方法后, 本端会触发`onLocalVideoStateChanged`回调, 远端用户会触发`onVideoSubscribeStateChanged`回调
 
 @param mute YES: 不发送本地视频流; NO: 发送本地视频流（默认）。
 @return  0: 方法调用成功, <0 方法调用失败
 注意：
 - 如果在该方法后调用`setClientRole`方法, SDK会根据你设置的角色，重新设置是否停止发送视频流。
 */
- (int)muteVideoStream:(BOOL)mute;
/**
 
 开关双流模式。

 该方法设置单流（默认）或者双流模式。发送端开启双流模式后，接收端可以选择接收大流还是小流。其中，大流指高分辨率、高码率的视频流，小流指低分辨率、低码率的视频流。
 @param enabled YES: 双流。NO: 单流（默认)。
 @return  0: 方法调用成功, <0 方法调用失败
 */
- (int)enableDualSteamMode:(BOOL)enabled;
/**
 设置视频编码属性。
 @param config  视频编码参数配置。详见 `TSVideoEncoderConfiguration`
 @return  0: 方法调用成功, <0 方法调用失败
 */
- (int)setVideoEncoderConfiguration:(TSVideoEncoderConfiguration *)config;


@end





NS_ASSUME_NONNULL_END
