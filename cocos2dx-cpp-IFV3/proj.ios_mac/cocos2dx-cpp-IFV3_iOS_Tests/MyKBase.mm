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

void MyKBase::setCompletionFunc(std::function<void (const char *)> completionFunc)
{
    this->completionFunc = completionFunc;
}

void MyKBase::myCallback(const char *json)
{
    if (this->completionFunc)
    {
        this->completionFunc(json);
    }
};