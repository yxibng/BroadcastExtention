//
//  ViewController.swift
//  RTCScreenBroadcast
//
//  Created by xiaobing yao on 2022/12/2.
//

import UIKit
import ReplayKit

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        let task = URLSession.shared.dataTask(with: URLRequest.init(url: .init(string: "https://www.baidu.com")!)) { _, _, _ in
            
        }
        task.resume()
        // Do any additional setup after loading the view.
    }

    @IBOutlet weak var systemBroadcastPicker: RPSystemBroadcastPickerView! {
        didSet {
            if let url = Bundle.main.url(
                forResource: "BroadcastExtention", withExtension: "appex", subdirectory: "PlugIns")
            {
                if let bundle = Bundle(url: url) {
                    systemBroadcastPicker.preferredExtension = bundle.bundleIdentifier
                    systemBroadcastPicker.showsMicrophoneButton = true
                }
            }
        }
    }
    
}

