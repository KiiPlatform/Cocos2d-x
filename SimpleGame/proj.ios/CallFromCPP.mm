//
//  CallFromCPP.cpp
//  SimpleGame
//
//  Created by Guest on 2014/01/22.
//
//

#include "CallFromCPP.h"

#include "KRanking.h"


void CallFromCpp::ranking_query_all(){
    NSLog(@"CallFromCpp::ranking_query_all");

    KRanking::ranking_query_all();
    
}
