//
//  SomeWorker.mm
//  cocos2dx-cpp-IFV3
//
//  Created by Guest on 2014/03/20.
//
//

#import "SomeWorker.h"

@implementation SomeWorker

//非同期で処理するもの、後でコールバックを実行する
-(void)doSomeWorkWith:(id)callbackInstance selector:(SEL)callbackSelector {
    NSLog(@"SomeWorker doSomeWorkWith");
/***
    
    // 何か処理を行う。HTTP通信やファイル扱いなど。
    NSError *error = nil;
    NSMutableDictionary *dictonary =
    
    // コールバックが指定されている場合には、コールバックを行う。
    if (callbackInstance && callbackSelector) {
        NSMethodSignature *sig = [callbackInstance methodSignatureForSelector:callbackSelector];
        NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:sig];
        [invocation setSelector:callbackSelector];
        [invocation setTarget:callbackInstance];
        // コールバックの引数に値を指定する場合には、index >=2を指定する。
        [invocation setArgument:&dictonary atIndex:2];
        [invocation setArgument:&error atIndex:3];
        [invocation invoke];
    }
***/
    NSError *error = nil;
    NSMutableDictionary *dictonary =nil;
    
    //コールバックの実行
    // callback.
    // 引数が２つまでの場合には、以下のように簡単にも書ける。
    if (callbackInstance && callbackSelector) {
        NSLog(@"SomeWorker doSomeWorkWith callbackInstance");
        [callbackInstance performSelector:callbackSelector withObject:dictonary withObject:error];
    } else{
        NSLog(@"SomeWorker doSomeWorkWith callbackInstance nasi");
    }
    
}

@end
