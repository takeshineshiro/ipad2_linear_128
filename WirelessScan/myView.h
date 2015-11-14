//
//  myView.h
//  WirelessScan
//
//  Created by book on 14-11-18.
//  Copyright (c) 2014年 SweetGan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ViewController.h"
@interface myView : UIView{

    NSInteger lineCount;
    double scale;
}
@property(nonatomic) double scale;
@property(nonatomic) NSInteger lineCount;
@end
