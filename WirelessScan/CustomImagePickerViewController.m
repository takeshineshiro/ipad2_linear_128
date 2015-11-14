//
//  CustomImagePickerViewController.m
//  WirelessScan
//
//  Created by book on 15-3-8.
//  Copyright (c) 2015年 SweetGan. All rights reserved.
//

#import "CustomImagePickerViewController.h"

@interface CustomImagePickerViewController ()

@end

@implementation CustomImagePickerViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.userInteractionEnabled = true;

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

-(void)viewDidAppear:(BOOL)animated{
    [super viewDidAppear:animated];
    [[UIApplication sharedApplication] setStatusBarHidden:true withAnimation:UIStatusBarAnimationNone];

}

-(void)viewWillDisappear:(BOOL)animated{

    [super viewWillDisappear:animated];
    [[UIApplication sharedApplication] setStatusBarHidden:true withAnimation:UIStatusBarAnimationNone];

}



@end
