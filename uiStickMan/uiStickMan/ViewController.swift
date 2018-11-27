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
import Social
import MessageUI


class ViewController: UIViewController, MFMessageComposeViewControllerDelegate {
    func messageComposeViewController(_ controller: MFMessageComposeViewController, didFinishWith result: MessageComposeResult) {
        
    }
    

    @IBOutlet weak var statusLabel: UILabel!
    let number = "123"
    var listKey: [String] = ["key0","key3","key6","key9","key12","key15"]
    var player: AVAudioPlayer?
    var distanceLL : Int = 0
    var distanceRR : Int = 0

 
    @IBOutlet weak var key0: UILabel!
    @IBOutlet weak var key3: UILabel!
    @IBOutlet weak var key6: UILabel!
    @IBOutlet weak var key9: UILabel!
    @IBOutlet weak var key12: UILabel!
    @IBOutlet weak var key15: UILabel!
    
    @IBOutlet weak var distanceR: UILabel!
    @IBOutlet weak var distanceL: UILabel!
    
    @IBOutlet weak var status: UILabel!
    var ref: DatabaseReference!
    let post: Dictionary = ["data": "",
                            "times": 0] as [String : Any]
    var checkD = ["LEFT": 19, "RIGHT" : 19]
    var times = ["LEFT" : 0, "RIGHT" : 0]
    var canPlay: Bool = false
    
    override func viewDidLoad() {
        super.viewDidLoad()
        ref = Database.database().reference()
        
        
        observeData()
        ref.child("SOS").observeSingleEvent(of: .value) { (snapshot) in
            let dic = snapshot.value as? NSDictionary
            let data = dic?["data"] as? Int
            if(data == 1){
                self.alert()
            }
            
        }
        
    }
    
    func play(name: String, pos: String){
        if(pos == "LEFT") {
            if(self.distanceLL < 50 && self.distanceRR < 50){
                self.playSound(name: "front")
            }else if(self.distanceLL < self.distanceRR){
                self.playSound(name: name)
            }
        } else {
            if(self.distanceLL < 50 && self.distanceRR < 50){
                self.playSound(name: "front")
            } else if(self.distanceLL > self.distanceRR){
                self.playSound(name: name)
            }
        }
    }
    
    func playSound(name: String) {
        var filename: String = "wav"
        if(name == "front"){
            filename = "mp3"
        }
        let path = Bundle.main.path(forResource: "\(name)", ofType:filename)!
        
        let url = URL(fileURLWithPath: path)
        
        do {
            player = try AVAudioPlayer(contentsOf: url)
            if(name == "AlarmAir") {
                player?.numberOfLoops = -1;
            }
            player?.play()
            
        } catch {
            // couldn't load file :(
            print("couldn't load file")
        }
    }
    
    func alert(){
        self.statusLabel.text = "ALERT"
        self.statusLabel.backgroundColor = UIColor.red
        self.statusLabel.textColor = UIColor.yellow
        self.playSound(name: "AlarmAir")
        
        let alert = UIAlertController(title: "DANGER!!", message: "PLEUM might be in trouble", preferredStyle: .alert )
        let action1 = UIAlertAction(title: "Call", style: .destructive) { (alert) in
            self.makeCall()
        }
        let action2 = UIAlertAction(title: "Stop Alert", style: .destructive) { (alert) in
            self.setAlertToNormal()
        }
        alert.addAction(action1)
        alert.addAction(action2)
        
        self.present(alert, animated: true, completion: nil)
    }
    func makeCall(){
        guard let number = URL(string: "tel://" + number) else { return }
        UIApplication.shared.open(number, options: [:], completionHandler: nil)
    }
    
    func setAlertToNormal(){
        self.statusLabel.text = "Normal"
        self.statusLabel.backgroundColor = UIColor.black
        self.statusLabel.textColor = UIColor.white
        self.player?.stop()
    }
    
    func observeData() {
        ref.child("SOS").observe(DataEventType.childChanged) { (snapshot) in
            if let isAlert = snapshot.value {
                let data = isAlert as? Int
                if(data == 1) {
                    self.alert()
                }
            }
        }
        
        ref.child("distance").observe(DataEventType.childChanged) { (snapshot) in
            let key = snapshot.key.last
            if let valueD = snapshot.value as? Int {
                if(key == "L"){
                    if(valueD < 400) {

                    self.distanceLL = valueD
                    self.distanceL.text = String(valueD)
                    }
                } else {
                    if(valueD < 400){
                        self.distanceRR = valueD
                        self.distanceR.text = String(valueD)
                    }
                }
            }
            
        }
        ref.child("key").observe(DataEventType.childChanged) { (snapshot) in
            let postDict = snapshot.value as? [String : AnyObject] ?? [:]
            let key = (snapshot.key)
            let indexafterKey = key.index(key.startIndex, offsetBy: 3)
            let code = key[indexafterKey...]
            print("times: \(String(describing: postDict["times"])) , lastkey: \(code)")
            if (postDict["times"]?.int32Value == 1){
                    if(["0","6","12"].contains(code)){
                        self.checkD["LEFT"] = Int(code)
                        if let a = self.checkD["LEFT"], let b = self.checkD["RIGHT"]{
                            if(a < b) {
                                print("a \(a), b \(b)")
                                if(self.times["LEFT"] == 1){
                                     self.play(name: "translate_tts (\(code))", pos: "LEFT")
                                    self.checkD["LEFT"] = 19
                                } else if (self.times["LEFT"] == 0){
                                    self.times["LEFT"] = 1
                                }
                            }
                        }
                        
                            
                    } else {
                        self.checkD["RIGHT"] = Int(code)
                        if let a = self.checkD["LEFT"], let b = self.checkD["RIGHT"]{
                            print(a,b)
                            if(b < a) {
                                print("a \(a), b \(b)")
                                if(self.times["RIGHT"] == 1){
                                    self.play(name: "translate_tts (\(code))", pos: "RIGHT")
                                    self.checkD["RIGHT"] = 19
                                } else if (self.times["RIGHT"] == 0){
                                    self.times["RIGHT"] = 1;
                                }
                            }
                        }
                    }
            } else {
                print("not qualified")
            }
        }
    }
}
    
    



    




