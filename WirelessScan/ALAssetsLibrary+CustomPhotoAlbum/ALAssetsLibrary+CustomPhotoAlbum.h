//
//  ALAssetsLibrary+CustomPhotoAlbum.h
//  WirelessScan
//
//  Created by Gan Sweet on 14-10-20.
//  Copyright (c) 2014年 SweetGan. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <AssetsLibrary/AssetsLibrary.h>
#import <UIKit/UIKit.h>


typedef void(^SaveImageCompletion)(NSError* error);

@interface ALAssetsLibrary(CustomPhotoAlbum)
-(void)saveImage:(UIImage*)image toAlbum:(NSString*)albumName withCompletionBlock:(SaveImageCompletion)completionBlock;

-(void)addAssetURL:(NSURL*)assetURL toAlbum:(NSString*)albumName withCompletionBlock:(SaveImageCompletion)completionBlock;
@end