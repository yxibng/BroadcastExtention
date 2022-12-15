//
//  TSManager.swift
//  ToseeBroadcastExtention
//
//  Created by xiaobing yao on 2022/11/21.
//

import Foundation
import CoreMedia

class TSManager {
    static let sharedInstance = TSManager()
    private let rtcManager = TSRtcManager()
    
    private weak var sampleHandler: SampleHandler? = nil
    func onBroadcastStarted(sampleHandler: SampleHandler) {
        self.sampleHandler = sampleHandler
        self.rtcManager.delegate = self
        self.rtcManager.onBroadcastStarted(sampleHandler: sampleHandler)
    }
    
    func onBroadcastFinished() {
        print("🥹🥹 \(#function)")
        self.rtcManager.onBroadcastFinished()
    }
    
    func onReceiveVideo(sampleBuffer: CMSampleBuffer) {
        self.rtcManager.onReceiveVideo(sampleBuffer: sampleBuffer)
    }
    
    func onReceiveAppAudio(sampleBuffer: CMSampleBuffer) {
        self.rtcManager.onReceiveAppAudio(sampleBuffer: sampleBuffer)
    }
    
    func onReceiveMicAudio(sampleBuffer: CMSampleBuffer) {
        self.rtcManager.onReceiveMicAudio(sampleBuffer: sampleBuffer)
    }
}



extension TSManager : TSRtcManagerDelegate {
    func rtcStartSuccess() {
        print("🥹🥹 \(#function)")
    }
    func rtcFailed(code: Int, message: String) {
        print("rtcFailed \(code), message \(message)")
        self.stopWithError(message: "RTC JoinChannelFailed")
    }
}



extension TSManager {
    func stopWithError(message: String) {
        let userInfo = [NSLocalizedFailureReasonErrorKey : "屏幕共享结束,\(message)"]
        let err = NSError.init(domain: "RPBroadcastErrorDomain", code: 401, userInfo: userInfo)
        self.sampleHandler?.finishBroadcastWithError(err)
        print("🥹🥹 \(#function)")
    }
}


extension TSManager {
    
    struct Log {
        static var logFilePathInContainer: String? {
            guard let group = FileManager.default.containerURL(forSecurityApplicationGroupIdentifier: "group.tech.tosee.mobile") else {
                return nil
            }
            guard var path = (group as NSURL).path else {
                return nil
            }
            path = (path as NSString).appendingPathComponent("xx.pcm")
            return path
        }
        
        static func copyToMain() {
            
            guard let path = logFilePathInContainer else {
                return
            }
            
            do {
                let mainPath = (NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true).first!  as NSString).appendingPathComponent("xx.pcm")
                try? FileManager.default.removeItem(atPath: mainPath)
                if #available(iOSApplicationExtension 16.0, *) {
                    try FileManager.default.moveItem(at: URL.init(filePath: path), to: URL.init(filePath: mainPath))
                } else {
                    // Fallback on earlier versions
                }
            } catch {
                print("move failed")
            }
        }
    }
    
    
    
}
