//
//  TSMediaIO.h
//  TSPasS_iOS
//
//  Created by yxibng on 2020/11/4.
//

#import <AVFoundation/AVFoundation.h>
#import <Foundation/Foundation.h>
#import "TSEnumerates.h"

NS_ASSUME_NONNULL_BEGIN

#pragma mark -

/**
 TSVideoFrameConsumer 支持接收三种 Buffer 类型的视频帧数据：h264裸数据，未压缩的原始视频数据 以及自定义类型。 自定义视频源时，开发者需要通过 bufferType 来指定一种 Buffer 类型，并在自定义视频源中只使用与其对应的方法来传递视频帧数据。
 */
@protocol TSVideoFrameConsumer <NSObject>

@optional

/// 用户通过下面方法，给sdk传递原始视频数据，TSVideoBufferTypeRawData类型的。
/// @param rawData 原始数据指针
/// @param pixelFormat 像素模式， i420，nv12，rgba 等
/// @param size 视频裸数据的尺寸
/// @param timestamp 传入的视频帧的时间戳，以毫秒为单位。
- (void)consumeRawData:(void *_Nonnull)rawData
           pixelFormat:(TSVideoPixelFormat)pixelFormat
                  size:(CGSize)size
             timestamp:(uint32_t)timestamp;

/// 用户通过下面方法，给sdk传递H264，或者自定义的数据类型。
/// @param packet 视频数据包指针
/// @param length 视频长度
/// @param bufferType 视频数据类型 TSVideoBufferTypeH264 || TSVideoBufferTypeCustom
/// @param isKeyframe 是否是关键帧数据
/// @param timestamp 传入的视频数据的时间戳，以毫秒为单位。
- (void)consumePacket:(void *_Nonnull)packet
               length:(long)length
           bufferType:(TSVideoBufferType)bufferType
           isKeyframe:(BOOL)isKeyframe
            timestamp:(uint32_t)timestamp;


/// 用户通过下面方法，给sdk传递H264，或者自定义的数据类型。
/// @param packet 视频数据包指针
/// @param length 视频长度
/// @param bufferType 视频数据类型 TSVideoBufferTypeH264 || TSVideoBufferTypeCustom
/// @param isKeyframe 是否是关键帧数据
/// @param bitrate  h264 码率
/// @param timestamp 传入的视频数据的时间戳，以毫秒为单位。
- (void)consumePacket:(void *_Nonnull)packet
               length:(long)length
           bufferType:(TSVideoBufferType)bufferType
           isKeyframe:(BOOL)isKeyframe
              bitrate:(uint32_t)bitrate
            timestamp:(uint32_t)timestamp;

///设置h264流元数据, 调用consumePacket前需要先setMetaInfo，告知sdk要推送h264流的元数据
- (void)setMetaInfo:(int)fps
            bitrate:(int)bitrate
              width:(int)width
             height:(int)height
         bufferType:(TSVideoBufferType)bufferType;
@end



#pragma mark -
/**
 TSVideoSourceProtocol 定义了一套协议，开发者通过实现该接口，来创建自定义的视频源，并设置给 TSPaaS 底层的 Media Engine
 实时通讯过程中，TSPaaS SDK 通常会启动默认的视频输入设备，即内置的摄像头，进行视频推流。 使用 TSVideoSourceProtocol 接口可以自定义视频源。通过调用 设置视频源 setVideoSource 接口，可以改变并控制默认的视频输入设备，再将自定义的视频源发送给 TSPaaS Media Engine，让 Media Engine 进行其它视频处理
 */
@protocol TSVideoSourceProtocol <NSObject>
@required
/** 当用户调用 setVideoSource，向传入自定义视频源的时候，sdk 会给 consumer 赋值。
 用户通过consumer 可以向 sdk 传入原始数据。sdk 会对传入的数据编码，推流到服务器。
 */
@property (strong) id<TSVideoFrameConsumer> _Nullable consumer;

/**
 *当用户调用 setVideoSource，向传入自定义视频源的时候，sdk 会给 lowStreamConsumer 赋值。
 *当 bufferType 为 TSVideoBufferTypeH264 时，sdk 会给 lowStreamConsumer 赋值，用户通过 lowStreamConsumer 可以向 sdk 传入 h264 数据， 结合 consumer 来实现给 sdk 推送 h264 数据时的大小流功能。
 */
@property (strong) id<TSVideoFrameConsumer> _Nullable lowStreamConsumer;


/**
 初始化视频源
 
 Media Engine 在初始化视频源的时候会回调此方法。开发者可以在这个方法中做一些准备工作，例如打开 Camera，或者初始化视频源，并通过返回值告诉 Media Engine，自定义的视频源是否已经准备好。
 
 Note: 初始化视频源过程中，开发者需要在 bufferType 中指定一种 Buffer 类型，并在自定义视频源中只使用与其对应的方法来传递视频帧数据。
 
 在初始化视频源过程中，Media Engine 会传递给开发者的一个 TSVideoFrameConsumer 对象。开发者需要保存该对象，并在视频源启动后，通过这个对象把视频帧输入给 Media Engine。
 
 开发者需要手动输入 YES 或 NO，以告诉 Media Engine 自定义视频源是否已准备好。
 
 @return YES: 自定义的视频源已经完成了初始化工作. NO: 自定义的视频源设备没准备好或者初始化失败，Media Engine 会停下来并上报错误
 */
- (BOOL)shouldInitialize;
/**
 Media Engine 在启动视频源时会回调这个方法。开发者可以在该方法中启动视频帧捕捉。开发者需要通过返回值告诉告知 Media Engine 自定义的视频源是否开启成功。
 
 开发者需要手动输入 YES 或 NO，以告诉 Media Engine 自定义视频源是否开启：
 
 @return YES:自定义的视频源已成功开启，接下来会打开 TSVideoFrameConsumer 的开关，接收开发者传输的视频帧
 NO：自定义的视频源设备启动失败，Media Engine 会停下来并上报错误
 
 */
- (int)shouldStart;
/**
 Media Engine 在停止视频源的时候会回调这个方法。开发者可以在这个方法中停止视频的采集。Media Engine 通过这个回调通知开发者，TSVideoFrameConsumer 的帧输入开关即将关闭，之后输入的视频帧都会被丢弃。
 */
- (void)shouldStop;
/**
 Media Engine 通知开发者视频源即将失效，开发者可以在这个方法中关闭视频源设备。引擎会销毁 TSVideoFrameConsumer 对象，开发者需要确保在此回调之后不再使用它。
 */
- (void)shouldDispose;
/**
 Media Engine 在初始化的时候，会调用这个方法来查询该视频源所使用的 Buffer 类型。开发者必须指定且只能指定一种 Buffer 类型并通过返回值告诉 Media Engine
 @return bufferType 自定义视频源的数据类型，详见`TSVideoBufferType`
 */
- (TSVideoBufferType)bufferType;

@optional
/**
 是否支持主动请求关键帧。
 当bufferType 为H264时，编码器由用户控制。
 SDK通过此接口获取编码器是否有强制输出关键帧的能力。
 */
- (BOOL)isKeyFrameRequestSupported;
/**
 请求编码器将下一帧强制输出为关键帧。
 当bufferType不为H264 或者 isKeyFrameRequestSupported 为NO时，该方法被忽略。
 */
- (BOOL)requestKeyFrame;


@end

#pragma mark -

/**
 TSVideoSinkProtocol 定义了一套协议，开发者通过实现该接口，来创建自定义的视频渲染器，并设置给 TSPaaS 底层的 Media Engine。
 实时通讯过程中，TSPaaS SDK 通常会启动默认的视频渲染器进行视频渲染。 TSVideoSinkProtocol 可以自定义视频渲染器，再通过调用 设置本地视频渲染器 setLocalVideoRenderer 和 设置远端视频渲染器 setRemoteVideoRenderer 接口，改变并控制默认的视频渲染器。
 */
@protocol TSVideoSinkProtocol <NSObject>
@required
/**
 初始化渲染器
 Media Engine 初始化渲染器的时候调用这个方法。开发者可以在这个方法中做渲染器的初始化工作。如果是耗时操作，也可以提前初始化好，然后在这个方法中通过返回值告知 Media Engine 自定义渲染器已初始化好。 该方法需要开发者手动输入 YES 或 NO，告知 Media Engine 自定义渲染器的状态。
 @return YES: Media Engine 会认为自定义的渲染器已经初始化好
 NO: Media Engine 会认为自定义的渲染器初始化失败，不继续往下运行
 */
- (BOOL)shouldInitialize;
/**
 启动渲染器
 Media Engine 在开启渲染功能的时候会回调这个方法。开发者可以在这个方法中启动渲染器。 该方法需要开发者手动输入 YES 或 NO，Media Engine 会根据输入值做对应的动作：
 @return YES: Media Engine 继续进行渲染
 NO：Media Engine 认为出错而停止渲染器的功能
 */
- (BOOL)shouldStart;
/**
 停止渲染器
 Media Engine 在停止渲染功能的时候会回调这个方法。开发者可以在这个方法中停止渲染。
 */
- (void)shouldStop;
/**
 释放渲染器
 Media Engine 通知开发者渲染器即将被废弃。在 shouldDispose 返回之后，开发者就可以释放掉资源了。
 */
- (void)shouldDispose;
/**
 获取 Buffer 类型
 用于在自定义渲染器的时候，需要指定 Buffer 类型，通过返回值告知引擎。Media Engine 会调用这个方法并检查返回值类型。
 
 @return 自定义渲染，需要的buffer类型， 详见`TSVideoBufferType`
 */
- (TSVideoBufferType)bufferType;
/**
 获取像素格式
 用于自定义渲染器的时候，还需要指定视频数据的像素格式。
 
 @return 自定义渲染，需要的像素格式， 详见`TSVideoPixelFormat`
 */
- (TSVideoPixelFormat)pixelFormat;
@optional
/**
 视频数据的位深，默认8位。
 @return 自定义渲染，期望视频数据位深。
 */
- (int)bitDepth; //默认8位

@optional

/**
 如果用户设置的数据类型是 TSVideoBufferTypeRawData， sdk 会通过此接口将内部的原始视频数据传递给用户。
 用户可以根据sdk传出的数据。做自定义的绘制。
 @param rawData 原始视频数据指针。
 @param size 原始视频帧的分辨率。
 @param pixelFormat 原始视频数据的像素格式
 @param timestamp 视频帧时间戳，毫秒。
 */
- (void)renderRawData:(void *_Nonnull)rawData
                 size:(CGSize)size
          pixelFormat:(TSVideoPixelFormat)pixelFormat
            timestamp:(long)timestamp;

/**
 如果用户设置的数据类型是 TSVideoBufferTypeH264 || TSVideoBufferTypeCustom， sdk 会通过此接口将内部解码后的H264或自定义数据传递给用户。
 @param packet h264 或者自定义视频数据的指针
 @param length 视频数据长度
 @param bufferType 视频数据类型 TSVideoBufferTypeH264 || TSVideoBufferTypeCustom
 @param keyframe 是否是关键帧
 @param timestamp 视频帧时间戳，毫秒。
 */
- (void)renderPacket:(void *_Nonnull)packet
              length:(long)length
          bufferType:(TSVideoBufferType)bufferType
            keyframe:(BOOL)keyframe
           timestamp:(long)timestamp;
@end



@protocol TSAudioFrameConsumer <NSObject>

/// 用户通过下面方法，给sdk传递音频数据
/// @param pcmDataArray 原始数据指针数组
/// @param lineSizeArray 音频数据长度数组
/// @param frameType 音频PCM格式
/// @param channel  声道数
/// @param sampleRate 采样率
/// @param timestamp 传入的音频帧的时间戳，以毫秒为单位。
- (void)consumeAudioPcmData:(void *_Nonnull *_Nonnull)pcmDataArray
                   lineSize:(int *)lineSizeArray
                  frameType:(TSAudioPcmFormat)frameType
                    channel:(int)channel
                 sampleRate:(int)sampleRate
                  timestamp:(long)timestamp;


@end


@protocol TSAudioSourceProtocol <NSObject>
@required
/**
 当用户调用 setAudioSource，向传入自定义音频源的时候，sdk 会给 consumer 赋值。
 用户通过consumer 可以向 sdk 传入原始数据。sdk 会对传入的数据编码，推流到服务器。
 */
@property (strong) id<TSAudioFrameConsumer> _Nullable consumer;


/**
 初始化音频源
 
 Media Engine 在初始化音频源的时候会回调此方法。开发者可以在这个方法中做一些准备工作，例如打开 麦克风，或者初始化音频源，并通过返回值告诉 Media Engine，自定义的音频源是否已经准备好。
 
 在初始化音频源过程中，Media Engine 会传递给开发者的一个 TSAudioFrameConsumer 对象。开发者需要保存该对象，并在音频源启动后，通过这个对象把音频帧输入给 Media Engine。
 
 开发者需要手动输入 YES 或 NO，以告诉 Media Engine 自定义音频源是否已准备好。
 
 @return YES: 自定义的音频源已经完成了初始化工作. NO: 自定义的音频源设备没准备好或者初始化失败，Media Engine 会停下来并上报错误
 */
- (BOOL)shouldInitialize;


/**
 Media Engine 通知开发者音频源即将失效，开发者可以在这个方法中关闭音频源设备。引擎会销毁 TSAudioFrameConsumer 对象，开发者需要确保在此回调之后不再使用它。
 */
- (void)shouldDispose;

/**
 启动渲染器
 Media Engine 在开启渲染功能的时候会回调这个方法。开发者可以在这个方法中启动渲染器。 该方法需要开发者手动输入 YES 或 NO，Media Engine 会根据输入值做对应的动作：
 @return YES: Media Engine 继续进行渲染
 NO：Media Engine 认为出错而停止渲染器的功能
 */
- (BOOL)shouldStart;
/**
 停止渲染器
 Media Engine 在停止渲染功能的时候会回调这个方法。开发者可以在这个方法中停止渲染。
 */
- (void)shouldStop;

@optional
/**
 是否开启回音消除，默认开启
 @return YES: 开启回音消除功能 NO: 关闭回音消除
 */
- (BOOL)enableAec;

@end



NS_ASSUME_NONNULL_END
