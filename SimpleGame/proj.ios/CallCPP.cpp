//
//  CallCPP.cpp
//  SimpleGame
//
//  Created by Guest on 2014/01/22.
//
//

#include "CallCPP.h"

extern void iPhone_rankingResponse(const char *json);
extern void iPhone_setDisplayame(const char *name);


void CallCpp::rankingResponse(const char *json){
    iPhone_rankingResponse(json);
}

void CallCpp::setDisplayame(const char *name){
    iPhone_setDisplayame(name);
}