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
extern void iPhone_setDisplayame2(const char *str1, int serviceID);
extern void iPhone_setName(const char *name);

void CallCpp::rankingResponse(const char *json){
    iPhone_rankingResponse(json);
}

void CallCpp::setDisplayame(const char *name){
    iPhone_setDisplayame(name);
}

void CallCpp::setDisplayame2(const char *str1, int serviceID){
    iPhone_setDisplayame2(str1, serviceID);
}

void CallCpp::setName(const char *name){
    iPhone_setName(name);
}