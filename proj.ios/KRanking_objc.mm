//
//  KRanking_objc.mm
//  SimpleGame
//
//  Created by Guest on 2014/01/23.
//
//

#import "KRanking_objc.h"
#import <KiiSDK/Kii.h>
#import "KiiAppSingleton.h"

#include "CallCPP.h"


@implementation KRanking_objc

//シングルトン
+ (instancetype) sharedInstance{
    static dispatch_once_t once;
    static KRanking_objc *sharedInstance;
    dispatch_once(&once, ^{
        sharedInstance =  [[self alloc] init];
    });
    return sharedInstance;
}

-(void)displayNameUpdate:(NSString*)name
{
    NSLog(@"displayNameUpdate %@", name);
    
    NSError *error;
    KiiUser *user = [KiiUser currentUser];
    [user setDisplayName:name];
    
    [user saveSynchronous:&error];
    if(error != nil) {
        NSLog(@"error %@", error);
    } else {
        NSLog(@"OK");
        _userDisplayName = user.displayName;
        NSLog(@"_userDisplayName %@", _userDisplayName);
    }
    
}

-(void)ranking_query_all
{
    NSLog(@"ranking_query_all");
    
    //m_appRankingBucket
    KiiBucket *bucket1 = [Kii bucketWithName:@"b_ranking"];
    
    NSError *error = nil;
    KiiQuery *all_query = [KiiQuery queryWithClause:nil];
    [all_query sortByDesc:@"score"];
    NSMutableArray *allResults = [NSMutableArray array];
    KiiQuery *nextQuery;
    NSArray *results = [bucket1 executeQuerySynchronous:all_query
                                              withError:&error
                                                andNext:&nextQuery];
    [allResults addObjectsFromArray:results];
    //NSLog(@"allResults %@ ", allResults);
    
    //ログ表示
    int size = [allResults count];
    NSLog(@"size %d",size);
    NSMutableArray *mArray = [NSMutableArray array];
    //xx strcpy(KRanking::ranking_buff, "");
    char buff[256];
    for(int i=0;i<size; i++){
        KiiObject* obj=[allResults objectAtIndex:i];
        //NSLog(@"obj %d %@",i, obj);
        
        NSString* name=[[obj dictionaryValue] objectForKey:@"name"];        //nilだとnilになる
        //NSString* score=[[obj dictionaryValue] objectForKey:@"score"];    //NSDecimalNumber に変換されてしまう
        NSDecimalNumber* dnscore=[[obj dictionaryValue] objectForKey:@"score"];
        NSString *score = [dnscore stringValue];
        NSLog(@"%d %@ %@", i, name, score);

        if(name==nil){  //nilだとmArrayに入らないので値を入れる
            name = @"nil dayo";
        }
        
        [mArray addObject:@{@"name" : name,@"score" : score}];  //JSONの返還元として使う

        /***
        //c++へ
        const char* c_name =  [name UTF8String];
        const char* c_score = [score UTF8String];
        
        sprintf(buff,"%s : %s \n", c_name, c_score );
        //xx strcat(KRanking::ranking_buff, buff);
        ***/
    }
    
    NSError *error2 = nil;
    NSData *data = nil;
    NSString* json_str = nil;
    if([NSJSONSerialization isValidJSONObject:mArray]){
        NSLog(@"true isValidJSONObject");
        data = [NSJSONSerialization dataWithJSONObject:mArray options:NSJSONReadingAllowFragments error:&error2];
        //NSLog(@"%@",data);
        json_str = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]autorelease];
        //NSLog(@"json_str %@", json_str);
        
        CallCpp::rankingResponse( [json_str UTF8String] );  //C++を呼び出す
    } else {
        NSLog(@"false isValidJSONObject");
    }

}


-(void)ranking_post:(NSString*)name Score:(int)score
{
    NSLog(@"ranking_post %@ %d", name, score);
    [self ranking_query:name Score:score];
}


-(void)ranking_query:(NSString*)name Score:(int)score
{
    NSLog(@"KRanking::ranking_query %d",score);
    //xx g_name = @"muku";   //for test
    
    //m_appRankingBucket
    KiiBucket *bucket1 = [Kii bucketWithName:@"b_ranking"];
    
    NSError *error = nil;
    KiiClause *clause1 = [KiiClause equals:@"name" value:name];  //muku　でテスト
    KiiQuery *query = [KiiQuery queryWithClause:clause1];
    
    [query sortByDesc:@"score"];
    NSMutableArray *allResults = [NSMutableArray array];
    KiiQuery *nextQuery;
    NSArray *results = [bucket1 executeQuerySynchronous:query
                                              withError:&error
                                                andNext:&nextQuery];
    [allResults addObjectsFromArray:results];
    //NSLog(@"allResults %@ ", allResults);
    
    //ログ表示
    int size = [allResults count];
    int myScore = 0;
    NSString *uri = nil;
    //g_uri = nil;
    for(int i=0;i<size; i++){
        KiiObject* obj=[allResults objectAtIndex:i];
        NSString* name=[[obj dictionaryValue] objectForKey:@"name"];
        //NSString* score=[[obj dictionaryValue] objectForKey:@"score"];    //NSDecimalNumber これに変換されてしまう、後で型を調べる
        NSDecimalNumber* dnscore=[[obj dictionaryValue] objectForKey:@"score"];
        //NSString *score = [dnscore stringValue];
        
        NSLog(@"%d %@ %@", i, name, dnscore);
        
        double doubleValue = [dnscore doubleValue];
        int iscore = (int)doubleValue;
        NSLog(@"iscore %d", iscore);
        if(iscore>myScore){
            myScore = iscore;
            uri = obj.objectURI;
            NSLog(@"myScore %d %@", myScore, uri);
        }
        
    }
    
    if(score>myScore){	//sizeが0の場合myScoreが0のままなのでscoreが0より大きいと真になる
        //if(true){
        NSLog(@"hiscore");
        if(uri!=nil){
            NSLog(@"uri %@", uri);
            [self ranking_save:uri Name:name Score:score];
        } else {
            NSLog(@"uri %@", uri);
            [self ranking_save:uri Name:name Score:score];//uriはnullなので新規作成 save
        }
    } else{
        NSLog(@"not hiscore");
    }
}


-(void) ranking_save:(NSString*)uri Name:(NSString*)name Score:(int)score
{
    //void KRanking::ranking_save(NSString *uri, NSString *name, int score){
    //NSLog(@"ranking_save %@ %@ %d", g_uri, g_name, score);
    NSLog(@"ranking_save %@ %@ %d", uri, name, score);
    
    
    KiiObject *object;
    if(uri==nil){
        NSLog(@"新規作成");
        KiiBucket *bucket = [Kii bucketWithName:@"b_ranking"];
        object = [bucket createObject];
    } else {
        NSLog(@"更新");
        object = [KiiObject objectWithURI:uri];
    }
    
    NSError *error;
    
    // Create/add new values
    NSString *sscore = [NSString stringWithFormat:@"%d", score];
    NSDecimalNumber* dnscore = [NSDecimalNumber decimalNumberWithString:sscore];
    
    NSLog(@"dnscore %@", dnscore);
    
    [object setObject:name forKey:@"name"];
    //[object setObject:[NSNumber numberWithInt:987] forKey:@"score"];
    [object setObject:dnscore forKey:@"score"];
    
    [object saveAllFieldsSynchronous:TRUE withError:&error];
    
    if(error == nil) {
        NSLog(@"ok");
        [self ranking_refresh:uri];
    } else {
        NSLog(@"error %@", error);
    }
    
}


-(void) ranking_refresh:(NSString*)uri
{
    NSError *error = nil;
    
    KiiObject *object = [KiiObject objectWithURI:uri];
    [object refreshSynchronous:&error];
    if(error == nil) {
        NSString* name=[[object dictionaryValue] objectForKey:@"name"];
        NSDecimalNumber* dnscore=[[object dictionaryValue] objectForKey:@"score"];
        
        NSLog(@"ranking_refresh %@ %@", name, dnscore);
    } else {
        NSLog(@"ranking_refresh %@ ", error);
    }
    
}

-(void) regist
{
    NSLog(@"regist");
    
    NSUUID *vendorUUID = [UIDevice currentDevice].identifierForVendor;
    NSString *username = vendorUUID.UUIDString;
    NSString *password = @"1234";
    [[KiiAppSingleton sharedInstance] setUUID:username];
    NSLog(@"%@,%@", username,password);
    
    KiiUser* userObj=nil;
    userObj = [KiiUser userWithUsername:username andPassword:password];
    [userObj performRegistration:self withCallback:@selector(authProcessCompleteResist:withError:)];
}

-(void) loginWithToken
{
    NSLog(@"loginWithToken");
    
    bool b = [[KiiAppSingleton sharedInstance] loginWithToken]; //authenticateWithTokenSynchronousを使用
    if(b){
        NSLog(@"loginWithToken OK");
        KiiUser *user = [KiiUser currentUser];
        [KiiAppSingleton sharedInstance].currentUser = user;    //userを保存、変数
        _userDisplayName =user.displayName;
        NSLog(@"_userDisplayName = %@",_userDisplayName);
        CallCpp::setDisplayame( [_userDisplayName UTF8String] );  //C++を呼び出す
    } else {
        NSLog(@"loginWithToken NG");
        //userとpasswdを使ってloginがいい？
        //[self regist];
    }
}


-(void) loginWithUserName
{
    NSString* username = [[KiiAppSingleton sharedInstance] getUserName];
    NSString* password = [[KiiAppSingleton sharedInstance] getPassword];
    [KiiUser authenticate:username withPassword:password andDelegate:self andCallback:@selector(authProcessCompleteLogin:withError:)];
}


- (void) authProcessCompleteLogin:(KiiUser*)user withError:(KiiError*)error {
    NSLog(@"authProcessCompleteLogin");
    
    // the request was successful
    if(error == nil) {
        [KiiAppSingleton sharedInstance].currentUser = user;    //userを保存、変数
        [[KiiAppSingleton sharedInstance] registerToken];       //トークンを保存、NSUserDefaults
        [[KiiAppSingleton sharedInstance] setUserName:user.username];
        [[KiiAppSingleton sharedInstance] setPassword:@"1234"]; //1234固定
        //_userDisplayName = user.displayName;
        NSLog(@"_userDisplayName = %@",_userDisplayName);
        [self displayNameUpdate:@"PalyerName"]; //Synchronous _userDisplayNameを更新する
        CallCpp::setDisplayame( [_userDisplayName UTF8String] );  //C++を呼び出す
    } else {
        // there was a problem
        NSLog(@"error %@", error);
    }
}


- (void) authProcessCompleteResist:(KiiUser*)user withError:(KiiError*)error {
    NSLog(@"authProcessCompleteResist");
    
    // the request was successful
    if(error == nil) {
        [KiiAppSingleton sharedInstance].currentUser = user;    //userを保存、変数
        [[KiiAppSingleton sharedInstance] registerToken];       //トークンを保存、NSUserDefaults
        //_userDisplayName = user.displayName;
        NSLog(@"_userDisplayName = %@",_userDisplayName);
        [self displayNameUpdate:@"PalyerName"]; //Synchronous _userDisplayNameを更新する
        CallCpp::setDisplayame( [_userDisplayName UTF8String] );  //C++を呼び出す
    } else {
        // there was a problem
        NSLog(@"error %@", error);
    }
}


@end
