//
//  KiiIF.h
//  cocos2dx-cpp-IFV3
//
//  Created by Guest on 2014/03/17.
//
//

#import <Foundation/Foundation.h>
#import "SomeWorker.h"
#import "KiiAPICall.h"

@interface KiiIF : NSObject{
    NSMutableDictionary *bucket_map, *service_map;
    // 呼び出すWorkerのインスタンス
    SomeWorker *someWorker;
    KiiAPICall *kiiAPICall;
}


// シングルトン
+ (KiiIF *)sharedInstance;

/***
-(void)regist;
-(void)loginWithToken;
-(void)ranking_query_all;
/Users/guest/Documents/cocos2d-x/cocos2d-x-2.2.2/projects/MyApp/cocos2dx-cpp-IFV3/proj.ios_mac/ios/KiiIF.h-(void)ranking_post:(NSString*)name Score:(int)score;
-(void)displayNameUpdate:(NSString*)name;
 ***/

-(void)kiiReq:(int)serviceID json:(const char *)json;
-(void)decodeJson:(int)serviceID json:(const char *)json;

-(void)createApplicationScopeBucket:(int)serviceID json_map:(NSDictionary *)json_map;
-(void)object_save:(int)serviceID json_map:(NSDictionary *)json_map;
-(void)object_refresh:(int)serviceID json_map:(NSDictionary *)json_map;
-(void)object_update:(int)serviceID json_map:(NSDictionary *)json_map;
-(void)bucket_query:(int)serviceID json_map:(NSDictionary *)json_map;

// コールバックで呼んでもらうメソッド
-(void)callbackFprWorkWithResult:(NSDictionary *)resultDictionary error:(NSError *)error;
-(void)callbackFprWorkWithResult2:(NSNumber *)num_serviceID json_map:(NSDictionary *)json_map;


@end
