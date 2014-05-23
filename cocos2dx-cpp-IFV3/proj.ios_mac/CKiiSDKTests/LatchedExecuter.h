//
//  LatchUtil.h
//  KiiSDK-Private
//
//  Created by Riza Alaudin Syah on 10/10/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LatchedExecuter : NSObject
-(BOOL) execute: (void (^) (void)) block withTimeOutSec :(int) timeoutSec;
-(void) offTheLatch;


@end
