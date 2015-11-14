//
//  RawImag.h
//  WirelessScan
//
//  Created by book on 15/3/14.
//  Copyright (c) 2015年 SweetGan. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface RawImag : NSObject

@property (nonatomic) NSInteger probeType;

@property(nonatomic) int gain;

@property(nonatomic) int zoom;

@property(nonatomic) NSString* time;

@property(nonatomic)NSData* rawData;

@end
