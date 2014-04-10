//
//  CallFromCPP.cpp
//  SimpleGame
//
//  Created by Guest on 2014/01/22.
//
//

#include "CallFromCPP.h"
#import "KRanking_objc.h"
#import "KiiIF.h"

//ログ出力
//#define DEBUG_CallFromCPP
#ifndef DEBUG_CallFromCPP
#define MYNSLog( m, args... )
#else
#define MYNSLog( m, args... ) NSLog( m, ##args )
#endif
//--

void CallFromCpp::ranking_query_all(){
    MYNSLog(@"CallFromCpp::ranking_query_all");

    [[KRanking_objc sharedInstance] ranking_query_all];
}


void CallFromCpp::ranking_post(const char *name, int score){
    MYNSLog(@"CallFromCpp::ranking_post %s %d", name ,score);
    
    NSString* ns_name = [NSString stringWithCString: name encoding:NSUTF8StringEncoding];
    [[KRanking_objc sharedInstance] ranking_post:ns_name Score:score ];
}

void CallFromCpp::save_display_name(const char *name){
    MYNSLog(@"CallFromCpp::save_display_name %s", name);
    
    NSString* ns_name = [NSString stringWithCString: name encoding:NSUTF8StringEncoding];
    [[KRanking_objc sharedInstance] displayNameUpdate:ns_name];
}

void CallFromCpp::kiiReq(const char *json, int servideID){
     MYNSLog(@"CallFromCpp::kiiReq %s", json);
    
    [[KiiIF sharedInstance] kiiReq:servideID json:json];
    
}
