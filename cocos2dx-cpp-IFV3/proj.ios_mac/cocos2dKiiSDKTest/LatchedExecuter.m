//
//  LatchUtil.m
//  KiiSDK-Private
//
//  Created by Riza Alaudin Syah on 10/10/13.
//  Copyright (c) 2013 Kii Corporation. All rights reserved.
//

#import "LatchedExecuter.h"

#import <sys/time.h>
@implementation LatchedExecuter{
    __block long long _execId;
    __block BOOL _shouldWait;
    
}
+ (long long)currentTimeMillis {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (((long long) t.tv_sec) * 1000) + (((long long) t.tv_usec) / 1000);
}

-(BOOL) execute: (void (^) (void)) block withTimeOutSec :(int) timeoutSec{
    block();
    _shouldWait = YES;
    __block BOOL isTimeOut = NO;
    if (timeoutSec>0) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            double delayInSeconds = timeoutSec;
            long long qId = [LatchedExecuter currentTimeMillis];
            _execId = qId;
            dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
            dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
                
                if (_execId==qId) {
                    _shouldWait = NO;
                    isTimeOut= YES;
                }
            });
        });
        
    }
    
    while( _shouldWait && [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]] );
    
    return !isTimeOut;
}

-(void) offTheLatch{
    _shouldWait = NO;
}
@end
