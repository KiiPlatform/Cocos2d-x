//
//  KRanking.mm
//  SimpleGame
//
//  Created by Guest on 2014/01/21.
//
//

#include "KRanking.h"

#import <KiiSDK/Kii.h>

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
        NSDecimalNumber* score=[[obj dictionaryValue] objectForKey:@"score"];
        NSLog(@"%d %@ %@", i, name, score);
        
        [mArray addObject:@{@"name" : name,@"score" : score}];
        
        //c++へ
        const char* c_name =  [name UTF8String];
        const char* c_score = [score UTF8String];
        
        sprintf(buff,"%s : %s \n", c_name, c_score );
        strcat(KRanking::ranking_buff, buff);//í«â¡
    }
    
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
        NSLog(@"json_str %@", json_str);
    } else {
        NSLog(@"false isValidJSONObject");
    }
    
}