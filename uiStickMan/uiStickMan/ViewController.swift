//
//  ViewController.swift
//  uiStickMan
//
//  Created by Yotsatornbenz on 23/11/2561 BE.
//  Copyright © 2561 BB. All rights reserved.
//

import UIKit
import Firebase
import AVFoundation

class ViewController: UIViewController {

    var listKey: [String] = ["key0","key3","key6","key9","key12","key15"]
    var player: AVAudioPlayer?

 
    @IBOutlet weak var key0: UILabel!
    @IBOutlet weak var key3: UILabel!
    @IBOutlet weak var key6: UILabel!
    @IBOutlet weak var key9: UILabel!
    @IBOutlet weak var key12: UILabel!
    @IBOutlet weak var key15: UILabel!
    
    @IBOutlet weak var distanceR: UILabel!
    @IBOutlet weak var distanceL: UILabel!
    
    var ref: DatabaseReference!
    let post: Dictionary = ["data": "",
                            "times": 0] as [String : Any]
    
    override func viewDidLoad() {
        super.viewDidLoad()
        ref = Database.database().reference()
        observeData()
    }
    
    func makeData(key: String){
//        for i in listKey {
//            if(key == i){
//                ref.child("key/\(i)/times").setValue(post)
//            }
//        }
    }
    
    func playSound(name: String) {
        let path = Bundle.main.path(forResource: "\(name)", ofType:"wav")!
        
        let url = URL(fileURLWithPath: path)
        
        do {
            player = try AVAudioPlayer(contentsOf: url)
            player?.play()
        } catch {
            // couldn't load file :(
            print("couldn't load file")
        }
    }
    func observeData() {
        ref.child("distance").observe(DataEventType.childChanged) { (snapshot) in
            let key = snapshot.key.last
            if let valueD = snapshot.value as? Int {
                if(key == "L"){
                    self.distanceL.text = String(valueD)
                } else {
                    self.distanceR.text = String(valueD)
                }
            }
            
        }
        ref.child("key").observe(DataEventType.childChanged) { (snapshot) in
            let postDict = snapshot.value as? [String : AnyObject] ?? [:]
            let key = (snapshot.key)
            let indexafterKey = key.index(key.startIndex, offsetBy: 3)
            let code = key[indexafterKey...]
            print("times: \(String(describing: postDict["times"])) , lastkey: \(code)")
            if(postDict["times"]?.int32Value == 1){
                self.playSound(name: "translate_tts (\(code))")
            } else {
                print("not qualified")
            }
            
        }
    }
}
        
    




