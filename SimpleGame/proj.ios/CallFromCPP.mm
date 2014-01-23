//
//  CallFromCPP.cpp
//  SimpleGame
//
//  Created by Guest on 2014/01/22.
//
//

#include "CallFromCPP.h"
#import "KRanking_objc.h"

void CallFromCpp::ranking_query_all(){
    NSLog(@"CallFromCpp::ranking_query_all");

    [[KRanking_objc sharedInstance] ranking_query_all];
}


void CallFromCpp::ranking_post(const char *name, int score){
    NSLog(@"CallFromCpp::ranking_post %s %d", name ,score);
    
    NSString* ns_name = [NSString stringWithCString: name encoding:NSUTF8StringEncoding];
    [[KRanking_objc sharedInstance] ranking_post:ns_name Score:score ];
}