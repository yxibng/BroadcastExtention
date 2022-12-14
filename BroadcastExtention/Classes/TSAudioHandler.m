//
//  TSAudioHandler.m
//  ToseeBroadcastExtention
//
//  Created by yxibng on 2021/8/30.
//

#import "TSAudioHandler.h"
#import "TSSpeexConverter.h"

const int bufferSamples = 48000;
size_t dataPointerSize = bufferSamples;
int16_t dataPointer[bufferSamples];

int16_t appAudioData[bufferSamples];
int16_t micAudioData[bufferSamples];

uint64_t micAudioSize = 0;

const int bufferCapacityInByte = bufferSamples * sizeof(int16_t);



static void writePCM(uint8_t * pcm, int length) {
    static FILE* m_pOutFile = NULL;
    if (!m_pOutFile) {
        NSString *path = [[[NSFileManager defaultManager] containerURLForSecurityApplicationGroupIdentifier:@"group.tech.tosee.mobile"].path stringByAppendingPathComponent:@"xx.pcm"];
        [[NSFileManager defaultManager] removeItemAtPath:path error:nil];
        m_pOutFile = fopen([path cStringUsingEncoding:NSUTF8StringEncoding], "a+");
        NSLog(@"path = %@", path);
    }
    fwrite(pcm, 1, length, m_pOutFile);
}



@interface TSAudioMixer : NSObject
{
    AudioStreamBasicDescription _dstDescription;
}

@property (nonatomic, strong) TSSpeexConverter *resamplerApp;
@property (nonatomic, strong) TSSpeexConverter *resamplerMic;


@property (nonatomic, weak) TSAudioHandler *audioHandler;

@end


@implementation TSAudioMixer


- (instancetype)init
{
    self = [super init];
    if (self) {
        _dstDescription = *([[AVAudioFormat alloc] initWithCommonFormat:AVAudioPCMFormatInt16 sampleRate:16000 channels:1 interleaved:NO].streamDescription);
    }
    return self;
}


- (void)receiveAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer audioType:(AudioType)audioType {
    
    OSStatus err = noErr;
    
    CMBlockBufferRef audioBuffer = CMSampleBufferGetDataBuffer(sampleBuffer);
    if (!audioBuffer) {
        return;
    }
    
    size_t totalBytes;
    char *samples;
    err = CMBlockBufferGetDataPointer(audioBuffer, 0, NULL, &totalBytes, &samples);
    if (!totalBytes || err != noErr) {
        return;
    }
    
    CMAudioFormatDescriptionRef format = CMSampleBufferGetFormatDescription(sampleBuffer);
    const AudioStreamBasicDescription *description = CMAudioFormatDescriptionGetStreamBasicDescription(format);
    
    //??????????????????????????????????????????????????????????????????????????????
    size_t totalFrames = totalBytes / description->mBytesPerFrame;
    //??????????????????????????? = totalFrames *2???
    size_t totalSamples = totalBytes / (description->mBitsPerChannel / 8);
    UInt32 channels = description->mChannelsPerFrame;
    
    memset(dataPointer, 0, bufferCapacityInByte);
    err = CMBlockBufferCopyDataBytes(audioBuffer,
                                     0,
                                     totalBytes,
                                     dataPointer);
    
    CMTime pts = CMSampleBufferGetPresentationTimeStamp(sampleBuffer);
    float timestamp = CMTimeGetSeconds(pts) * 1000;
    
    BOOL isFloat = description->mFormatFlags & kAudioFormatFlagIsFloat;
    BOOL isBigEndian = description->mFormatFlags & kAudioFormatFlagIsBigEndian;
    BOOL isInterleaved = !(description->mFormatFlags & kAudioFormatFlagIsNonInterleaved);
    
    // big endian to little endian
    size_t bytesPerSample = description->mBitsPerChannel / 8;
    if (isBigEndian) {
        for (int i = 0; i < totalSamples; i++) {
            uint8_t* p = (uint8_t*)dataPointer + i * bytesPerSample;
            for (int j = 0; j < bytesPerSample / 2; j++) {
                uint8_t tmp;
                tmp = p[j];
                p[j] = p[bytesPerSample - j -1];
                p[bytesPerSample -j -1] = tmp;
            }
        }
    }
    
    // float to int
    if (isFloat) {
        float* floatData = (float*)dataPointer;
        int16_t* intData = (int16_t*)dataPointer;
        for (int i = 0; i < totalSamples; i++) {
            float tmp = floatData[i] * 32767;
            intData[i] = (tmp >= 32767) ?  32767 : tmp;
            intData[i] = (tmp < -32767) ? -32767 : tmp;
        }
        totalBytes = totalSamples * sizeof(int16_t);
    }
    
    //??????????????????
    if (channels > 1) {
        if (isInterleaved) {
            int bytesPerFrame = (*description).mBytesPerFrame;
            for (int i = 0; i < totalFrames; i++) {
                memmove(dataPointer + i, (uint8_t *)dataPointer + i * bytesPerFrame, sizeof(int16_t));
            }
        }
    }
    
    //???????????????????????????????????????
    size_t srcLength = totalBytes / channels;
    uint8_t *srcData = (uint8_t *)dataPointer;
    
    if (audioType == AudioTypeApp) {
        /*
         ??????app???????????????
         1. ??????float->int16, bigEndian->littleEndian, ????????????????????????
         2. ????????????????????????????????????appAudioData??? ???????????????????????????outputLength
         3. ???????????????????????????????????????
            1????????? outputLength > micAudioSize, ?????????????????????????????? micAudioSize
            2????????? outputLength <= micAudioSize, ?????????????????????????????? outputLength
            3????????????????????????micAudioSize??? ?????? micAudioData ??????
            4??????appAudioData ?????????????????????
         */

        
        if (!_resamplerApp) {
            AudioStreamBasicDescription srcFormat = *([[AVAudioFormat alloc] initWithCommonFormat:AVAudioPCMFormatInt16 sampleRate:description->mSampleRate channels:1 interleaved:NO].streamDescription);
            _resamplerApp = [[TSSpeexConverter alloc] initWithSrcFormat: srcFormat dstFormat:_dstDescription];
        }
        
        uint32_t capacity = bufferCapacityInByte;
        uint8_t *dstData = (uint8_t *)appAudioData;
        uint32_t outputLength = 0;
        
        BOOL ret = [self.resamplerApp convertMonoPCMWithSrc:srcData
                                                    srcLength:(uint32_t)srcLength
                                             outputBufferSize:capacity
                                                 outputBuffer:dstData
                                                 outputLength:&outputLength];
        
        assert(ret);
        if (!ret) {
            return;
        }
        
        int32_t mixSize = outputLength > micAudioSize ? (int32_t)micAudioSize : outputLength;
        int32_t mixCount = mixSize / sizeof(int16_t);
        
        for (int i = 0; i< mixCount; i++) {
            appAudioData[i]  = (appAudioData[i] + micAudioData[i]) / 2;
        }
        
        if (mixCount > 0) {
            memmove(micAudioData, (uint8_t *)micAudioData + mixSize, micAudioSize - mixSize);
            micAudioSize -= mixSize;
        }
        
        void *pcm[2];
        pcm[0] = appAudioData;
        int lineSize[1] = {outputLength};
        [self.audioHandler.consumer consumeAudioPcmData:pcm lineSize:lineSize frameType:TSAudioPcmS16 channel:1 sampleRate:_dstDescription.mSampleRate timestamp:timestamp];
    } else {
        /*
         ?????????????????????
         1. ??????float->int16, bigEndian->littleEndian, ????????????????????????
         2. ????????????????????????????????????micAudioBuffer??? ?????? micAudioSize
         */
        if (!_resamplerMic) {
            AudioStreamBasicDescription srcFormat = *([[AVAudioFormat alloc] initWithCommonFormat:AVAudioPCMFormatInt16 sampleRate:description->mSampleRate channels:1 interleaved:NO].streamDescription);
            _resamplerMic = [[TSSpeexConverter alloc] initWithSrcFormat: srcFormat dstFormat:_dstDescription];
        }

        uint8_t tempBuffer[bufferCapacityInByte] = {0};
        uint32_t capacity = bufferCapacityInByte;
        uint8_t *dstData = tempBuffer;
        uint32_t outputLength = 0;
        BOOL ret = [self.resamplerMic convertMonoPCMWithSrc:srcData
                                                  srcLength:(uint32_t)srcLength
                                           outputBufferSize:capacity
                                               outputBuffer:dstData
                                               outputLength:&outputLength];
        assert(ret);
        if (!ret) {
            return;
        }
        memcpy((uint8_t *)micAudioData + micAudioSize, dstData, outputLength);
        micAudioSize += outputLength;
    }
}

@end


@interface TSAudioHandler()
@property (nonatomic, strong) NSLock *lock;
@property (nonatomic, strong) TSAudioMixer *audioMixer;
@end



@implementation TSAudioHandler

- (instancetype)init
{
    self = [super init];
    if (self) {
        _lock = [[NSLock alloc] init];
        _audioMixer = [[TSAudioMixer alloc] init];
        _audioMixer.audioHandler = self;
    }
    return self;
}

- (void)receiveAudioSampleBuffer:(CMSampleBufferRef)sampleBuffer audioType:(AudioType)audioType {
    [self.lock lock];
    [self.audioMixer receiveAudioSampleBuffer:sampleBuffer audioType:audioType];
    [self.lock unlock];
}

@end

