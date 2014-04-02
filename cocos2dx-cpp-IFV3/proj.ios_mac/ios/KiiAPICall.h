//
//  KiiAPICall.h
//  cocos2dx-cpp-IFV3
//
//  Created by Guest on 2014/03/20.
//
//

#import <Foundation/Foundation.h>

//typedef void (^CallbackHandler)(NSDictionary * resultDictonary, NSError *error);
typedef void (^CallbackHandler)(NSString * resultDictonary, NSError *error);


@interface KiiAPICall : NSObject {
@public
    int _serviceID;
    NSDictionary *_json_map;
}

// 引数にBlockのコールバック関数を受け取る処理を定義します。
-(void)doSomeWorkWith:(CallbackHandler)handler;

/***
// idとselectorを引数で受け取るメソッドを定義します。
//-(void)doSomeWorkWith:(id)callbackInstance selector:(SEL)callbackSelector;
***/

-(void)run_object_save:(CallbackHandler)handler;
-(void)run_object_refresh:(CallbackHandler)handler;
-(void)run_object_update:(CallbackHandler)handler;
-(void)run_query:(CallbackHandler)handler;

@end
