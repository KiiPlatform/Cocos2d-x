//
//  KRanking.mm
//  SimpleGame
//
//  Created by Guest on 2014/01/21.
//
//

#include "KRanking.h"
#include "CallCPP.h"

#import <KiiSDK/Kii.h>

NSString *g_uri;
NSString *g_name;

char KRanking::ranking_buff[2000];

void KRanking::ranking_query_all(){
    
    NSLog(@"KRanking::ranking_query_all");
    
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
    NSMutableArray *mArray = [NSMutableArray array];
    strcpy(KRanking::ranking_buff, "");
    char buff[256];
    for(int i=0;i<size; i++){
        KiiObject* obj=[allResults objectAtIndex:i];
        //NSLog(@"obj %@",obj);
        
        NSString* name=[[obj dictionaryValue] objectForKey:@"name"];
        //NSString* score=[[obj dictionaryValue] objectForKey:@"score"];    //NSDecimalNumber これに変換されてしまう、後で型を調べる
        NSDecimalNumber* dnscore=[[obj dictionaryValue] objectForKey:@"score"];
        NSString *score = [dnscore stringValue];
        NSLog(@"%d %@ %@", i, name, score);
        
        [mArray addObject:@{@"name" : name,@"score" : score}];
        
        //c++へ
        const char* c_name =  [name UTF8String];
        const char* c_score = [score UTF8String];
        
        sprintf(buff,"%s : %s \n", c_name, c_score );
        strcat(KRanking::ranking_buff, buff);//í«â¡
    }
    NSLog(@"KRanking::ranking_buff %s", KRanking::ranking_buff);
    
    //id dict = [NSDictionary dictionaryWithObjectsAndKeys:@"1",@"hoge",@"2",@"fuga", nil];
    //id dict = [NSArray allResults];
    
    /***
     NSMutableArray *mArray = [NSMutableArray array];
     [mArray addObject:@{@"name" : @"名前1",@"score" : @"12341"}];
     [mArray addObject:@{@"name" : @"名前2",@"score" : @"12342"}];
     [mArray addObject:@{@"name" : @"名前3",@"score" : @"12343"}];
     ***/
    
    
    NSError *error2 = nil;
    NSData *data = nil;
    NSString* json_str = nil;
    if([NSJSONSerialization isValidJSONObject:mArray]){
        NSLog(@"true isValidJSONObject");
        data = [NSJSONSerialization dataWithJSONObject:mArray options:NSJSONReadingAllowFragments error:&error2];
        //NSLog(@"%@",data);
        //NSLog(@"%@",[[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]autorelease]);
        json_str = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]autorelease];
        //NSLog(@"json_str %@", json_str);
        
        CallCpp::rankingResponse( [json_str UTF8String] );
    } else {
        NSLog(@"false isValidJSONObject");
    }
    
}


void KRanking::ranking_query(int score){
    
    NSLog(@"KRanking::ranking_query %d",score);
    g_name = @"muku";   //for test
    
    //m_appRankingBucket
    KiiBucket *bucket1 = [Kii bucketWithName:@"b_ranking"];
    
    NSError *error = nil;
    KiiClause *clause1 = [KiiClause equals:@"name" value:g_name];  //muku　でテスト
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
    //NSString *uri = nil;
    g_uri = nil;
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
            g_uri = obj.objectURI;
            NSLog(@"myScore %d %@", myScore, g_uri);
        }

    }
    
    if(score>myScore){	//sizeが0の場合myScoreが0のままなのでscoreが0より大きいと真になる
        //if(true){
        NSLog(@"hiscore");
        if(g_uri!=nil){
            NSLog(@"g_uri %@", g_uri);
            ranking_save(score);	//uriを使って更新 save
        } else {
            NSLog(@"g_uri %@", g_uri);
            ranking_save(score);//uriはnullなので新規作成 save
        }
    } else{
        NSLog(@"not hiscore");
    }
}

void KRanking::ranking_save(int score){
//void KRanking::ranking_save(NSString *uri, NSString *name, int score){
    //NSLog(@"ranking_save %@ %@ %d", g_uri, g_name, score);
    NSLog(@"ranking_save %@ %@ %d", g_uri, g_name, score);

    
    KiiObject *object;
    if(g_uri==nil){
        NSLog(@"新規作成");
        KiiBucket *bucket = [Kii bucketWithName:@"b_ranking"];
        object = [bucket createObject];
    } else {
        NSLog(@"更新");
        object = [KiiObject objectWithURI:g_uri];
    }
    
    NSError *error;
    
    // Create/add new values
    NSString *sscore = [NSString stringWithFormat:@"%d", score];
    NSDecimalNumber* dnscore = [NSDecimalNumber decimalNumberWithString:sscore];

    NSLog(@"dnscore %@", dnscore);
    
    [object setObject:g_name forKey:@"name"];
    //[object setObject:[NSNumber numberWithInt:987] forKey:@"score"];
    [object setObject:dnscore forKey:@"score"];

    [object saveAllFieldsSynchronous:TRUE withError:&error];
    
    if(error == nil) {
        NSLog(@"ok");
        ranking_refresh();
    } else {
        NSLog(@"error %@", error);
    }
    
}

void KRanking::ranking_refresh(){
    NSError *error = nil;
    
    KiiObject *object = [KiiObject objectWithURI:g_uri];
    [object refreshSynchronous:&error];
    if(error == nil) {
        NSString* name=[[object dictionaryValue] objectForKey:@"name"];
        NSDecimalNumber* dnscore=[[object dictionaryValue] objectForKey:@"score"];
        
        NSLog(@"ranking_refresh %@ %@", name, dnscore);
    } else {
        NSLog(@"ranking_refresh %@ ", error);
    }
    

}

