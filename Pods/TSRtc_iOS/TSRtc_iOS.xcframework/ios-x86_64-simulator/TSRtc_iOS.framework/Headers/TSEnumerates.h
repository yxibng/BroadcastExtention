//
//  TSEnumerates.h
//  TSPasS_iOS
//
//  Created by yxibng on 2020/11/4.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, TSWarningCode) {
    /**
     音频设备模块：运行时录音设备出现警告。
     例如：iOS 播放或录制音频时被系统事件（如来电）干扰。
     */
    TSWarningCodeAdmRuntimeRecordingWarning = 1021,
    /**
     音频设备模块：运行时播放设备出现警告。
     */
    TSWarningCodeAdmRuntimePlaybackWarning = 1061,
    /**
     iOS系统不允许后台使用摄像头。iOS开启摄像头之后，切换到后台，摄像头会被系统关闭。 重新切换到前台，摄像头会被重新打开。
     */
    TSWarningCodeVpmRuntimeVideoCaptureWarning = 1511,
    /**
     对端不存在小流
     */
    TSWarningCodeVpmDualNoLowStream = 3041,
    /**
     切换小流失败
     */
    TSWarningCodeVpmDualSwitchLowFailed = 3042,
    /**
     切换大流失败
     */
    TSWarningCodeVpmDualSwitchHighFailed = 3043,
};


typedef NS_ENUM(NSInteger, TSErrorCode) {
    /**
     无错误
     */
    TSErrorCodeNoError = 0,
    /**
     一般性的错误（没有明确归类的错误原因）。
     例如，已经选择自定义渲染，此时调用setLocalVideoCanvas，返回错误
     */
    TSErrorCodeFailed = 1,
    /**
     API 调用了无效的参数。例如指定的频道Id含有非法字符。
     1、createChannel 参数错误 （rtcEngine）
     2、createVideoStream 参数错误 （Channel)
     */
    TSErrorCodeInvalidArgument = 2,
    /**
     调用被拒绝
     1、重复joinChannel
     2、重复leaveChannel
     3、重复publish/unPublish 等
     4、没有权限的指令(观众的一些操作)
     5、通信模式切换角色
     */
    TSErrorCodeRefused = 3,
    /**
     RtcEngine 尚未初始化就调用方法。请确认在调用该方法前已创建 RtcEngine 对象，并完成初始化。
     */
    TSErrorCodeNotInitialized = 4,
    /**
     加入频道被拒绝。一般有以下原因：用户已进入频道，再次调用加入频道的 API，例如 joinChannel ，会返回此错误。停止调用该方法即可
     */
    TSErrorCodeJoinChannelRejected = 5,
    /**
     离开频道失败。一般有以下原因：
     1、用户已离开频道，再次调用退出频道的 API，例如 leaveChannel ，会返回此错误。停止调用该方法即可。
     2、用户尚未加入频道，就调用退出频道的 API。这种情况下无需额外操作。
     */
    TSErrorCodeLeaveChannelRejected = 6,
    
    /**
     查找调度服务器超时。在加入频道时 SDK 先要查找调度服务，出现该警告一般是因为网络太差，连接不到服务器。
     */
    TSErrorCodeLookupScheduleServerTimeout = 101,
    /**
     不是有效的 appId。请更换有效的 appId 重新加入频道。
     */
    TSErrorCodeInvalidAppId = 102,
    /**
     不是有效的channelId。请更换有效的channelId重新加入频道。
     */
    TSErrorCodeInvalidChannelId = 103,
    /**
     查找调度服务器失败
     */
    TSErrorCodeLookupScheduleServerFailed = 104,
    /**
     无法获取当前指定区域的调度服务器资源。请在调用 initialize 方法时尝试指定其他区域
     */
    TSErrorCodeNoScheduleServerResources = 105,
    /**
     查找服务超时, 进入频道前需要找调度服务器分配资源
     */
    TSErrorCodeLookupServerTimeout = 106,
    /**
     没有可用的服务资源
     */
    TSErrorCodeNoAvailableServerResources = 107,
    /**
     不是有效的streamId。请更换有效的streamId
     */
    TSErrorCodeInvalidStreamID = 108,
    
    /**
     音频设备模块：音频设备出现错误（未明确指明为何种错误）。请检查音频设备是否被其他应用占用，或者尝试重新进入频道。
     */
    TSErrorCodeAdmGeneralError = 1001,
    /**
     音频设备模块：设置的采样频率出现错误。
     */
    TSErrorCodeAdmSampleRate = 1017,
    /**
     音频设备模块：停止录音设备出现错误。
     */
    TSErrorCodeAdmStopRecordingFailed = 1031,
    /**
     音频设备模块：无录制设备。请检查是否有可用的录音设备或者录音设备是否已经被其他应用占用。
     */
    TSErrorCodeAdmNoRecordingDevice = 1036,
    /**
     音频设备模块：运行时播放出现错误。请检查播放设备是否正常，或者尝试重新进入频道。
     */
    TSErrorCodeAdmRuntimePlaybackError = 1062,
    /**
     音频设备模块：停止播放设备出现错误。
     */
    TSErrorCodeAdmStopPlaybackFailed = 1071,
    /**
     音频设备模块：无播放设备。
     */
    TSErrorCodeAdmNoPlaybackDevice = 1076,
    /**
     视频设备模块：运行时采集出现错误。请检查采集设备是否正常，或者尝试重新进入频道。
     */
    TSErrorCodeVdmRuntimeVideoCaptureError = 1512,
    /**
     视频采集模块停止失败
     */
    TSErrorCodeVdmStopCaptureFailed = 1531,
    /**
     视频设备模块：无摄像头。请检查是否有可用的摄像头或者摄像头是否已经被其他应用占用。
     */
    TSErrorCodeVdmNoCamera = 1541,
};

/** 日志级别 */
typedef NS_ENUM(NSUInteger, TSLogLevel) {
    TSLogLevelOff = 0,     //不输出文件
    TSLogLevelDebug = 1,   //输出所有 API 日志信息。 如果你想获取最完整的日志，可以将日志级别设为该等级。
    TSLogLevelInfo = 2,    //输出 FATAL、ERROR、WARNING 和 INFO 级别的日志信息。 我们推荐你将日志级别设为该等级。
    TSLogLevelWarn = 3,    //输出 FATAL、ERROR 和 WARNING 级别的日志信息。
    TSLogLevelError = 4,   //输出 FATAL 和 ERROR 级别的日志信息。
    TSLogLevelFatal = 5    //输出 FATAL 级别的日志信息。
};



typedef NS_ENUM(NSInteger, TSChannelProfile) {
    //通信场景。该场景下，频道内所有用户都可以发布和接收音、视频流。适用于语音通话、视频群聊等应用场景。
    TSChannelProfileCommunication = 0,
    //直播场景。该场景有主播和观众两种用户角色，可以通过 setClientRole 设置。主播可以发布和接收音视频流，观众直接接收流。适用于语聊房、视频直播、互动大班课等应用场景。
    TSChannelProfileLiveBroadcasting = 1,
};

typedef NS_ENUM(NSInteger, TSClientRole) {
    //主播。主播可以发流也可以收流。
    TSClientRoleBroadcaster = 1,
    //（默认）观众。观众只能收流不能发流。
    TSClientRoleAudience = 2
};


typedef NS_ENUM(NSInteger, TSVideoStreamType) {
    //视频大流
    TSVideoStreamTypeHigh,
    //视频小流
    TSVideoStreamTypeLow,
};


typedef NS_ENUM(NSInteger, TSAudioScenario) {
    //0: 默认设置。
    TSAudioScenarioDefault = 0,
    //1: 娱乐应用，需要频繁上下麦的场景。
    TSAudioScenarioChatRoomEntertainment = 1,
    //2: 教育应用，流畅度和稳定性优先。
    TSAudioScenarioEducation = 2,
    //3: 高音质语聊房应用。
    TSAudioScenarioGameStreaming = 3,
    //4: 秀场应用，音质优先和更好的专业外设支持。
    TSAudioScenarioShowRoom = 4,
    //5: 游戏开黑。
    TSAudioScenarioChatRoomGaming = 5
};


typedef NS_ENUM(NSInteger, TSConnectionStateType) {
    //1: 网络连接断开。
    TSConnectionStateDisconnected = 1,
    //2: 建立网络连接中。
    TSConnectionStateConnecting = 2,
    //3: 网络已连接。
    TSConnectionStateConnected = 3,
    //4: 重新建立网络连接中。
    TSConnectionStateReconnecting = 4,
    //5: 网络连接失败。
    TSConnectionStateFailed = 5,
};


typedef NS_ENUM(NSUInteger, TSVideoMirrorMode) {
    /**
     (默认）由 SDK 决定镜像模式。
     */
    TSVideoMirrorModeAuto = 0,
    /**
     启用镜像模式。
     */
    TSVideoMirrorModeEnabled = 1,
    /**
     关闭镜像模式。
     */
    TSVideoMirrorModeDisabled = 2,
};


typedef NS_ENUM(NSInteger, TSVideoOutputOrientationMode) {
    /**
     (默认）该模式下 SDK 输出的视频方向与采集到的视频方向一致。接收端会根据收到的视频旋转信息对视频进行旋转。该模式适用于接收端可以调整视频方向的场景。如果采集的视频是横屏模式，则输出的视频也是横屏模式；如果采集的视频是竖屏模式，则输出的视频也是竖屏模式。
     */
    TSVideoOutputOrientationModeAdaptative = 0,
    /**
     该模式下 SDK 固定输出风景（横屏）模式的视频。如果采集到的视频是竖屏模式，则视频编码器会对其进行裁剪。该模式适用于当接收端无法调整视频方向时，如使用旁路推流场景下。
     */
    TSVideoOutputOrientationModeFixedLandscape = 1,
    /**
     该模式下 SDK 固定输出人像（竖屏）模式的视频，如果采集到的视频是横屏模式，则视频编码器会对其进行裁剪。该模式适用于当接收端无法调整视频方向时，如使用旁路推流场景下。
     */
    TSVideoOutputOrientationModeFixedPortrait = 2,
};


typedef NS_ENUM(NSUInteger, TSVideoRenderMode) {
    //视频尺寸等比缩放。优先保证视窗被填满。因视频尺寸与显示视窗尺寸不一致而多出的视频将被截掉。
    TSVideoRenderModeHidden = 1,
    //视频尺寸等比缩放。优先保证视频内容全部显示。因视频尺寸与显示视窗尺寸不一致造成的视窗未被填满的区域填充黑色。
    TSVideoRenderModeFit = 2,
    //视频尺寸进行缩放和拉伸以充满显示视窗。
    TSVideoRenderModeFill = 3,
};

/** Video frame rate */
typedef NS_ENUM(NSInteger, TSVideoFrameRate) {
    /** 1 fps. */
    TSVideoFrameRateFps1 = 1,
    /** 7 fps. */
    TSVideoFrameRateFps7 = 7,
    /** 10 fps. */
    TSVideoFrameRateFps10 = 10,
    /** 15 fps. */
    TSVideoFrameRateFps15 = 15,
    /** 24 fps. */
    TSVideoFrameRateFps24 = 24,
    /** 30 fps. */
    TSVideoFrameRateFps30 = 30,
    /** 60 fps (macOS only). */
    TSVideoFrameRateFps60 = 60,
};

typedef NS_ENUM(NSInteger, TSBitrateControlType) {
    /**
     固定码率  软编码支持
     */
    TSBitrateControlType_CBR,
    /**
     可变码率 软编码支持
     */
    TSBitrateControlType_VBR,
    /**
     平均码率 硬编码支持
     */
    TSBitrateControlType_ABR
};


typedef NS_ENUM(NSInteger, TSVideoPixelFormat) {
    //-1: 无效接口
    TSVideoPixelFormatUnavailable = -1,
    /** I420 */
    TSVideoPixelFormatI420,
    /** BGRA */
    TSVideoPixelFormatBGRA,
    /** NV21 */
    TSVideoPixelFormatNV21,
    /** BGRA */
    TSVideoPixelFormatRGBA,
    /** IMC2 */
    TSVideoPixelFormatIMC2,
    /** ARGB */
    TSVideoPixelFormatARGB,
    /** NV12 */
    TSVideoPixelFormatNV12,
    /** I422 */
    TSVideoPixelFormatI422,
};

/** Video buffer type */
typedef NS_ENUM(NSInteger, TSVideoBufferType) {
    //原始数据
    TSVideoBufferTypeRawData = 1,
    //h264
    TSVideoBufferTypeH264 = 2,
    //用户自定义数据流
    TSVideoBufferTypeCustom = 3,
};

/** Audio profile. */
typedef NS_ENUM(NSInteger, TSAudioProfile) {
    //0: 默认值。直播场景下：48 KHz 采样率，音乐编码，单声道，编码码率最大值为 64 Kbps。通信场景下：Windows 平台：16 KHz 采样率，音乐编码，单声道，编码码率最大值为 16 Kbps。Android、macOS、iOS 平台：32 KHz 采样率，音乐编码，单声道，编码码率最大值为 18 Kbps。
    TSAudioProfileDefault = 0,
    //1: 指定 32 KHz 采样率，语音编码，单声道，编码码率最大值为 18 Kbps。
    TSAudioProfileSpeechStandard = 1,
    //2: 指定 48 KHz 采样率，音乐编码，单声道，编码码率最大值为 64 Kbps。
    TSAudioProfileMusicStandard = 2,
    //3: 指定 48 KHz 采样率，音乐编码，双声道，编码码率最大值为 80 Kbps。
    TSAudioProfileMusicStandardStereo = 3,
    //4: 指定 48 KHz 采样率，音乐编码，单声道，编码码率最大值为 96 Kbps。
    TSAudioProfileMusicHighQuality = 4,
    //5: 指定 48 KHz 采样率，音乐编码，双声道，编码码率最大值为 128 Kbps。
    TSAudioProfileMusicHighQualityStereo = 5,
    //6: 指定 16 KHz 采样率，语音编码，单声道，应用回声消除算法 AES。
    TSAudioProfileIOT = 6,
};

#pragma mark -
typedef NS_ENUM(NSInteger, TSAudioStreamType) {
    //pcm格式的原始音频流
    TSAudioStreamTypePcm,
    //opus编码格式的音频流
    TSAudioStreamTypeOpus
};

#pragma mark -
typedef NS_ENUM(NSInteger, TSAudioPcmFormat) {
    TSAudioPcmUNAVILABLE = -1,//无效
    TSAudioPcmU8 = 0,      //U8格式
    TSAudioPcmS16,         //S16格式
    TSAudioPcmS32,         //S32格式
    TSAudioPcmS64,         //S64格式
    TSAudioPcmFLT,         //float格式
    TSAudioPcmDBL,         //double格式

    TSAudioPcmU8P,         //U8P格式
    TSAudioPcmS16P,        //S16P格式
    TSAudioPcmS32P,        //S32P格式
    TSAudioPcmS64P,        //S64格式
    TSAudioPcmFLTP,        //FLOATP格式
    TSAudioPcmDBLP         //DOUBLEP格式
};


/** The state of the local video stream. */
typedef NS_ENUM(NSInteger, TSLocalVideoStreamState) {
    //本地视频默认初始状态。
    TSLocalVideoStreamStateStopped,
    //本地视频启动成功。
    TSLocalVideoStreamStateCapturing,
    //本地视频启动失败。
    TSLocalVideoStreamStateFailed,
    //正在发送媒体数据
    TSLocalVideoStreamStateSending,
    //不发送媒体数据
    TSLocalVideoStreamStateNoSend
};

typedef NS_ENUM(NSInteger, TSLocalVideoStreamError) {
    //本地视频状态正常。
  TSLocalVideoStreamErrorOK,
    //出错原因不明确。
  TSLocalVideoStreamErrorFailure,
    //没有权限启动本地视频采集设备。
  TSLocalVideoStreamErrorDeviceNoPermission,
    //本地视频采集设备正在使用中。
  TSLocalVideoStreamErrorDeviceBusy,
    //本地视频采集失败，建议检查采集设备是否正常工作。
  TSLocalVideoStreamErrorCaptureFailure,
    //本地编码失败
  TSLocalVideoStreamErrorEncodeFailure
    
    
};

typedef NS_ENUM(NSUInteger, TSLocalAudioState) {
    //本地音频默认初始状态。
    TSLocalAudioStateStopped,
    //本地音频录制设备启动成功。
    TSLocalAudioStateRecording,
    //本地音频启动失败
    TSLocalAudioStateFailed,
    //正在发送媒体数据
    TSLocalAudioStateSending,
    //不发送媒体数据
    TSLocalAudioStateNoSend,
};


typedef NS_ENUM(NSUInteger, TSLocalAudioError) {
    //本地音频状态正常。
    TSLocalAudioErrorOk = 0,
    //本地音频出错原因不明确
    TSLocalAudioErrorFailure = 1,
    //没有权限启动本地音频录制设备
    TSLocalAudioErrorDeviceNoPermission = 2,
    //本地音频录制设备已经在使用中。
    TSLocalAudioErrorDeviceBusy = 3,
    //本地音频录制失败，建议你检查录制设备是否正常工作。
    TSLocalAudioErrorRecordFailure = 4,
    //本地音频编码失败
    TSLocalAudioErrorEncodeFailure = 5,
};

/** Audio output routing. */
typedef NS_ENUM(NSInteger, TSAudioOutputRouting) {
    //使用默认的语音路由
    TSAudioOutputRoutingDefault,
    //使用耳机为语音路由
    TSAudioOutputRoutingHeadset,
    //使用听筒为语音路由
    TSAudioOutputRoutingEarpiece,
    //使用不带麦的耳机为语音路由
    TSAudioOutputRoutingHeadsetNoMic,
    //使用手机的扬声器为语音路由
    TSAudioOutputRoutingSpeakerphone,
    //使用外接的扬声器为语音路由
    TSAudioOutputRoutingLoudspeaker,
    //使用蓝牙耳机为语音路由
    TSAudioOutputRoutingHeadsetBluetooth,
    //使用 USB 外围设备为语音路由（仅适用于 macOS）
    TSAudioOutputRoutingUsb,
    //使用 HDMI 外围设备为语音路由（仅适用于 macOS）
    TSAudioOutputRoutingHdmi,
    //使用 DisplayPort 外围设备为语音路由（仅适用于 macOS）
    TSAudioOutputRoutingDisplayPort,
    //使用 Apple AirPlay 为语音路由（仅适用于 macOS）
    TSAudioOutputRoutingAirPlay
};
/** Media device type. */
typedef NS_ENUM(NSInteger, TSMediaDeviceType) {
    
    //音频播放设备
    TSMediaDeviceTypeAudioPlayout = 0,
    //音频录制设备
    TSMediaDeviceTypeAudioRecording,
    //视频采集设备
    TSMediaDeviceTypeVideoCapture
};


/** Network type. */
typedef NS_ENUM(NSInteger, TSNetworkType) {
    //网络连接类型未知
    TSNetworkTypeUnavailable,
    // wifi
    TSNetworkTypeWIFI,
    // 有线
    TSNetworkTypeWired,
    // 网络类型为 2G 移动网络。
    TSNetworkTypeMobile2G,
    // 网络类型为 3G 移动网络。
    TSNetworkTypeMobile3G,
    // 网络类型为 4G 移动网络。
    TSNetworkTypeMobile4G,
    // 网络类型为 5G 移动网络。
    TSNetworkTypeMobile5G,
};


/** Network quality. */
typedef NS_ENUM(NSUInteger, TSNetworkQuality) {
    //0: 网络质量未知。
    TSNetworkQualityUnknown = 0,
    //1: 网络质量极好。
    TSNetworkQualityExcellent = 1,
    //2: 用户主观感觉和 excellent 差不多，但码率可能略低于 excellent。
    TSNetworkQualityGood = 2,
    //3: 用户主观感受有瑕疵但不影响沟通。
    TSNetworkQualityPoor = 3,
    //4: 勉强能沟通但不顺畅。
    TSNetworkQualityBad = 4,
    //5: 网络质量非常差，基本不能沟通。
    TSNetworkQualityVBad = 5,
    //6: 完全无法沟通。
    TSNetworkQualityDown = 6,
    //7: 暂时无法检测网络质量（未使用）。
    TSNetworkQualityUnsupported = 7,
    //8: 网络质量检测已开始还没完成。
    TSNetworkQualityDetecting = 8,
};

/** Reasons for the connection state change. */
typedef NS_ENUM(NSUInteger, TSConnectionChangedReason) {
    //建立网络连接中。
    TSConnectionChangedConnecting = 0,
    //成功加入频道。
    TSConnectionChangedJoinSuccess = 1,
    //网络连接中断
    TSConnectionChangedInterrupted = 2,
    //网络连接被服务器禁止。
    TSConnectionChangedBannedByServer = 3,
    //加入频道失败。SDK 在尝试加入频道 20 分钟后还是没能加入频道，会返回该状态，并停止尝试重连。
    TSConnectionChangedJoinFailed = 4,
    //离开频道。
    TSConnectionChangedLeaveChannel = 5,
    //不是有效的频道ID。请更换有效的频道ID重新加入频道。
    TSConnectionChangedInvalidChannelId = 6,
    //此用户被服务器禁止，被踢出频道
    TSConnectionChangedRejectedByServer = 7,
    //8: 客户端 IP 地址变更，可能是由于网络类型，或网络运营商的 IP
    TSConnectionChangedClientIpAddressChanged = 8,
    //SDK 和服务器连接保活超时，进入自动重连状态 CONNECTION_STATE_RECONNECTING(4)。
    TSConnectionChangedKeepAliveTimeout = 9,
    //10: 不是有效的 APP ID。请更换有效的 APP ID 重新加入频道。
    TSConnectionInvalidAppId = 10,
};


/** The state of the probe test result. */
typedef NS_ENUM(NSUInteger, TSLastmileProbeResultState) {
  //表示本次 last mile 质量探测的结果是完整的。
  TSLastmileProbeResultComplete,
  //表示本次 last mile 质量探测未进行带宽预测，因此结果不完整。一个可能的原因是测试资源暂时受限。
  TSLastmileProbeResultIncompleteNoBwe,
  //未进行 last mile 质量探测。一个可能的原因是网络连接中断。
  TSLastmileProbeResultUnavailable
};

/** Reason for the user being offline. */
typedef NS_ENUM(NSUInteger, TSUserOfflineReason) {
    //用户主动离开。
    TSUserOfflineReasonQuit,
    //因过长时间收不到对方数据包，超时掉线。注意：由于 SDK 使用的是不可靠通道，也有可能对方主动离开本方没收到对方离开消息而误判为超时掉线。
    TSUserOfflineReasonDropped,
    //用户身份从主播切换为观众时触发。用户身份从主播切换为观众时触发。
    TSUserOfflineReasonBecomeAudience,
};

typedef NS_ENUM(NSUInteger, TSStreamPublishState) {
    //发布失败
    TSStreamPublishError = 0,
    //未发布
    TSStreamNoPublish = 1,
    //发布中
    TSStreamPublished = 2,
};


/** The subscribing state. */
typedef NS_ENUM(NSUInteger, TSStreamSubscribeState) {
    //流订阅失败
    TSStreamSubscribeStateFailed = 0,
    //流下线
    TSStreamSubscribeStateOffline = 1,
    //流上线
    TSStreamSubscribeStateOnline = 2,
    //流上线但没发送
    TSStreamSubscribeStateNoSend = 3,
    //流上线且发送但不接收
    TSStreamSubscribeStateNoRecv = 4,
    //流上线但没发送且不接收
    TSStreamSubscribeStateNoSendNoRecv = 5,
    //正在订阅
    TSStreamSubscribeStateSubscribing = 6,
    //订阅成功
    TSStreamSubscribeStateSubscribed = 7,
    //卡顿中
    TSStreamSubscribeStateFrozen = 8
};

/** 编解码方式 */
typedef NS_ENUM(NSUInteger, TSCodecPriority) {
    //sdk 自动选择编码方式
    TSCodecPriorityAuto,
    //优先使用硬件编解码
    TSCodecPriorityHardware,
    //优先使用软件编解码
    TSCodecPrioritySoftWare
};

/** 地区选择 */
typedef NS_ENUM(NSUInteger, TSRegionCode) {
    TSRegionCodeGlobal, //全球
    TSRegionCodeCN,//中国大陆
    TSRegionCodeNA,//北美
    TSRegionCodeEU,//欧洲
    TSRegionCodeAS//除中国大陆以外的亚洲区域
};


/** 设备连接断开状态 */
typedef NS_ENUM(NSUInteger, TSDeviceChangeType) {
    TSDeviceChangeTypePlugin, //设备连接
    TSDeviceChangeTypeUnplug //设备被拔出
};



/** 设备连接断开状态 */
typedef NS_ENUM(NSUInteger, TSPublishMediaType) {
    TSPublishMediaTypeInit = 0, //不可用
    TSPublishMediaTypeAudio = 1,//将默认音频发布到该频道中
    TSPublishMediaTypeVideo = 2,//将默认视频发布到该频道中
    TSPublishMediaTypeAudioVideo = 3//将默认音视频流都发布到该频道中
};

