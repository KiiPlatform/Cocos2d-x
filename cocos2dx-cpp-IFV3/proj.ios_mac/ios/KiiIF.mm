//
//  KiiIF.m
//  cocos2dx-cpp-IFV3
//
//  Created by Guest on 2014/03/17.
//
//

#import <KiiSDK/Kii.h>
#import "KiiIF.h"
#include "CallCPP.h"

//ログ出力
#define DEBUG_KiiIF
#ifndef DEBUG_KiiIF
#define MYNSLog( m, args... )
#else
#define MYNSLog( m, args... ) NSLog( m, ##args )
#endif
//--

@implementation KiiIF

+(KiiIF*) sharedInstance {
    
    static KiiIF *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[KiiIF alloc] init];
        //sharedInstance.internetReach=[Reachability reachabilityForInternetConnection];
        
    });
    return sharedInstance;
}

-(void)kiiReq:(int)serviceID json:(const char *)json{
    MYNSLog(@"KiiIF kiiReq");
    [self decodeJson:serviceID json:json];
}

-(void)decodeJson:(int)serviceID json:(const char *)json{
    MYNSLog(@"KiiIF decodeJson %s",json); //%s
    
    NSString *s = [ [ NSString alloc ] initWithUTF8String:json ];
    NSData *data = [s dataUsingEncoding:NSUTF8StringEncoding];
    NSError *error = nil;
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
    MYNSLog(@"dic %@",dic);
    
    NSString *cmd_val = [dic objectForKey:@"cmd"];
    MYNSLog(@"cmd_val %@",cmd_val);
    
    
    /***
        cmdにてコマンドを判定し分岐する
     ***/
    BOOL b;
    //1
    //createApplicationScopeBucket
    b = [cmd_val isEqualToString:@"createApplicationScopeBucket"];
    if(b){
        MYNSLog(@"decodeJson exec createApplicationScopeBucket");
        [self createApplicationScopeBucket:serviceID json_map:dic];
    }
    
    //2
    //object_save
    b = [cmd_val isEqualToString:@"object_save"];
    if(b){
        MYNSLog(@"decodeJson exec object_save");
        [self object_save:serviceID json_map:dic];
    }
    
    //3
    //object_refresh
    b = [cmd_val isEqualToString:@"object_refresh"];
    if(b){
        MYNSLog(@"decodeJson exec object_refresh");
        [self object_refresh:serviceID json_map:dic];
    }
    
    //4
    //object_update
    b = [cmd_val isEqualToString:@"object_update"];
    if(b){
        MYNSLog(@"decodeJson exec object_update");
        [self object_update:serviceID json_map:dic];
    }
    
    //5
    //bucket_query
    b = [cmd_val isEqualToString:@"bucket_query"];
    if(b){
        MYNSLog(@"decodeJson exec bucket_query");
        [self bucket_query:serviceID json_map:dic];
    }
    
    MYNSLog(@"KiiIF decodeJson end ---");
}

//1
-(void)createApplicationScopeBucket:(int)serviceID json_map:(NSDictionary *)json_map{
    MYNSLog(@"KiiIF createApplicationScopeBucket %d, %@ ---",serviceID,json_map);

    kiiAPICall = [[KiiAPICall alloc] init];
    kiiAPICall->_json_map = json_map;
    kiiAPICall->_serviceID = serviceID;
    
    //Bucketの作成
    NSString *backet_key = [json_map objectForKey:@"backet_key"];
    KiiBucket *bucket = [Kii bucketWithName:backet_key];
    MYNSLog(@"backet_key=%@", backet_key);
    
    //JSONの作成
    NSError *error = nil;
    NSMutableDictionary *dictonary = [NSMutableDictionary dictionary];
    [dictonary setObject:backet_key forKey:@"backet_key"];
    
    NSError *error2 = nil;
    NSData *data = nil;
    NSString* json_str = nil;
    if([NSJSONSerialization isValidJSONObject:dictonary]){
        MYNSLog(@"true isValidJSONObject");
        data = [NSJSONSerialization dataWithJSONObject:dictonary options:NSJSONReadingAllowFragments error:&error2];
        json_str = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]autorelease];
        MYNSLog(@"json_str %@", json_str);
        
        CallCpp::setDisplayame2([json_str UTF8String], serviceID);
    } else {
        MYNSLog(@"false isValidJSONObject");
    }
    
    
    //CallCpp::setDisplayame2(str1, serviceID);
    /***
    [kiiAPICall doSomeWorkWith:^(NSDictionary *resultDictonary, NSError *error) {
        
        // ここの中がコールバック処理。
        // 引数で結果（resultDictionary）とエラー内容(error)を受け取る。
        MYNSLog(@"CallbackHandler called.");
        MYNSLog(@"resultDictonary = %@", resultDictonary);
        MYNSLog(@"error = %@", error);
        
        //c++をコール
        char str1[10];
        str1[0] = 0;
        CallCpp::setDisplayame2(str1, serviceID);
    }];
    ***/
    
    //[kiiAPICall doSomeWorkWith:self selector:@selector(callbackFprWorkWithResult:error:)];//OK
    //[kiiAPICall doSomeWorkWith:self selector:@selector(callbackFprWorkWithResult2:json_map:)];
    //[kiiAPICall doSomeWorkWith2:self selector:select:@selector(callbackFprWorkWithResult:error:) serviceID:serviceID json_map:json_map];
    
    MYNSLog(@"KiiIF createApplicationScopeBucket end ---");
}
// コールバックで呼んでもらうメソッドを実装します。
-(void)callbackFprWorkWithResult:(NSDictionary *)resultDictionary error:(NSError *)error {
    MYNSLog(@"callbackForWorkWithResult:error: is called.");
    MYNSLog(@"resultDictonary = %@", resultDictionary);
    MYNSLog(@"error = %@", error);
}

-(void)callbackFprWorkWithResult2:(NSNumber *)num_serviceID json_map:(NSDictionary *)json_map {
    MYNSLog(@"callbackForWorkWithResult2:json_map: is called.");
    MYNSLog(@"serviceID = %@", num_serviceID);
    MYNSLog(@"json_map = %@", json_map);
}

//2
-(void)object_save:(int)serviceID json_map:(NSDictionary *)json_map{
    MYNSLog(@"KiiIF object_save");
    
    kiiAPICall = [[KiiAPICall alloc] init];
    kiiAPICall->_json_map = json_map;
    kiiAPICall->_serviceID = serviceID;
    
    [kiiAPICall run_object_save:^(NSString *result, NSError *error) {    //run_object_update
        // ここの中がコールバック処理。
        // 引数で結果（resultDictionary）とエラー内容(error)を受け取る。
        MYNSLog(@"CallbackHandler called.");
        MYNSLog(@"result = %@", result);
        MYNSLog(@"error = %@", error);
        
        //c++をコール
        const char *str1 = [result UTF8String];
        CallCpp::setDisplayame2(str1, serviceID);
    }];
}

//3
-(void)object_refresh:(int)serviceID json_map:(NSDictionary *)json_map{
    MYNSLog(@"KiiIF object_refresh");
    
    kiiAPICall = [[KiiAPICall alloc] init];
    kiiAPICall->_json_map = json_map;
    kiiAPICall->_serviceID = serviceID;
    
    [kiiAPICall run_object_refresh:^(NSString *result, NSError *error) {    //run_object_update
        // ここの中がコールバック処理。
        // 引数で結果（resultDictionary）とエラー内容(error)を受け取る。
        MYNSLog(@"CallbackHandler called.");
        MYNSLog(@"result = %@", result);
        MYNSLog(@"error = %@", error);
        
        //c++をコール
        const char *str1 = [result UTF8String];
        CallCpp::setDisplayame2(str1, serviceID);
    }];
}

//4
-(void)object_update:(int)serviceID json_map:(NSDictionary *)json_map{
    MYNSLog(@"KiiIF object_update");
    
    kiiAPICall = [[KiiAPICall alloc] init];
    kiiAPICall->_json_map = json_map;
    kiiAPICall->_serviceID = serviceID;
    
    [kiiAPICall run_object_update:^(NSString *result, NSError *error) {    //run_object_update
        // ここの中がコールバック処理。
        // 引数で結果（resultDictionary）とエラー内容(error)を受け取る。
        MYNSLog(@"CallbackHandler called.");
        MYNSLog(@"result = %@", result);
        MYNSLog(@"error = %@", error);
        
        //c++をコール
        const char *str1 = [result UTF8String];
        CallCpp::setDisplayame2(str1, serviceID);
    }];
}

//5
-(void)bucket_query:(int)serviceID json_map:(NSDictionary *)json_map{
    MYNSLog(@"KiiIF bucket_query");
    
    kiiAPICall = [[KiiAPICall alloc] init];
    kiiAPICall->_json_map = json_map;
    kiiAPICall->_serviceID = serviceID;
    
    [kiiAPICall run_query:^(NSString *result, NSError *error) {    //run_query
        // ここの中がコールバック処理。
        // 引数で結果（resultDictionary）とエラー内容(error)を受け取る。
        MYNSLog(@"CallbackHandler called.");
        MYNSLog(@"result = %@", result);
        MYNSLog(@"error = %@", error);
     
        //c++をコール
        const char *str1 = [result UTF8String];
        CallCpp::setDisplayame2(str1, serviceID);
    }];
}

@end
