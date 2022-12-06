//
//  TSEngineManager.swift
//  ToseeBroadcastExtention
//
//  Created by xiaobing yao on 2022/11/17.
//

import Foundation
import TSRtc_iOS
import ReplayKit


protocol TSLoginParams {
    var appId: String { get set }
    var uid: String { get set }
    var channelId: String { get set }
    var streamName: String { get set }
}


struct RtcLoginParams : TSLoginParams{
    var appId = "Drv4Q0KB14Y85qma"
    var uid = "screenshare"
    var channelId = "555"
    var streamName = "first"
}

struct BroadcastLoginParams : TSLoginParams {
    var appId: String
    var uid: String
    var channelId: String
    var streamName: String
}


extension CGSize {
    static func encodeSize(width: Int, height: Int, vertical: Bool) -> CGSize {
        if vertical {
            return CGSize.init(width: width, height: height)
        } else {
            return CGSize.init(width: height, height: width)
        }
    }
}


class TSVideoSource: NSObject, TSVideoSourceProtocol, TSHardwareEncoderDelegate {
    func encoder(_ encoder: TSHardwareEncoder, gotEncoderData data: UnsafeMutablePointer<UInt8>, length: Int32, iskey: Bool, timestamp: Int64) {
        if let consumer = consumer {
            consumer.consumePacket?(data, length: Int(length), bufferType: .H264, isKeyframe: iskey, timestamp: UInt32(timestamp))
        }
    }
    
    var lowStreamConsumer: TSVideoFrameConsumer?
    var consumer: TSVideoFrameConsumer?
    let encoder = TSHardwareEncoder.init()
    
    var lastTimestamp: Double = 0
    
    func shouldInitialize() -> Bool {
        encoder.delegate = self
        return true
    }
    
    func shouldStart() -> Int32 {
        return 1
    }
    
    func shouldStop() {
        
    }
    
    func shouldDispose() {
        
    }
    
    func bufferType() -> TSVideoBufferType {
        return .H264
    }
    
    func handleSampleBuffer(sampleBuffer: CMSampleBuffer, rotation: Int32) {
        
        guard let pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer) else {
            return
        }
        
        let timestamp = CMSampleBufferGetPresentationTimeStamp(sampleBuffer).seconds * 1000
        
#if true
        //TODO: 此处需要处理丢帧
        let gap = timestamp - lastTimestamp
        if gap < 1000/15.0 && gap != 0 {
            return
        }
        
        lastTimestamp = timestamp
#endif
        if rotation == 90 || rotation == 270 {
            var newPixelBuffer: CVPixelBuffer?
            let error = CVPixelBufferCreate(kCFAllocatorDefault,
                                            CVPixelBufferGetHeight(pixelBuffer),
                                            CVPixelBufferGetWidth(pixelBuffer),
                                            CVPixelBufferGetPixelFormatType(pixelBuffer),
                                            nil,
                                            &newPixelBuffer)
            guard error == kCVReturnSuccess else {
                return
            }
            
            
            let ciImage = CIImage(cvPixelBuffer: pixelBuffer).oriented(rotation == 270 ? .left : .right)
            let context = CIContext(options: nil)
            context.render(ciImage, to: newPixelBuffer!)
            
            guard let newPixelBuffer = newPixelBuffer else {
                return
            }
            self.encoder.encode(newPixelBuffer, timestamp: Int64(timestamp))
            return;
        }
        self.encoder.encode(pixelBuffer, timestamp: Int64(timestamp))
    }
}

class TSAudioSource: NSObject, TSAudioSourceProtocol {
    
    private let audioHandler: TSAudioHandler = TSAudioHandler.init()
    
    var consumer: TSAudioFrameConsumer?
    
    func shouldInitialize() -> Bool {
        return true
    }
    
    func shouldDispose() {
        
    }
    
    func shouldStart() -> Bool {
        audioHandler.consumer = consumer
        return true
    }
    
    func shouldStop() {
        audioHandler.consumer = nil
    }
    
    func onReceiveAppAudio(sampleBuffer: CMSampleBuffer) {
        audioHandler.receiveAudioSampleBuffer(sampleBuffer, audioType: .app)
    }
    
    func onReceiveMicAudio(sampleBuffer: CMSampleBuffer) {
        audioHandler.receiveAudioSampleBuffer(sampleBuffer, audioType: .mic)
    }
}


@objc protocol TSRtcManagerDelegate {
    @objc optional func rtcFailed(code: Int, message: String)
    @objc optional func rtcStartSuccess()
}



class TSRtcManager: NSObject {
    static let sharedInstance = TSRtcManager()
    weak var delegate: TSRtcManagerDelegate? = nil
    
    private var engine: TSRtcEngineKit? = nil
    private var channel: TSRtcChannel? = nil
    private var rotation = 0
    
    private let audioSource: TSAudioSource = TSAudioSource.init()
    private let videoSource: TSVideoSource = TSVideoSource.init()
    private var videoStream: TSRtcVideoStream? = nil
    private var joinSuccess = false
    
    //是否发送音频
    private let sendAduio = true
    
    func onBroadcastStarted(sampleHandler: SampleHandler) {
        print("🥹🥹 rtc appId = \(loginParams.appId) \n uid = \(loginParams.uid) \n channelId = \(loginParams.channelId) \n streamName = \(loginParams.streamName) ")
        
        joinSuccess = false
        let config = TSRtcEngineConfig.init()
        config.appId = loginParams.appId
        engine = TSRtcEngineKit.sharedEngine(with: config, delegate: self)
        engine?.enableLocalAudio(false)
        if sendAduio {
            engine?.setLocalAudioSource(self.audioSource)
            engine?.enableLocalAudio(true)
        }
        engine?.enableLocalVideo(true)
        
        channel = engine?.createRtcChannel(loginParams.channelId, profile: .liveBroadcasting, delegate: self)
        channel?.setDefaultMuteAllRemoteAudioStreams(true)
        channel?.setDefaultMuteAllRemoteVideoStreams(true)
        
        
        let encodeSize = UIScreen.main.currentMode?.size ?? CGSize.init(width: 720, height: 1280)
        let encodeConfig = TSVideoEncoderConfiguration.init(size: encodeSize, frameRate: .fps15, bitrate: 0, orientationMode: .adaptative, mirrorMode: .auto)
        videoStream = channel?.setupCustomVideoSource(self.videoSource, streamName: loginParams.streamName, bufferType: .H264, delegate: self, encodeConfig: encodeConfig)
        videoStream?.enableDualSteamMode(false)
        if sendAduio {
            channel?.publish(.audio)
        }
        channel?.join(byUid: loginParams.uid)
        
    }
    
    func onBroadcastFinished() {
        videoStream?.unPublish()
        channel?.unpublish()
        channel?.leave()
    }
    
    func onReceiveVideo(sampleBuffer: CMSampleBuffer) {
        var rotation : Int = 0
        if let orientationAttachment = CMGetAttachment(sampleBuffer, key: RPVideoSampleOrientationKey as CFString, attachmentModeOut: nil) as? NSNumber {
            if let orientation = CGImagePropertyOrientation(rawValue: orientationAttachment.uint32Value) {
                switch orientation {
                case .up,    .upMirrored:    rotation = 0
                case .down,  .downMirrored:  rotation = 180
                case .left,  .leftMirrored:  rotation = 90
                case .right, .rightMirrored: rotation = 270
                default:   break
                }
            }
        }
        self.videoSource.handleSampleBuffer(sampleBuffer: sampleBuffer, rotation: Int32(rotation))
    }
    
    func onReceiveAppAudio(sampleBuffer: CMSampleBuffer) {
        audioSource.onReceiveAppAudio(sampleBuffer: sampleBuffer)
    }
    
    func onReceiveMicAudio(sampleBuffer: CMSampleBuffer) {
        audioSource.onReceiveMicAudio(sampleBuffer: sampleBuffer)
    }
    
}

extension TSRtcManager: TSRtcEngineDelegate {
    
}

extension TSRtcManager: TSRtcChannelDelegate {
    
    func rtcChannel(_ rtcChannel: TSRtcChannel, didOccurError errorCode: Int32, message: String) {
        if errorCode >= TSErrorCode.lookupScheduleServerFailed.rawValue &&
            errorCode <= TSErrorCode.noAvailableServerResources.rawValue ||
            errorCode == TSErrorCode.invalidAppId.rawValue ||
            errorCode == TSErrorCode.lookupScheduleServerTimeout.rawValue {
            handleJoinChannelFailed(code: errorCode, message: message)
        }
    }
    
    func rtcChannel(_ rtcChannel: TSRtcChannel, didJoinChannelWithUid uid: String, elapsed: Int) {
        //进入频道成功
        videoStream?.publish()
        joinSuccess = true
        self.delegate?.rtcStartSuccess?()
    }
    
    func rtcChannel(_ rtcChannel: TSRtcChannel, didLeaveChannelWith stats: TSChannelStats) {
        joinSuccess = false
    }
}


extension TSRtcManager: TSRtcVideoStreamDelegate {
    func videoStream(_ videoStream: TSRtcVideoStream, onVideoPublishStateChangedTo state: TSStreamPublishState, elapsed: Int) {}
    func videoStream(_ videoStream: TSRtcVideoStream, onLocalVideoStateChangedTo state: TSLocalVideoStreamState, elapsed: Int) {}
}

extension TSRtcManager {
    var loginParams: TSLoginParams {
        return RtcLoginParams.init()
    }
}

extension TSRtcManager {
    func handleJoinChannelFailed(code: Int32, message: String) {
        self.delegate?.rtcFailed?(code: Int(code), message: message)
    }
}


