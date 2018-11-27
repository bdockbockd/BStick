//
//  Round.swift
//  uiStickMan
//
//  Created by Yotsatornbenz on 27/11/2561 BE.
//  Copyright © 2561 BB. All rights reserved.
//

import UIKit

@IBDesignable class Round: UILabel
{
    override func layoutSubviews() {
        super.layoutSubviews()
        
        updateCornerRadius()
    }
    
    @IBInspectable var rounded: Bool = false {
        didSet {
            updateCornerRadius()
        }
    }
    
    func updateCornerRadius() {
        layer.masksToBounds = true
        layer.cornerRadius = 6

       
    }
}
