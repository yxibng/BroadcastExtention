//
//  AppDelegate.swift
//  RTCScreenBroadcast
//
//  Created by xiaobing yao on 2022/12/2.
//

import UIKit

@main
class AppDelegate: UIResponder, UIApplicationDelegate {
    
    var window: UIWindow?
    
    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        // Override point for customization after application launch.
        return true
    }

    func applicationDidBecomeActive(_ application: UIApplication) {
//        Log.copyToMain()
    }

}


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
            try FileManager.default.copyItem(atPath: path, toPath: mainPath)
            print("move success")
        } catch {
            print("move failed, \(error)")
        }
        
        
    }
    
    
}
