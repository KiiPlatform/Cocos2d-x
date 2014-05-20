//
//  MyKBase.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/20.
//
//

#include "MyKBase.h"

MyKBase::MyKBase()
{
}

const char* MyKBase::getCallbackJson()
{
    while (true) {
        sleep(1);
        if (callbackJson != NULL)
            return callbackJson;
    }
}

void MyKBase::nullifyCallbackJson()
{
    callbackJson = NULL;
}

void MyKBase::myCallback(const char *json)
{
    callbackJson = json;
};