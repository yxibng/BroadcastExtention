//
//  AudioHandler.h
//  ToseeBroadcastExtention
//
//  Created by yxibng on 2021/8/30.
//

#import <Foundation/Foundation.h>
#import <TSRtc_iOS/TSMediaIO.h>
NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, AudioType) {
    AudioTypeApp,
    AudioTypeMic
};


@interface TSAudioHandler: NSObject


@property (nonatomic, weak) id<TSAudioFrameConsumer> consumer;

- (void)receiveAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer audioType:(AudioType)audioType;

@end


NS_ASSUME_NONNULL_END
