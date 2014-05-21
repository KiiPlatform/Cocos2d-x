//
//  KiiAPICall.mm
//  cocos2dx-cpp-IFV3
//
//  Created by Guest on 2014/03/20.
//
//

#import "KiiAPICall.h"
#import <KiiSDK/Kii.h>


//ログ出力
#define DEBUG_kiiRanking
#ifndef DEBUG_kiiRanking
#define MYNSLog( m, args... )
#else
#define MYNSLog( m, args... ) NSLog( m, ##args )
#endif
//--


@implementation KiiAPICall


// 引数にBlockのコールバック関数を受け取る処理
-(void)doSomeWorkWith:(CallbackHandler)handler {
    MYNSLog(@"KiiAPICall doSomeWorkWith");
    MYNSLog(@"_serviceID=%d", _serviceID);
    MYNSLog(@"_json_map=%@", _json_map);
    
    // 何らかの処理を行います。
    // ここではコールバックに渡す内容のインスタンスを作成しているのみ。
    NSError *error = nil;
    //NSMutableDictionary *dictonary = [NSMutableDictionary dictionary];
    NSString *result =@"hoge";
    
    // コールバックが指定された場合には、それを呼び出す。
    if (handler) {
        MYNSLog(@"handler");
        handler(result, error);
    }
}

-(void)run_object_save:(CallbackHandler)handler {
    MYNSLog(@"KiiAPICall run_object_save");
    MYNSLog(@"_serviceID=%d", _serviceID);
    MYNSLog(@"_json_map=%@", _json_map);

    __block NSString *result = nil;

    MYNSLog(@"新規作成");
    KiiObject *object;
    NSString *backet_key = [_json_map objectForKey:@"backet_key"];
    KiiBucket *bucket = [Kii bucketWithName:backet_key];
    object = [bucket createObject];
    
    //setを取り出して実行する
    //[object setObject:_username forKey:@"name"];
    for (id key in [_json_map keyEnumerator]) {
        //MYNSLog(@"Key:%@ Value:%@", key, [dictionary valueForKey:key]);
        NSString *val = [_json_map objectForKey:key];
        MYNSLog(@"key:%@  val:%@",key,val);
        if( [key hasPrefix:@"set_"] ){
            NSArray *keys = [key componentsSeparatedByString:@"set_"];
            //MYNSLog(@"%@  %@",keys,val);
            NSString *key2 = [keys objectAtIndex:1];
            [object setObject:val forKey:key2]; //setする
            MYNSLog(@"set %@  %@",key2,val);
        }
    }

    [object saveAllFields:TRUE withBlock:^(KiiObject *object, NSError *error) {
        if(error == nil) {
            MYNSLog(@"ok");
            
            NSMutableDictionary *mdic = [NSMutableDictionary dictionary];
            NSString *uri = object.objectURI;
            [mdic setObject:uri forKey:@"uri"];
            
            //JSONにする
            NSError *error2 = nil;
            NSData *data = nil;
            if([NSJSONSerialization isValidJSONObject:mdic]){
                MYNSLog(@"true isValidJSONObject");
                data = [NSJSONSerialization dataWithJSONObject:mdic options:NSJSONReadingAllowFragments error:&error2];
                //result
                result = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
                MYNSLog(@"result %@", result);
            } else {
                result = nil;
                MYNSLog(@"false isValidJSONObject");
            }
            
        } else {
            MYNSLog(@"error %@", error);
            result = [self makeErrorJsonString:error];
        }
        
        // コールバックが指定された場合には、それを呼び出す。
        if (handler) {
            MYNSLog(@"handler");
            handler(result, error); //errorは使用しない
        }
    }];

}

-(void)run_object_refresh:(CallbackHandler)handler {
    MYNSLog(@"KiiAPICall run_object_refresh");
    MYNSLog(@"_serviceID=%d", _serviceID);
    MYNSLog(@"_json_map=%@", _json_map);
    
    NSError *error   = nil;
    NSString *result = nil;
    
    // 何らかの処理を行います。
    // ここではコールバックに渡す内容のインスタンスを作成しているのみ。
    NSString *s_uri = [_json_map objectForKey:@"uri"];
    MYNSLog(@"s_uri %@",s_uri);
    
    //refresh
    KiiObject *object = [KiiObject objectWithURI:s_uri];
    [object refreshSynchronous:&error];
    if(error == nil) {
        NSString* name=[[object dictionaryValue] objectForKey:@"name"];
        NSDecimalNumber* dnscore=[[object dictionaryValue] objectForKey:@"score"];
        
        MYNSLog(@"ranking_refresh %@ %@", name, dnscore);
        
        NSDictionary *mdic = [object dictionaryValue];
        
        //JSONにする
        NSError *error2 = nil;
        NSData *data = nil;
        if([NSJSONSerialization isValidJSONObject:mdic]){
            MYNSLog(@"true isValidJSONObject");
            data = [NSJSONSerialization dataWithJSONObject:mdic options:NSJSONReadingAllowFragments error:&error2];
            //result
            result = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
            MYNSLog(@"result %@", result);
        } else {
            result = nil;
            MYNSLog(@"false isValidJSONObject");
        }
        
    } else {
        MYNSLog(@"ranking_refresh %@ ", error);
        result = [self makeErrorJsonString:error];
    }
    
    // コールバックが指定された場合には、それを呼び出す。
    if (handler) {
        MYNSLog(@"handler");
        handler(result, error); //errorは使用しない
    }
}

-(void)run_object_update:(CallbackHandler)handler {
    MYNSLog(@"KiiAPICall run_object_update");
    MYNSLog(@"_serviceID=%d", _serviceID);
    MYNSLog(@"_json_map=%@", _json_map);
    
    NSError *error   = nil;
    NSString *result = nil;
    
    // 何らかの処理を行います。
    // ここではコールバックに渡す内容のインスタンスを作成しているのみ。
    NSString *s_uri = [_json_map objectForKey:@"uri"];
    MYNSLog(@"s_uri %@",s_uri);
    
    //refresh
    KiiObject *object = [KiiObject objectWithURI:s_uri];
    [object refreshSynchronous:&error];
    if(error == nil) {
        NSString* name=[[object dictionaryValue] objectForKey:@"name"];
        NSDecimalNumber* dnscore=[[object dictionaryValue] objectForKey:@"score"];
        
        MYNSLog(@"ranking_refresh %@ %@", name, dnscore);
    } else {
        MYNSLog(@"ranking_refresh %@ ", error);
    }

    //setを取り出して実行する
    //[object setObject:_username forKey:@"name"];
    for (id key in [_json_map keyEnumerator]) {
        //MYNSLog(@"Key:%@ Value:%@", key, [dictionary valueForKey:key]);
        //NSString *val = [_json_map objectForKey:key];   //NSStringじゃないかも
        id id_val = [_json_map objectForKey:key];   //NSStringじゃないかも

        MYNSLog(@"key:%@  id_val:%@",key,id_val);
        if( [key hasPrefix:@"set_"] ){
            NSArray *keys = [key componentsSeparatedByString:@"set_"];
            //MYNSLog(@"%@  %@",keys,val);
            NSString *key2 = [keys objectAtIndex:1];
            MYNSLog(@"object=%@ key2=%@  id_val=%@",object,key2,id_val);
            [object setObject:id_val forKey:key2]; //setする
            //MYNSLog(@"set %@  %@",key2,id_val);
        }
    }
    
    //save
    MYNSLog(@"saveAllFieldsSynchronous ---");
    [object saveAllFieldsSynchronous:TRUE withError:&error];
    if(error == nil) {
        MYNSLog(@"ok");
        
        NSMutableDictionary *mdic = [NSMutableDictionary dictionary];
        NSString *uri = object.objectURI;
        [mdic setObject:uri forKey:@"uri"];
        
        //JSONにする
        NSError *error2 = nil;
        NSData *data = nil;
        if([NSJSONSerialization isValidJSONObject:mdic]){
            MYNSLog(@"true isValidJSONObject");
            data = [NSJSONSerialization dataWithJSONObject:mdic options:NSJSONReadingAllowFragments error:&error2];
            //result
            result = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
            MYNSLog(@"result %@", result);
        } else {
            result = nil;
            MYNSLog(@"false isValidJSONObject");
        }
        
    } else {
        MYNSLog(@"error %@", error);
        result = [self makeErrorJsonString:error];
    }
    
    
    // コールバックが指定された場合には、それを呼び出す。
    if (handler) {
        MYNSLog(@"handler");
        handler(result, error); //errorは使用しない
    }
}

-(KiiClause *)createClause:(NSDictionary *)clause {
    MYNSLog(@"createClause %@",clause);
    
    KiiClause *kii_clause;
    NSString *field, *type, *value, *type2;
    NSString *lowerLimit;
    NSString *upperLimit;
    
    field = [clause objectForKey:@"field"];
    type =  [clause objectForKey:@"type"];
    value = [clause objectForKey:@"value"];

/***1
    lowerLimit = [clause objectForKey:@"lowerLimit"];
    lowerIncluded = [clause objectForKey:@"lowerIncluded"];
    upperLimit = [clause objectForKey:@"upperLimit"];
    upperIncluded = [clause objectForKey:@"upperIncluded"];
***/
    
    if([type isEqualToString:@"eq"]){
        kii_clause = [KiiClause equals:field value:value];  //equals
    } else if([type isEqualToString:@"not"]){
        kii_clause = [KiiClause notEquals:field value:value];  //notEquals
    } else if([type isEqualToString:@"range"]){
        type2 =  [clause objectForKey:@"type2"];
        //greaterThan
        if([type2 isEqualToString:@"greaterThan"]){                  //greaterThan
            lowerLimit = [clause objectForKey:@"lowerLimit"];
            kii_clause = [KiiClause greaterThan:field value:lowerLimit];
            
        } else if([type2 isEqualToString:@"greaterThanOrEqual"]){    //greaterThanOrEqual
            lowerLimit = [clause objectForKey:@"lowerLimit"];
            kii_clause = [KiiClause greaterThanOrEqual:field value:lowerLimit];
            
        } else if([type2 isEqualToString:@"lessThan"]){              //lessThan
            upperLimit = [clause objectForKey:@"upperLimit"];
            kii_clause = [KiiClause lessThan:field value:upperLimit];
            
        } else if([type2 isEqualToString:@"lessThanOrEqual"]){       //lessThanOrEqual
            upperLimit = [clause objectForKey:@"upperLimit"];
            kii_clause = [KiiClause lessThanOrEqual:field value:upperLimit];
            
        } else {
            kii_clause = nil;
        }
    } else {
        kii_clause = nil;
    }
    
    return kii_clause;
}

-(void)run_query:(CallbackHandler)handler {
    MYNSLog(@"KiiAPICall run_query");
    MYNSLog(@"_serviceID=%d", _serviceID);
    MYNSLog(@"_json_map=%@", _json_map);
    
    KiiQuery *kii_query=nil;
    //KiiClause *totalClause = nil;

    //run_queryの処理 バラバラにする
    //query
    //NSString *result;
    NSDictionary *query = [_json_map objectForKey:@"query"];
    MYNSLog(@"query=%@", query);
    if(query!=nil){
        NSNumber *descending = [query objectForKey:@"descending"];

        NSString *orderBy = [query objectForKey:@"orderBy"];
        MYNSLog(@"descending=%@", descending);
        MYNSLog(@"orderBy=%@", orderBy);
    
        //clause
        NSDictionary *clause = [query objectForKey:@"clause"];
        MYNSLog(@"clause=%@", clause);
        
/***
        NSString *field, *type, *value;
        if(clause!=nil){
            type = [clause objectForKey:@"type"];
            
            if([type isEqualToString:@"and"] || [type isEqualToString:@"or"]){
                NSArray *clauses = [clause objectForKey:@"clauses"];
                MYNSLog(@"clauses=%@", clauses);
                
                int size = [clauses count];
                MYNSLog(@"size %d",size);
                for(int i=0;i<size; i++){
                    NSDictionary *clause2 =[clauses objectAtIndex:i];
                    MYNSLog(@"clause2 %d, %@",i,clause2);
                    field = [clause2 objectForKey:@"field"];
                    type =  [clause2 objectForKey:@"type"];
                    value = [clause2 objectForKey:@"value"];
                    
                    MYNSLog(@"field=%@", field);
                    MYNSLog(@"type=%@",  type);
                    MYNSLog(@"value=%@", value);
                }
                // Combine the clauses with an 'AND'
                //KiiClause *totalClause = [KiiClause and:clause1, clause2, nil];
                //KiiClause *totalClause = nil;
                KiiClause *kii_clause1,*kii_clause2,*kii_clause3,*kii_clause4,*kii_clause5;
                
                type = [clause objectForKey:@"type"];
                
                if([type isEqualToString:@"and"]){
                    //and
                    if(size==1){
                        //ないはず
                    } else if(size==2){
                        kii_clause1 = [self createClause:[clauses objectAtIndex:0]];
                        kii_clause2 = [self createClause:[clauses objectAtIndex:1]];
                        totalClause = [KiiClause and:kii_clause1, kii_clause2, nil];
                        
                    } else if(size==3){
                        kii_clause1 = [self createClause:[clauses objectAtIndex:0]];
                        kii_clause2 = [self createClause:[clauses objectAtIndex:1]];
                        kii_clause3 = [self createClause:[clauses objectAtIndex:2]];
                        totalClause = [KiiClause and:kii_clause1, kii_clause2, kii_clause3, nil];
                        
                    } else if(size==4){
                        kii_clause1 = [self createClause:[clauses objectAtIndex:0]];
                        kii_clause2 = [self createClause:[clauses objectAtIndex:1]];
                        kii_clause3 = [self createClause:[clauses objectAtIndex:2]];
                        kii_clause4 = [self createClause:[clauses objectAtIndex:3]];
                        totalClause = [KiiClause and:kii_clause1, kii_clause2, kii_clause3, kii_clause4, nil];
                        
                    } else if(size==5){
                        kii_clause1 = [self createClause:[clauses objectAtIndex:0]];
                        kii_clause2 = [self createClause:[clauses objectAtIndex:1]];
                        kii_clause3 = [self createClause:[clauses objectAtIndex:2]];
                        kii_clause4 = [self createClause:[clauses objectAtIndex:3]];
                        kii_clause5 = [self createClause:[clauses objectAtIndex:4]];
                        totalClause = [KiiClause and:kii_clause1, kii_clause2, kii_clause3, kii_clause4, kii_clause5, nil];

                    } else {
                        //ないはず
                    }
                } else if([type isEqualToString:@"or"]){
                    //or
                    if(size==1){
                        
                    } else if(size==2){
                        kii_clause1 = [self createClause:[clauses objectAtIndex:0]];
                        kii_clause2 = [self createClause:[clauses objectAtIndex:1]];
                        totalClause = [KiiClause or:kii_clause1, kii_clause2, nil];
                        
                    } else if(size==3){
                        kii_clause1 = [self createClause:[clauses objectAtIndex:0]];
                        kii_clause2 = [self createClause:[clauses objectAtIndex:1]];
                        kii_clause3 = [self createClause:[clauses objectAtIndex:2]];
                        totalClause = [KiiClause or:kii_clause1, kii_clause2, kii_clause3, nil];
                        
                    } else if(size==4){
                        kii_clause1 = [self createClause:[clauses objectAtIndex:0]];
                        kii_clause2 = [self createClause:[clauses objectAtIndex:1]];
                        kii_clause3 = [self createClause:[clauses objectAtIndex:2]];
                        kii_clause4 = [self createClause:[clauses objectAtIndex:3]];
                        totalClause = [KiiClause or:kii_clause1, kii_clause2, kii_clause3, kii_clause4, nil];
                        
                    } else if(size==5){
                        kii_clause1 = [self createClause:[clauses objectAtIndex:0]];
                        kii_clause2 = [self createClause:[clauses objectAtIndex:1]];
                        kii_clause3 = [self createClause:[clauses objectAtIndex:2]];
                        kii_clause4 = [self createClause:[clauses objectAtIndex:3]];
                        kii_clause5 = [self createClause:[clauses objectAtIndex:4]];
                        totalClause = [KiiClause or:kii_clause1, kii_clause2, kii_clause3, kii_clause4, kii_clause5, nil];

                    } else {
                        //ないはず
                    }
                } else {
                    //はいはず
                }
                
                //MYNSLog(@"return");
                //return;
            } else {
                //１つだけ
                field = [clause objectForKey:@"field"];
                type =  [clause objectForKey:@"type"];
                value = [clause objectForKey:@"value"];
        
                MYNSLog(@"field=%@", field);
                MYNSLog(@"type=%@",  type);
                MYNSLog(@"value=%@", value);
                
                totalClause = [self createClause:clause];
            }
        } else {
            //エラー、エラー処理はあとで
            MYNSLog(@"error!!");
            return;
        }
 
        kii_query = [KiiQuery queryWithClause:totalClause];  //queryが出来た
***/
        
        kii_query = [KiiQuery queryWithDictionary:clause];
        
        if( (descending!=nil) && (orderBy!=nil) ){
            NSNumber *intNumber1 = [[NSNumber alloc]initWithInteger:1];
            if([descending isEqualToNumber:intNumber1]){
                [kii_query sortByDesc:orderBy];
            } else {
                [kii_query sortByAsc:orderBy];
            }
        }
        
        //Bucketの作成
        NSString *backet_key = [_json_map objectForKey:@"backet_key"];
        KiiBucket *kii_bucket = [Kii bucketWithName:backet_key];
        
/***
        //実行
        NSError *error = nil;
        NSMutableArray *allResults = [NSMutableArray array];
        KiiQuery *nextQuery;
        NSArray *results = [kii_bucket executeQuerySynchronous:kii_query    //同期で実行
                                                  withError:&error
                                                    andNext:&nextQuery];
***/
        //実行 blockを使用
        // To recursively get all results in a query (from multiple pages)
        KiiQueryResultBlock __block __weak weakQueryBlock;
        KiiQueryResultBlock queryBlock = ^(KiiQuery *retQuery, KiiBucket *retBucket, NSArray *retResults, KiiQuery *retNextQuery, NSError *retError) {
            
            MYNSLog(@"queryBlock ---");
            MYNSLog(@"retError %@",retError);
            
            // We got some valid results
            //if (retError == nil) {
            if (true) {
                // Do something with the results
                [self handleResults:retResults err:retError handler:handler];    //結果の処理
            }
            
            // We have another query available (another page of results)
            if (retNextQuery != nil) {
                // Execute the next query
                [kii_bucket executeQuery:retNextQuery withBlock:[weakQueryBlock copy]];
            }
        };
        weakQueryBlock = queryBlock;
        
        MYNSLog(@"executeQuery ---");
        //KiiQuery *all_query = [KiiQuery queryWithClause:nil];
        [kii_bucket executeQuery:kii_query withBlock:queryBlock];
        //[kii_bucket executeQuery:all_query withBlock:queryBlock];
        //
    }
    
/***
        //結果
        [allResults addObjectsFromArray:results];
        MYNSLog(@"allResults1 %@ ", allResults);
        
        //ログ表示 _uri_の追加
        NSMutableArray *jArray = [NSMutableArray array];
        int size = [allResults count];
        for(int i=0;i<size; i++){
            KiiObject* obj=[allResults objectAtIndex:i];
            NSMutableDictionary *dic = [obj dictionaryValue];
            //_uri_を追加
            NSString *uri = obj.objectURI;
            [dic setObject:uri forKey:@"_uri_"];
            MYNSLog(@"%d %@", i, dic);
            [jArray addObject:dic];
        }
        MYNSLog(@"jArray %@", jArray);
        
        //JSONにする
        NSError *error2 = nil;
        NSData *data = nil;
        //NSString* json_str = nil;
        if([NSJSONSerialization isValidJSONObject:jArray]){
            MYNSLog(@"true isValidJSONObject");
            data = [NSJSONSerialization dataWithJSONObject:jArray options:NSJSONReadingAllowFragments error:&error2];
            //MYNSLog(@"%@",data);
            //result
            result = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]autorelease];
            MYNSLog(@"result %@", result);
            
            //CallCpp::setDisplayame2([json_str UTF8String], serviceID);
            //CallCpp::rankingResponse( [json_str UTF8String] );  //C++を呼び出す
        } else {
            result = nil;
            MYNSLog(@"false isValidJSONObject");
        }
        //
        
    } else {
        result = nil;
    }
    // コールバックが指定された場合には、それを呼び出す。
    // ここではコールバックに渡す内容のインスタンスを作成しているのみ。
    NSError *error = nil;
    if (handler) {
        MYNSLog(@"handler");
        handler(result, error);
    }
 ***/
}

//結果の処理
-(void)handleResults:(NSArray *)retResults err:(NSError *)retError handler:(CallbackHandler)handler{
    MYNSLog(@"handleResults %@ %@", retResults, retError);

    //retError = (NSError *)@"hoge erro dayo";    //for debug
    
    NSString *result;
    if(retError == nil) {
    
        //ログ表示 _uri_の追加
        NSMutableArray *jArray = [NSMutableArray array];
        int size = [retResults count];
        for(int i=0;i<size; i++){
            KiiObject* obj=[retResults objectAtIndex:i];
            NSMutableDictionary *dic = [NSMutableDictionary dictionaryWithDictionary:[obj dictionaryValue]];
            //_uri_を追加
            NSString *uri = obj.objectURI;
            [dic setObject:uri forKey:@"_uri_"];
            //MYNSLog(@"%d %@", i, dic);
            [jArray addObject:dic];
        }
        MYNSLog(@"jArray %@", jArray);
    
        //JSONにする
        NSError *error2 = nil;
        NSData *data = nil;
        //NSString* json_str = nil;
        if([NSJSONSerialization isValidJSONObject:jArray]){
            MYNSLog(@"true isValidJSONObject");
            data = [NSJSONSerialization dataWithJSONObject:jArray options:NSJSONReadingAllowFragments error:&error2];
            //MYNSLog(@"%@",data);
            //result
            result = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
            MYNSLog(@"result %@", result);
        
            //CallCpp::setDisplayame2([json_str UTF8String], serviceID);
            //CallCpp::rankingResponse( [json_str UTF8String] );  //C++を呼び出す
        } else {
            result = nil;
            MYNSLog(@"false isValidJSONObject");
        }
    //
    } else {
        MYNSLog(@"error %@", retError);
        result = [self makeErrorJsonString:retError];
    }
    NSError *error = nil;
    if (handler) {
        MYNSLog(@"handler");
        handler(result, error);
    }

    
}

-(NSString *)makeErrorJsonString:(NSError *)error
{
    MYNSLog(@"makeErrorJsonString %@", error);
    
    NSString *result;
    NSData *data = nil;
    NSError *error2 = nil;
    
    NSMutableDictionary *mdic = [[NSMutableDictionary alloc]init];
    // TODO: set other useful information.
    mdic[@"_error_"] = @{
                         @"code" : [NSNumber numberWithInt:error.code]
                         };

    NSDictionary *dic = [NSDictionary dictionaryWithDictionary:mdic];
    if([NSJSONSerialization isValidJSONObject:dic]){
        data = [NSJSONSerialization dataWithJSONObject:dic
                                               options:NSJSONReadingAllowFragments
                                                 error:&error2];
        result = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    } else {
        // Won't reach here.
        result = @"{\"_error_\" : \"Unexpected error.\"}";
    }
    return result;
}




@end
